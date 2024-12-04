/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <link.h>
#include <sys/wait.h>

#include "test.h"

#define MAX_BUF 256
#define TEST_NUM 1000
#define TEST_SIZE 4096
#define CALLBACK_TEST_VAL 2

const char* g_libPath = "/data/local/tmp/libc-test-lib/libdlopen_dso.so";
const char* g_initlibPath = "/data/local/tmp/libc-test-lib/libdlopen_init.so";
void* g_init_handler = NULL;
static int test_value = 0;

int check_loaded(char* so)
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

    char buffer[MAX_BUF] = { 0 };
    while (fgets(buffer, MAX_BUF, fp) != NULL) {
        if (strstr(buffer, so) != NULL) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

static int CallBack001(struct dl_phdr_info* info, size_t size, void* data)
{
    if (strcmp(info->dlpi_name, g_libPath) != 0 || strcmp(info->dlpi_name, g_initlibPath) != 0) {
        return 0;
    }
    test_value++;
    if (test_value != 1) {
        t_error("test_value should be 1, but: %d\n", test_value);
    }
    test_value--;

    if (test_value != 0) {
        t_error("test_value should be 0, but: %d\n", test_value);
    }
    return 0;
}

static int CallBack002(struct dl_phdr_info* info, size_t size, void* data)
{
    if (strcmp(info->dlpi_name, g_libPath) != 0 || strcmp(info->dlpi_name, g_initlibPath) != 0) {
        return 0;
    }
    test_value = test_value + CALLBACK_TEST_VAL;
    if (test_value != CALLBACK_TEST_VAL) {
        t_error("test_value should be 2, but: %d\n", test_value);
    }

    test_value = test_value - CALLBACK_TEST_VAL;
    if (test_value != 0) {
        t_error("test_value should be 0, but: %d\n", test_value);
    }
    return 0;
}

static void* CallBack003(void* arg)
{
    pid_t pid = fork();
    if (pid > 0) {
        int status = 0;
        int options = 0;
        pid_t waitpid_for_pind = waitpid(pid, &status, options);
        if (waitpid_for_pind != pid) {
            t_error("%s waitpid get pid is %d are not want %d\n", __func__, waitpid_for_pind, pid);
        }
        if (status != 0) {
            t_error("%s waitpid get status is %d are not 0\n", __func__, status);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(0);
    } else {
        t_error("%s waitpid fork error\n");
    }
    return arg;
}

static int CallBack004(struct dl_phdr_info* info, size_t size, void* data)
{
    char *memory = (char *)malloc(TEST_SIZE);
    free(memory);
    return 0;
}

static void* dlopen_dlclose_test001(void* arg)
{
    void* handle = dlopen(g_libPath, RTLD_NOW);
    if (!handle) {
      t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath, RTLD_NOW, dlerror());
    }
    dlclose(handle);
    return arg;
}

static void* dlopen_dlclose_test002(void* arg)
{
    void* handle = dlopen(g_initlibPath, RTLD_NOW);
    if (!handle) {
      t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_initlibPath, RTLD_NOW, dlerror());
    }
    g_init_handler = handle;
    return arg;
}

static void* dlopen_dlclose_test003(void* arg)
{
    dlclose(g_init_handler);
    return arg;
}

static void* dlopen_dlclose_test0041(void* arg)
{
    dl_iterate_phdr(CallBack001, NULL);
    return arg;
}

static void* dlopen_dlclose_test0042(void* arg)
{
    dl_iterate_phdr(CallBack002, NULL);
    return arg;
}

static void* dlopen_dlclose_test005(void* arg)
{
    int(* get_val)(void) = dlsym(g_init_handler, "getVal");
    if (get_val == NULL) {
        t_error("dlsym failed, don't find the symbol getVal\n");
    }
    if (get_val != NULL && get_val() != 1) {
        t_error("This val after init should be 1, but %d\n", get_val());
    }
    return arg;
}

static void* dlopen_dlclose_test006(void* arg)
{
    dl_iterate_phdr(CallBack004, NULL);
    return arg;
}

static void do_test_concurrently(void *(*test) (void *arg), size_t num_threads)
{
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
    if (threads == NULL) {
        t_error("Failed to allocate memory: %s\n", strerror(errno));
        return;
    }

    size_t last = 0;
    while (last < num_threads) {
        if (pthread_create(&(threads[last]), NULL, test, NULL)) {
            t_error("Failed to create thread: %s\n", strerror(errno));
            break;
        }
        last++;
    }

    for (size_t i = 0; i < last; i++) {
        if (pthread_join(threads[i], NULL)) {
            t_error("Failed to join thread: %s\n", strerror(errno));
        }
    }

    free(threads);
    return;
}

static void do_test_double_concurrently(void *(*test1) (void *arg), void *(*test2) (void *arg),
    size_t num_threads1, size_t num_threads2)
{
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * (num_threads1 + num_threads2));
    if (threads == NULL) {
        t_error("Failed to allocate memory: %s\n", strerror(errno));
        return;
    }

    size_t last = 0;
    while (last < num_threads1) {
        if (pthread_create(&(threads[last]), NULL, test1, NULL)) {
            t_error("Failed to create thread: %s\n", strerror(errno));
            break;
        }
        last++;
    }

    while (last < num_threads1 + num_threads2) {
        if (pthread_create(&(threads[last]), NULL, test2, NULL)) {
            t_error("Failed to create thread: %s\n", strerror(errno));
            break;
        }
        last++;
    }

    for (size_t i = 0; i < last; i++) {
        if (pthread_join(threads[i], NULL)) {
            t_error("Failed to join thread: %s\n", strerror(errno));
        }
    }

    free(threads);
    return;
}

/**
 * @tc.name      : dl_multithread_lock_0100
 * @tc.desc      : multithreaded dlopen/dlclose, at the end the expected so file should not be in memory.
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0100(void)
{
    size_t num_threads = 1000;
    do_test_concurrently(dlopen_dlclose_test001, num_threads);
    if (check_loaded((char*)g_libPath)) {
        t_error("This so file should not exist, %s\n", (char*)g_libPath);
    }
}

/**
 * @tc.name      : dl_multithread_lock_0200
 * @tc.desc      : multithreaded dlopen, the init constructor should be called only once.
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0200(void)
{
    size_t num_threads = 20;
    do_test_concurrently(dlopen_dlclose_test002, num_threads);
    if (!check_loaded((char*)g_initlibPath)) {
        t_error("This so file should exist, %s\n", (char*)g_initlibPath);
    }
    int(* get_val)(void) = dlsym(g_init_handler, "getVal");
    if (get_val == NULL) {
        t_error("dlsym failed, don't find the symbol getVal\n");
    }
    if (get_val != NULL && get_val() != 1) {
        t_error("This val after init should be 1, but %d\n", get_val());
    }
}

/**
 * @tc.name      : dl_multithread_lock_0300
 * @tc.desc      : multithreaded dlopen, the deconstructor should be called only at the last dlclose.
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0300(void)
{
    size_t num_threads = 19;
    do_test_concurrently(dlopen_dlclose_test003, num_threads);
    if (!check_loaded((char*)g_initlibPath)) {
        t_error("This so file should exist, %s\n", (char*)g_initlibPath);
    }
    int(* get_val)(void) = dlsym(g_init_handler, "getVal");
    if (get_val == NULL) {
        t_error("dlsym failed, don't find the symbol getVal\n");
    }
    if (get_val != NULL && get_val() != 1) {
        t_error("This val after init should be 1, but %d\n", get_val());
    }
    dlclose(g_init_handler);
    if (check_loaded((char*)g_initlibPath)) {
        t_error("This so file should not exist, %s\n", (char*)g_initlibPath);
    }
}

/**
 * @tc.name      : dl_multithread_lock_0400
 * @tc.desc      : multithreaded iterate Callback in dl_iterate_phdr, the static test_value should be thread safe.
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0400(void)
{
    void* handle1 = dlopen(g_libPath, RTLD_NOW);
    if (!handle1) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath, RTLD_NOW, dlerror());
    }

    void* handle2 = dlopen(g_initlibPath, RTLD_NOW);
    if (!handle2) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_initlibPath, RTLD_NOW, dlerror());
    }

    size_t num_threads = 5;
    do_test_double_concurrently(dlopen_dlclose_test0041, dlopen_dlclose_test0042, num_threads, num_threads);

    dlclose(handle1);
    dlclose(handle2);
    if (check_loaded((char*)g_initlibPath) || check_loaded((char*)g_libPath)) {
        t_error("These so files should not exist\n");
    }
}

/**
 * @tc.name      : dl_multithread_lock_0500
 * @tc.desc      : multithreaded dlsym, dlsym should not be blocked by dlsym in other threads.
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0500(void)
{
    void* handle = dlopen(g_initlibPath, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_initlibPath, RTLD_NOW, dlerror());
    }

    g_init_handler = handle;
    size_t num_threads = 1000;
    do_test_concurrently(dlopen_dlclose_test005, num_threads);
    g_init_handler = NULL;
    dlclose(handle);
}

/**
 * @tc.name      : dl_multithread_lock_0600
 * @tc.desc      : malloc and fork in different threads to check that there is no ABBA deadlock
 *                (ld lock and jemalloc lock).
 * @tc.level     : Level 0
 */
void dl_multithread_lock_0600(void)
{
    size_t num_threads = 100;
    do_test_double_concurrently(dlopen_dlclose_test006, CallBack003, num_threads, num_threads);
}

int main(int argc, char* argv[])
{
    dl_multithread_lock_0100();
    dl_multithread_lock_0200();
    dl_multithread_lock_0300();
    dl_multithread_lock_0400();
    dl_multithread_lock_0500();
    dl_multithread_lock_0600();
    return t_status;
}