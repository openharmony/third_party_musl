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
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_009", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_009", false);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    if (signo == SIGHUP) {
        EXPECT_EQ("sigchain_rm_special_handler_009", signo, SIGHUP);
    } else {
        EXPECT_EQ("sigchain_rm_special_handler_009", signo, SIGSEGV);
    }
}

/**
 * @tc.name      : sigchain_rm_special_handler_008
 * @tc.desc      : Add the special handler for the different signals that is registered with
 *                 the kernel (Using sigaction interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_009()
{
    struct sigaction sigsegv1 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGSEGV, &sigsegv1, NULL);

    struct sigaction sigsegv2 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGHUP, &sigsegv2, NULL);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    sigset_t set = {0};
    int signo[2] = {SIGSEGV, SIGHUP};
    SIGCHIAN_TEST_SET_MASK(set, "sigchain_rm_special_handler_009", signo, 2);

    remove_special_signal_handler(SIGHUP, sigchain_special_handler2);
    remove_special_signal_handler(SIGSEGV, sigchain_special_handler1);
}

int main(void)
{
    sigchain_rm_special_handler_009();
    raise(SIGHUP);
    raise(SIGSEGV);
    return t_status;
}