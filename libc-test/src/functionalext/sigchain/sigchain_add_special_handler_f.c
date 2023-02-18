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
    EXPECT_EQ("sigchain_add_special_handler_006", signo, SIGABRT);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_006", signo, SIGABRT);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_006", signo, SIGABRT);
}

/**
 * @tc.name      : sigchain_add_special_handler_006
 * @tc.desc      : Add multiple special handlers for a signal that is registered with
 *                 the kernel (Using sigaction interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_006()
{
    struct sigaction sigac = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGABRT, &sigac, NULL);

    struct signal_chain_action sigabrt = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigabrt);

    struct signal_chain_action sigabrt2 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigabrt2);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGABRT};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_add_special_handler_006", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
}

int main(void)
{
    sigchain_add_special_handler_006();
    raise(SIGABRT);
    EXPECT_EQ("sigchain_add_special_handler_006", g_count, SIGCHIAN_TEST_SIGNAL_NUM_3);
    return t_status;
}