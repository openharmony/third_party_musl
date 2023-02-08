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

/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_001", true);
    return false;
}

/**
 * @tc.name      : sigchain_rm_special_handler_001
 * @tc.desc      : remove a special handler for a signal that is not registered with
 *                 the kernel in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_001()
{
    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sigsegv);

    remove_special_signal_handler(SIGHUP, sigchain_special_handler);
}

int main(void)
{
    sigchain_rm_special_handler_001();
    raise(SIGHUP);
    return t_status;
}