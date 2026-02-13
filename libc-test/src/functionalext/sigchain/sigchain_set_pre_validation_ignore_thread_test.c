/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sigchain.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

static int g_special_handler_count = 0;
static pthread_t g_main_thread_id;

/**
 * @brief the special handler for SIGPIPE
 */
static bool special_handler_sigpipe(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_special_handler_count++;
    EXPECT_EQ("sigchain_set_pre_validation_ignore_thread", signo, SIGPIPE);
    return false;
}

/**
 * @brief Thread function to send SIGPIPE to main thread
 */
static void* thread_func(void* arg)
{
    for (int i = 0; i < 50; i++) {
        pthread_kill(g_main_thread_id, SIGPIPE);
        usleep(1000);
    }
    return NULL;
}

/**
 * @tc.name      : sigchain_set_pre_validation_ignore_thread_001
 * @tc.desc      : Test multi-threaded SIGPIPE handling with pre_validation_ignore enabled (1)
 *                 Child process raises SIGPIPE 100 times, new thread sends 50 SIGPIPE to main thread.
 *                 Expected: child process exits normally.
 * @tc.level     : Level 0
 */
static void sigchain_set_pre_validation_ignore_thread_001()
{
    pid_t pid = fork();
    if (pid < 0) {
        EXPECT_EQ("sigchain_set_pre_validation_ignore_thread_001 fork", pid, 0);
        return;
    }

    if (pid == 0) {
        g_special_handler_count = 0;
        g_main_thread_id = pthread_self();

        set_sigchain_pre_validation_ignore(1);

        struct signal_chain_action sigpipe = {
            .sca_sigaction = special_handler_sigpipe,
            .sca_mask = {},
            .sca_flags = 0,
        };
        add_special_signal_handler(SIGPIPE, &sigpipe);

        struct sigaction sigac = {
            .sa_handler = SIG_IGN,
            .sa_flags = SA_SIGINFO,
        };
        sigemptyset(&sigac.sa_mask);
        sigaction(SIGPIPE, &sigac, NULL);

        pthread_t thread;
        if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
            exit(1);
        }

        for (int i = 0; i < 100; i++) {
            raise(SIGPIPE);
            usleep(1000);
        }

        pthread_join(thread, NULL);

        if (g_special_handler_count >= 100) {
            exit(0);
        } else {
            exit(1);
        }
    }

    int status;
    waitpid(pid, &status, 0);

    int exit_code = 0;
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    }

    EXPECT_EQ("sigchain_set_pre_validation_ignore_thread_001", exit_code, 0);
    EXPECT_TRUE("sigchain_set_pre_validation_ignore_thread_001", WIFEXITED(status));

    set_sigchain_pre_validation_ignore(0);
}

int main(void)
{
    sigchain_set_pre_validation_ignore_thread_001();
    return t_status;
}
