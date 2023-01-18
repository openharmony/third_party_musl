/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <wchar.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGHUP);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGSEGV);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler1(int signo)
{
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGHUP);
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_003
 * @tc.desc      : Test the influence of sigchain on sigaction. the signals are registered with
 *                 the special handler, and remove the special handler.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_003()
{
    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sigsegv);

    struct signal_chain_action sigsegv1 = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv1);

    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
    };
    sigaction(SIGHUP, &siga1, NULL);

    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
    };
    sigaction(SIGSEGV, &siga2, NULL);

    sigset_t set = {0};
    int signo[2] = {SIGSEGV, SIGHUP};
    SIGCHIAN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_002", signo, 2);

    remove_special_signal_handler(SIGHUP, sigchain_special_handler);
    remove_special_signal_handler(SIGSEGV, sigchain_special_handler1);
}

int main(void)
{
    sigchain_intercept_sigaction_003();
    raise(SIGHUP);
    raise(SIGSEGV);
    return t_status;
}