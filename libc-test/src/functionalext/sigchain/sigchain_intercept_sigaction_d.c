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
 * @brief the special handler
 */
static void signal_handler1(int signo)
{
    EXPECT_EQ("sigchain_intercept_sigaction_004", signo, SIGHUP);
    sigset_t set = {0};
    int signal[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGSEGV, SIGHUP};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_004", signal, SIGCHIAN_TEST_SIGNAL_NUM_2);
    g_count++;
    raise(SIGSEGV);
}

/**
 * @brief the special handler
 */
static void signal_handler2(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_004", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_004
 * @tc.desc      : The signals are not registered with the special handler, and mask and rasie the signal
 *                 at the special handler. Test the influence of sigchain on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_004()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
    };
    sigaction(SIGHUP, &siga1, NULL);

    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
    };
    sigaction(SIGSEGV, &siga2, NULL);
}

int main(void)
{
    sigchain_intercept_sigaction_004();
    raise(SIGHUP);
    EXPECT_EQ("sigchain_intercept_sigaction_004", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
    return t_status;
}