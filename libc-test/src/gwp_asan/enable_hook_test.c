/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

/**
 * Currently,musl malloc/free hooks may introduce risks to GWP_ASan's memory management. This TEST aim to
 * enhance libc-test by adding GWP_ASan test case for proactive monitoring.
 */

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "gwp_asan_test.h"
#include "test.h"

void handler(void)
{
    t_error("Under the condition of memory hooks, GWP_ASan is encountering errors during usage.\n");
    _exit(1);
}

void install_sigv_handler(void)
{
    struct sigaction sigv = {
        .sa_handler = handler,
        .sa_flags = 0,
    };
    sigaction(SIGSEGV, &sigv, NULL);
}

void enable_hook_test(void)
{
    config_gwp_asan_environment(true);
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    install_sigv_handler();

    void *ptr = malloc(20);
    if (!ptr) {
        t_error("malloc failed.");
        return;
    }
    if (!libc_gwp_asan_ptr_is_mine(ptr)) {
        t_error("Memory is not allocated by gwp_asan.\n");
        return;
    }
    // send signal 36 to self process and enable memory hook.
    send_sig36();

    free(ptr);
}

int main(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        enable_hook_test();
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            t_error("gwp_asan_enable_hook_test waitpid failed.\n");
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("gwp_asan_enable_hook_test failed.\n");
        }

        cancel_gwp_asan_environment(true);
    }

    return t_status;
}
