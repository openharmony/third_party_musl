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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "test.h"
#define MAX_BUF 256
#define HEX_TERM 16
#define RELRO_INDEX 3
#define RELRO_NUM 1
#define BUFFER_SIZE 1024
#define PID_BUFFER_SIZE 128

const char *g_libPath = "/system/lib64/libc++.so";
const char *g_keylog = "ro_seal report: provition_type: 1, reason: 2, filename: /system/lib64/libc++.so";

int dlclose_check(unsigned long start, unsigned long end, const char *so)
{
    if (mprotect((void *)start, end - start, PROT_READ | PROT_WRITE) == 0) {
        t_error("the relro segment of %s should be readonly and cannot be changed\n", so);
        return 0;
    }
    int fd = open(so, O_RDONLY);
    if (fd < 0) {
        t_error("open file %s should be success but failed, errno %d\n", so, errno);
        return 0;
    }
    if (mmap((void *)start, end - start, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, 0) != MAP_FAILED) {
        t_error("mmap to the fixed region of so %s, start: %zu, end: %zu should fail but success\n",
            so, start, end);
        close(fd);
        munmap((void *)start, end - start);
        return 0;
    }
    close(fd);
    return 1;
}

int check_load_permission(const char *so, int is_dlclose)
{
    int pid = getpid();
    const char *so_name = NULL;
    char path[MAX_BUF] = { 0 };
    if (sprintf(path, "/proc/%d/maps", pid) < 0) {
       t_error("Failed in sprintf: %s\n", strerror(errno));
    }
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }
    if (strchr(so, '/')) {
        so_name = strrchr(so, '/') + 1;
    }
    char buffer[MAX_BUF] = { 0 };
    unsigned long start = 0;
    unsigned long end = 0;
    int seg_num = 0;
    while (fgets(buffer, MAX_BUF, fp) != NULL) {
        if (strstr(buffer, so_name) != NULL) {
            char *end_ptr;
            seg_num++;
            if (is_dlclose && seg_num == RELRO_NUM) {
                start = strtoul(buffer, &end_ptr, HEX_TERM);
                end = strtoul(end_ptr + 1, &end_ptr, HEX_TERM);
            }
            if (!is_dlclose && seg_num == RELRO_INDEX) {
                start = strtoul(buffer, &end_ptr, HEX_TERM);
                end = strtoul(end_ptr + 1, &end_ptr, HEX_TERM);
            }
        }
    }
    fclose(fp);
    if (start != 0 && end != 0 && is_dlclose && seg_num == RELRO_NUM) {
        return dlclose_check(start, end, so);
    } else if (start != 0 && end != 0 && !is_dlclose && seg_num >= RELRO_INDEX) {
        mprotect((void*)start, end - start, PROT_READ | PROT_WRITE);
        return 1;
    }
    return 0;
}

void test_lib(void)
{
    void *handle = dlopen(g_libPath, RTLD_LAZY);
    if (!handle) {
        t_error("dlopen(name=%s, mode=%d) failed: %s\n", g_libPath, RTLD_LAZY, dlerror());
    }

    if (!check_load_permission(g_libPath, 0)) {
        t_error("check_load_permission failed for so %s\n", g_libPath);
    }
    dlclose(handle);
}

int main(int argc, char* argv[])
{
    int pipe_fd[2];
    pid_t pid;
    test_lib();

    if (pipe(pipe_fd) == -1) {
        t_error("create pipe failed\n");
        return t_status;
    }
    pid = fork();
    if (pid == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        t_error("fork failed\n");
        return t_status;
    }

    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execlp("dmesg", "dmesg", NULL);
        _exit(0);
    } else {
        close(pipe_fd[1]);
        FILE *pipe_read = fdopen(pipe_fd[0], "r");
        if (pipe_read == NULL) {
            t_error("Failed to open read pipe fd, errno %d\n", errno);
            close(pipe_fd[0]);
            return t_status;
        }
        char buffer[BUFFER_SIZE];
        char pid_buffer[PID_BUFFER_SIZE] = { 0 };
        int check_success = 0;
        if (sprintf(pid_buffer, "pid=%d", getpid()) < 0) {
            t_error("Failed in sprintf: %s\n", strerror(errno));
        }
        while (fgets(buffer, BUFFER_SIZE, pipe_read) != NULL) {
            if (strstr(buffer, g_keylog) && strstr(buffer, pid_buffer)) {
                check_success = 1;
                break;
            }
        }
        if (!check_success) {
            t_error("don't find correct log in dmesg: %s\n", g_keylog);
        }
        waitpid(pid, NULL, 0);
        fclose(pipe_read);
    }
    return t_status;
}

