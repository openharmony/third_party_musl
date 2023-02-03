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
    EXPECT_FALSE("sigchain_rm_special_handler_008", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_008", false);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    if (signo == SIGHUP) {
        EXPECT_EQ("sigchain_rm_special_handler_008", signo, SIGHUP);
    } else {
        EXPECT_EQ("sigchain_rm_special_handler_008", signo, SIGSEGV);
    }
}

/**
 * @tc.name      : sigchain_rm_special_handler_008
 * @tc.desc      : Add the special handler for the different signals that is registered with
 *                 the kernel (Using signal interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_008()
{
    signal(SIGSEGV, signal_handler);
    signal(SIGHUP, signal_handler);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    struct signal_chain_action sigsegv1 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sigsegv1);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGSEGV, SIGHUP};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_008", signo, SIGCHIAN_TEST_SIGNAL_NUM_2);

    remove_special_signal_handler(SIGHUP, sigchain_special_handler2);
    remove_special_signal_handler(SIGSEGV, sigchain_special_handler1);
}

int main(void)
{
    sigchain_rm_special_handler_008();
    raise(SIGHUP);
    raise(SIGSEGV);
    return t_status;
}