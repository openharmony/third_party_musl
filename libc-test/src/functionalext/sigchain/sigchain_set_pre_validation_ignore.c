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
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

static int g_special_handler_count = 0;

/**
 * @brief the special handler for SIGUSR2
 */
static bool special_handler_sigusr2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_special_handler_count++;
    EXPECT_EQ("sigchain_set_pre_validation_ignore", signo, SIGUSR2);
    return false;
}

/**
 * @brief the special handler for SIGPIPE
 */
static bool special_handler_sigpipe(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_special_handler_count++;
    EXPECT_EQ("sigchain_set_pre_validation_ignore", signo, SIGPIPE);
    return false;
}

/**
 * @tc.name      : sigchain_set_pre_validation_ignore_001
 * @tc.desc      : Test that special handler is NOT called when pre_validation_ignore is enabled (1)
 *                 and signal handler is set to SIG_IGN.
 * @tc.level     : Level 0
 */
static void sigchain_set_pre_validation_ignore_001()
{
    pid_t pid = fork();
    if (pid < 0) {
        EXPECT_EQ("sigchain_set_pre_validation_ignore_001 fork", pid, 0);
        return;
    }

    if (pid == 0) {
        g_special_handler_count = 0;

        set_sigchain_pre_validation_ignore(0);

        struct signal_chain_action sigusr2 = {
            .sca_sigaction = special_handler_sigusr2,
            .sca_mask = {},
            .sca_flags = 0,
        };
        add_special_signal_handler(SIGUSR2, &sigusr2);

        struct sigaction sigac = {
            .sa_handler = SIG_IGN,
            .sa_flags = SA_SIGINFO,
        };
        sigemptyset(&sigac.sa_mask);
        sigaction(SIGUSR2, &sigac, NULL);

        raise(SIGUSR2);

        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);

    EXPECT_TRUE("sigchain_set_pre_validation_ignore_001", WIFSIGNALED(status));
}

/**
 * @tc.name      : sigchain_set_pre_validation_ignore_002
 * @tc.desc      : Test that when SA_SIGINFO flag is set, pre_validation_ignore does NOT
 *                 affect the behavior even if sa_handler is incorrectly set to SIG_IGN.
 *                 Ensures no crash occurs with incorrect handler configuration.
 * @tc.level     : Level 0
 */
static void sigchain_set_pre_validation_ignore_002()
{
    pid_t pid = fork();
    if (pid < 0) {
        EXPECT_EQ("sigchain_set_pre_validation_ignore_002 fork", pid, 0);
        return;
    }

    if (pid == 0) {
        g_special_handler_count = 0;

        set_sigchain_pre_validation_ignore(1);

        struct signal_chain_action sigusr2 = {
            .sca_sigaction = special_handler_sigusr2,
            .sca_mask = {},
            .sca_flags = 0,
        };
        add_special_signal_handler(SIGUSR2, &sigusr2);

        struct sigaction sigac = {
            .sa_handler = SIG_IGN,
            .sa_flags = SA_SIGINFO,
        };
        sigemptyset(&sigac.sa_mask);
        sigaction(SIGUSR2, &sigac, NULL);

        raise(SIGUSR2);

        if (g_special_handler_count == SIGCHIAN_TEST_SIGNAL_NUM_1) {
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

    EXPECT_EQ("sigchain_set_pre_validation_ignore_002", exit_code, 0);

    set_sigchain_pre_validation_ignore(0);
}

/**
 * @tc.name      : sigchain_set_pre_validation_ignore_003
 * @tc.desc      : Test that special handler is NOT called when pre_validation_ignore is enabled (1)
 *                 and signal handler is set to SIG_IGN.
 * @tc.level     : Level 0
 */
static void sigchain_set_pre_validation_ignore_003()
{
    pid_t pid = fork();
    if (pid < 0) {
        EXPECT_EQ("sigchain_set_pre_validation_ignore_003 fork", pid, 0);
        return;
    }

    if (pid == 0) {
        g_special_handler_count = 0;

        set_sigchain_pre_validation_ignore(0);

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

        raise(SIGPIPE);

        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);

    EXPECT_TRUE("sigchain_set_pre_validation_ignore_003", WIFSIGNALED(status));
}

/**
 * @tc.name      : sigchain_set_pre_validation_ignore_004
 * @tc.desc      : Test that when SA_SIGINFO flag is set, pre_validation_ignore does NOT
 *                 affect the behavior even if sa_handler is incorrectly set to SIG_IGN.
 *                 Ensures no crash occurs with incorrect handler configuration.
 * @tc.level     : Level 0
 */
static void sigchain_set_pre_validation_ignore_004()
{
    pid_t pid = fork();
    if (pid < 0) {
        EXPECT_EQ("sigchain_set_pre_validation_ignore_004 fork", pid, 0);
        return;
    }

    if (pid == 0) {
        g_special_handler_count = 0;

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

        raise(SIGPIPE);

        if (g_special_handler_count == SIGCHIAN_TEST_SIGNAL_NUM_1) {
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

    EXPECT_EQ("sigchain_set_pre_validation_ignore_004", exit_code, 0);

    set_sigchain_pre_validation_ignore(0);
}

int main(void)
{
    sigchain_set_pre_validation_ignore_001();
    sigchain_set_pre_validation_ignore_002();
    sigchain_set_pre_validation_ignore_003();
    sigchain_set_pre_validation_ignore_004();
    return t_status;
}
