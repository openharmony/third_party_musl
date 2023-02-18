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
    EXPECT_FALSE("sigchain_rm_special_handler_005", true);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_rm_special_handler_005", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_rm_special_handler_005", signo, SIGABRT);
}

/**
 * @tc.name      : sigchain_rm_special_handler_005
 * @tc.desc      : The signal is registered with the kernel(using signal), and add two special handler.
 *                 Call the remove_special_signal_handler to remove the two special handler.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_005()
{
    signal(SIGABRT, signal_handler);

    struct signal_chain_action sigabrt = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigabrt);

    struct signal_chain_action sigabrt1 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigabrt1);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGABRT};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_rm_special_handler_005", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
   
    remove_special_signal_handler(SIGABRT, sigchain_special_handler2);
    remove_special_signal_handler(SIGABRT, sigchain_special_handler1);
}

int main(void)
{
    sigchain_rm_special_handler_005();
    raise(SIGABRT);
    EXPECT_EQ("sigchain_rm_special_handler_005", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    return t_status;
}