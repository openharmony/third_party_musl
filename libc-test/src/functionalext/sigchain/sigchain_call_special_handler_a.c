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

#define SIGCHAIN_CALL_SPECIAL_HANDLER_TAG 1
static int g_count = 0;

/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_call_special_handler_001", signo, SIGSEGV);
    g_count++;
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    EXPECT_EQ("sigchain_call_special_handler_001", signo, SIGSEGV);
    EXPECT_EQ("sigchain_call_special_handler_001", g_count, SIGCHAIN_CALL_SPECIAL_HANDLER_TAG);
    return;
}

/**
 * @tc.name      : sigchain_call_special_handler_001
 * @tc.desc      : Add a special handler for a signal that is registered with
 *                 the kernel (Using signal interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_call_special_handler_001()
{
    signal(SIGSEGV, signal_handler);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGSEGV};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_call_special_handler_001", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

int main(void)
{
    sigchain_call_special_handler_001();
    raise(SIGSEGV);
    return t_status;
}