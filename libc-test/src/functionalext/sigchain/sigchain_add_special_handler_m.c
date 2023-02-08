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
    EXPECT_FALSE("sigchain_add_special_handler_013", true);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_add_special_handler_013", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    EXPECT_EQ("sigchain_add_special_handler_013", signo, SIGABRT);
    return;
}

/**
 * @tc.name      : sigchain_add_special_handler_013
 * @tc.desc      : Add the special handler with wrong signal
 * @tc.level     : Level 2
 */
static void sigchain_add_special_handler_013()
{
    signal(SIGABRT, signal_handler);

    struct signal_chain_action sig_special_action = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(0, &sig_special_action);

    struct signal_chain_action sig_special_action1 = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_65, &sig_special_action1);
}

int main(void)
{
    sigchain_add_special_handler_013();
    return t_status;
}