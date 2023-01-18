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
    EXPECT_EQ("sigchain_add_special_handler_003", signo, SIGSEGV);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    EXPECT_EQ("sigchain_add_special_handler_003", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_add_special_handler_003
 * @tc.desc      : Add a special handler for a signal that is registered with
 *                 the kernel (Using sigaction interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_003()
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

    sigset_t set = {0};
    int signo[1] = {SIGSEGV};
    SIGCHIAN_TEST_SET_MASK(set, "sigchain_add_special_handler_003", signo, 1);
}

int main(void)
{
    sigchain_add_special_handler_003();
    raise(SIGSEGV);
    return t_status;
}