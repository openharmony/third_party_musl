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
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_003", true);
    return true;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_003", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_rm_special_handler_003
 * @tc.desc      : The signal is registered with the kernel(using sigaction), and add the special handler.
 *                 Call the remove_special_signal_handler
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_003()
{
    struct sigaction sigsegv1 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGSEGV, &sigsegv1, NULL);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGSEGV};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_003", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    remove_special_signal_handler(SIGSEGV, sigchain_special_handler);
}

int main(void)
{
    sigchain_rm_special_handler_003();
    raise(SIGSEGV);
    EXPECT_EQ("sigchain_rm_special_handler_003", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    return t_status;
}