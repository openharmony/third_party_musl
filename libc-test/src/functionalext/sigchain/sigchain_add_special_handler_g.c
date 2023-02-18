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

static int g_count = 0;
/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_007", signo, SIGSEGV);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_007", signo, SIGHUP);
    return true;
}

/**
 * @tc.name      : sigchain_add_special_handler_007
 * @tc.desc      : Multiple signal are not registered with the kernel, call add_special_signal_handler
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_007()
{
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

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGSEGV, SIGHUP};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_add_special_handler_007", signo, SIGCHIAN_TEST_SIGNAL_NUM_2);
    }
}

int main(void)
{
    sigchain_add_special_handler_007();
    raise(SIGHUP);
    EXPECT_EQ("sigchain_add_special_handler_007", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    raise(SIGSEGV);
    EXPECT_EQ("sigchain_add_special_handler_007", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
    return t_status;
}