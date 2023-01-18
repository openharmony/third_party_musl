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

#include <signal.h>
#include <wchar.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"

/**
 * @brief the signal handler
 */
static void signal_handler1(int signo)
{
    EXPECT_EQ("sigchain_intercept_signal_001", signo, SIGHUP);
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    EXPECT_EQ("sigchain_intercept_signal_001", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_add_special_handler_025
 * @tc.desc      : Test the influence of sigchain on signal, the signals are not registered with
 *                 the special handler
 * @tc.level     : Level 0
 */
static void sigchain_intercept_signal_001()
{
    signal(SIGHUP, signal_handler1);
    signal(SIGSEGV, signal_handler2);
}

int main(void)
{
    sigchain_intercept_signal_001();
    raise(SIGHUP);
    raise(SIGSEGV);
    return t_status;
}