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
#include "sigchain_util.h"

static int g_count = 0;
/**
 * @brief the signal handler
 */
static void signal_handler1(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_signal_001", signo, SIGHUP);
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_signal_001", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_add_special_handler_025
 * @tc.desc      : The signals are not registered with the special handler, test the influence of sigchain on signal
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
    EXPECT_EQ("sigchain_intercept_sigaction_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
    return t_status;
}