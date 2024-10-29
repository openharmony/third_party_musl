/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "gwp_asan_test.h"
#include "test.h"

void buffer_overflow_handler()
{
    find_and_check_file(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "Buffer Overflow");
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    cancel_gwp_asan_environment(true);
    _exit(0);
}

static void install_sigv_handler()
{
    struct sigaction sigv = {
        .sa_handler = buffer_overflow_handler,
        .sa_flags = 0,
    };
    sigaction(SIGSEGV, &sigv, NULL);
}

void buffer_overflow_test()
{
    config_gwp_asan_environment(true);
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    install_sigv_handler();

    char *ptr = (char *)malloc(sysconf(_SC_PAGE_SIZE));
    if (!ptr) {
        t_error("malloc failed.");
        return;
    }
    if (!libc_gwp_asan_ptr_is_mine(ptr)) {
        t_error("Memory is not allocated by gwp_asan.");
        return;
    }
    char c = *(ptr + sysconf(_SC_PAGE_SIZE) + 1);
    printf("c:%c", c);
}

/*
 *
 *|--guard area--|--malloc area--|--guard area--|
 *                                ^
 */
int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        buffer_overflow_test();
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            t_error("gwp_asan_buffer_overflow_test waitpid failed.");
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("gwp_asan_buffer_overflow_test failed.");
        }

        cancel_gwp_asan_environment(true);
    }

    return t_status;
}
