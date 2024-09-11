/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <musl_log.h>
#include <stdio_impl.h>

#include <unordered_map>

#include "functionalext.h"

#define DEV_NULL_PATH "/dev/null"

void signal_handler_abort(int signum)
{
    kill(getpid(), SIGSTOP);
}

void signal_handler_abort1(int signum)
{
    exit(signum);
}

void fdsan_test_get_tag_value()
{
    int fd = open(DEV_NULL_PATH, O_RDONLY);
    uint64_t tag = fdsan_get_owner_tag(fd);
    EXPECT_EQ("fdsan_test_get_tag_value", strcmp(fdsan_get_tag_type(tag), "native object of unknown type"), 0);
    EXPECT_EQ("fdsan_test_get_tag_value", fdsan_get_tag_value(tag), 0);
    EXPECT_EQ("fdsan_test_get_tag_value", close(fd), 0);
}

void fdsan_test_overflow()
{
    std::unordered_map<int, uint64_t> fds;
    for (int i = 0; i < 4096; ++i) {
        int fd = open(DEV_NULL_PATH, O_RDONLY);
        auto tag = 0xdead00000000ULL | i;
        fdsan_exchange_owner_tag(fd, 0, tag);
        fds[fd] = tag;
    }

    for (auto [fd, tag] : fds) {
        EXPECT_EQ("fdsan_test_overflow", fdsan_close_with_tag(fd, tag), 0);
    }
}

void fdsan_test_vfork()
{
    int fd = open(DEV_NULL_PATH, O_RDONLY);

    pid_t rc = vfork();
    EXPECT_NE("fdsan_test_vfork", -1, rc);

    if (rc == 0) {
        close(fd);
        _exit(0);
    }

    int status;
    pid_t wait_result = waitpid(rc, &status, 0);
    EXPECT_EQ("fdsan_test_vfork", wait_result, rc);
    EXPECT_TRUE("fdsan_test_vfork", WIFEXITED(status));
    EXPECT_EQ("fdsan_test_vfork", 0, WEXITSTATUS(status));
}

void fdsan_test_fatal_level()
{
    struct sigaction sigabrt = {
        .sa_handler = signal_handler_abort,
    };
    sigaction(SIGABRT, &sigabrt, nullptr);

    int status;
    int pid = fork();
    switch (pid) {
        case -1: {
            t_error("fork failed: %d\n", __LINE__);
            break;
        }
        case 0: {
            fdsan_set_error_level(FDSAN_ERROR_LEVEL_FATAL);
            int fd = open(DEV_NULL_PATH, O_RDONLY);
            uint64_t tag = 0xdead00000000ULL;
            fdsan_exchange_owner_tag(fd, 0, tag);
            fdsan_close_with_tag(fd, 0);
            exit(0);
        }
        default: {
            waitpid(pid, &status, WUNTRACED);
            EXPECT_EQ("fdsan_test_fatal_level", WIFEXITED(status), 0);
            EXPECT_EQ("fdsan_test_fatal_level", WIFSTOPPED(status), 1);
            EXPECT_EQ("fdsan_test_fatal_level", WSTOPSIG(status), SIGSTOP);
            kill(pid, SIGCONT);
            break;
        }
    }
    return;
}

bool CreateFile()
{
    // 创建一个临时文件
    // 指定文件路径
    const char *file_path = "/data/local/tmp/test.txt";

    // 创建文件并打开文件描述符
    int fd = open(file_path, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return false;
    }

    // 关闭文件描述符
    close(fd);
    return true;
}


void fdsan_test_internal_fopen_succeed()
{
    struct sigaction sigabrt = {
        .sa_handler = signal_handler_abort,
    };
    sigaction(SIGABRT, &sigabrt, nullptr);
    bool res = CreateFile();
    if (!res) {
        t_error("fdsan_test_internal_fopen create file failed");
        return;
    }
    int status;
    int pid = fork();
    switch (pid) {
        case -1: {
            t_error("fork failed: %d\n", __LINE__);
            break;
        }
        case 0: {
            fdsan_set_error_level(FDSAN_ERROR_LEVEL_FATAL);
            unsigned char str[1024];
            FILE f;
            FILE *ptr = __fopen_rb_ca(file_path, &f, str, sizeof(str));
            EXPECT_NE("fdsan_test_internal_fopen_succeed open file failed", ptr, NULL);
            if (ptr) {
                __fclose_ca(ptr);
            }
            exit(0);
        }
        default: {
            waitpid(pid, &status, WUNTRACED);
            EXPECT_EQ("fdsan_test_fork_subprocess_disabled WIFEXITED", WIFEXITED(status), 1);
            EXPECT_EQ("fdsan_test_fork_subprocess_disabled WIFSTOPPED", WIFSTOPPED(status), 0);
            // 子进程应该能够匹配到对应的tag，不应该通过信号方式退出，这里检测子进程不会收到abort信号
            EXPECT_EQ("fdsan_test_fork_subprocess_disabled WSTOPSIG", WSTOPSIG(status), 0);
            kill(pid, SIGCONT);
            break;
        }
    }
    return;
}

int main()
{
    fdsan_test_get_tag_value();
    fdsan_test_overflow();
    fdsan_test_vfork();
    fdsan_test_fatal_level();
    fdsan_test_fork_subprocess_disabled();
    fdsan_test_internal_fopen_succeed();
    return t_status;
}
