/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <dlfcn.h>
#include <link.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include "test.h"
typedef void(*notify_call)(uintptr_t dso_base, size_t dso_len, const char *dso_path);
#define MAX_BUF 256
#define THREAD_NUMBER 3
#define SET_PROC_TYPE_CMD _IOW('E', 0x18, uint32_t)
#define ASSIGN_ENCAPS_CMD _IOW('E', 0x1A, char *)

const char* g_libPath1 = "/data/local/tmp/libc-test-lib/libdlopen_register_a.so";
const char* g_libPath2 = "/data/local/tmp/libc-test-lib/libdlopen_register_b.so";
static int test_value1 = 0;
static int test_value2 = 0;
static int test_value3 = 0;
static int test_value4 = 0;
static void *register_ptr = NULL;
static int so_nums = 0;

int check_loaded(const char *so, uintptr_t dso_base, size_t dso_len)
{
    int pid = getpid();
    char path[MAX_BUF] = { 0 };
    if (sprintf(path, "/proc/%d/maps", pid) < 0) {
       t_error("Failed in sprintf: %s\n", strerror(errno));
    }
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }
    if (strchr(so, '/')) {
        so = strrchr(so, '/') + 1;
    }

    char buffer[MAX_BUF] = { 0 };
    unsigned long start = 0;
    unsigned long end = 0;
    unsigned long base = 0;
    unsigned long size = 0;
    int has_check = 0;
    while (fgets(buffer, MAX_BUF, fp) != NULL) {
        if (strstr(buffer, so) != NULL) {
            char *end_ptr;
            start = strtoul(buffer, &end_ptr, 16);
            end = strtoul(end_ptr + 1, &end_ptr, 16);
            size += end - start;
            if (base == 0) {
                base = start;
                has_check = 1;
            }
        } else if (has_check) {
            break;
        }
    }
    fclose(fp);
    if (dso_base == base && size == dso_len) {
        return 1;
    }
    return 0;
}

void callback1(uintptr_t dso_base, size_t dso_len, const char *dso_path)
{
    test_value1++;
    int ret = check_loaded(dso_path, dso_base, dso_len);
    if (!ret) {
        t_error("Failed check in callback1 for so: %s\n", dso_path);
    }
}

void callback2(uintptr_t dso_base, size_t dso_len, const char *dso_path)
{
    test_value2++;
    int ret = check_loaded(dso_path, dso_base, dso_len);
    if (!ret) {
        t_error("Failed check in callback2 for so: %s\n", dso_path);
    }
}

void callback3(uintptr_t dso_base, size_t dso_len, const char *dso_path)
{
    test_value3++;
    int ret = check_loaded(dso_path, dso_base, dso_len);
    if (!ret) {
        t_error("Failed check in callback3 for so: %s\n", dso_path);
    }
}

void callback4(uintptr_t dso_base, size_t dso_len, const char *dso_path)
{
    test_value4++;
    int ret = check_loaded(dso_path, dso_base, dso_len);
    if (!ret) {
        t_error("Failed check in callback4 for so: %s\n", dso_path);
    }
}

static int header_handler(struct dl_phdr_info *info, size_t size, void *data)
{
    if (strlen(info->dlpi_name) == 0) {
        return 0;
    }
    so_nums++;
    return 0;
}

static notify_call func_list[THREAD_NUMBER] = {callback1, callback2, callback3};

struct callback_param {
    int (*register_func)(notify_call);
    notify_call callback;
};

void *add_callback(void *arg)
{
    struct callback_param *info = (struct callback_param *)arg;
    info->register_func(info->callback);
    return arg;
}

void add_callback_concurrently()
{
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * THREAD_NUMBER);
    if (threads == NULL) {
        t_error("Failed to allocate memory: %s\n", strerror(errno));
        return;
    }
    register_ptr = dlsym(RTLD_DEFAULT, "register_ldso_func_for_add_dso");
    if (register_ptr == NULL) {
        t_error("Failed to find register function in libc.so\n");
        return;
    }
    size_t index = 0;
    struct callback_param arg[THREAD_NUMBER] = {0};
    while (index < THREAD_NUMBER) {
        arg[index].register_func = register_ptr;
        arg[index].callback = func_list[index];
        if (pthread_create(&(threads[index]), NULL, add_callback, (void*)(&arg[index]))) {
            t_error("Failed to create thread: %s\n", strerror(errno));
            break;
        }
        index++;
    }
    for (size_t i = 0; i < index; i++) {
        if (pthread_join(threads[i], NULL)) {
            t_error("Failed to join thread: %s\n", strerror(errno));
        }
    }

    free(threads);
}

int do_iterate_check()
{
    so_nums = 0;
    if (dl_iterate_phdr(header_handler, NULL)) {
        t_error("Failed to run dl_iterate_phdr\n");
        return -1;
    }

    if (so_nums != test_value1 || so_nums != test_value2 || so_nums != test_value3) {
        t_error("callback check failed, so_nums: %d, test_value1: %d, test_value2: %d, test_value3: %d\n",
            so_nums, test_value1, test_value2, test_value3);
        return -1;
    }

    if (test_value4 != 0 && so_nums != test_value4) {
        t_error("callback second check failed, so_nums: %d, test_value4: %d\n",
            so_nums, test_value4);
        return -1;
    }
    return 0;
}

void *dlopen_thread1(void *arg)
{
    void* handle = dlopen(g_libPath1, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath1, RTLD_NOW, dlerror());
    }
    return arg;
}

void *dlopen_thread2(void *arg)
{
    void* handle = dlopen(g_libPath2, RTLD_LAZY);
    if (!handle) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath2, RTLD_LAZY, dlerror());
    }
    return arg;
}

void dlopen_concurrently()
{
    pthread_t td1, td2, td3;
    struct callback_param arg = {register_ptr, callback4};
    pthread_create(&td1, NULL, dlopen_thread1, NULL);
    pthread_create(&td2, NULL, dlopen_thread2, NULL);
    pthread_create(&td3, NULL, add_callback, (void *)(&arg));
    pthread_join(td1, NULL);
    pthread_join(td2, NULL);
    pthread_join(td3, NULL);
}

void set_proc_type()
{
    int fd = open("/dev/encaps", O_RDWR);
    if (fd < 0) {
        t_error("open /dev/encaps failed, errno: %d\n", errno);
        return;
    } else {
        uint32_t type = 4;
        ioctl(fd, SET_PROC_TYPE_CMD, &type);
        close(fd);
    }
}

void assign_encaps()
{
    int fd = open("/dev/encaps", O_RDWR);
    char str[] = "{\"encaps\":{\"ohos.encaps.count\":1, \"ohos.encaps.fork.count\":5}}";
    if (fd < 0) {
        t_error("open /dev/encaps failed, errno: %d\n", errno);
        return;
    } else {
        ioctl(fd, ASSIGN_ENCAPS_CMD, str);
        close(fd);
    }
}

int main(int argc, char* argv[])
{
    set_proc_type();
    assign_encaps();
    add_callback_concurrently();
    if (do_iterate_check() < 0) {
        return t_status;
    }
    dlopen_concurrently();
    do_iterate_check();
    return t_status;
}