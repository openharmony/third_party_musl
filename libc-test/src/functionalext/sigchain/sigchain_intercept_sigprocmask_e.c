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
    EXPECT_EQ("sigchain_intercept_sigprocmask_005", signo, SIGHUP);
    if (g_count == 0) {
        sigset_t set = {0};
        int signal[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGHUP, SIGSEGV};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_001", signal, SIGCHIAN_TEST_SIGNAL_NUM_2);
        raise(SIGSEGV);
        g_count++;
    }
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_intercept_sigprocmask_005", signo, SIGSEGV);
    EXPECT_EQ("sigchain_intercept_sigprocmask_005", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
    return false;
}


/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    g_count++;
    EXPECT_TRUE("sigchain_intercept_sigprocmask_005", true);
}


/**
 * @tc.name      : sigchain_intercept_sigprocmask_005
 * @tc.desc      : The signals are registered with the special handler, and mask and rasie the signal
 *                 at the special handler. Test the influence of sigchain on sigprocmask.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_005()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler,
    };
    sigaction(SIGSEGV, &siga1, NULL);

    struct sigaction siga2 = {
        .sa_handler = signal_handler,
    };
    sigaction(SIGHUP, &siga2, NULL);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_2] = {SIGHUP, SIGSEGV};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_005", signo, SIGCHIAN_TEST_SIGNAL_NUM_2);
}

int main(void)
{
    sigchain_intercept_sigprocmask_005();
    raise(SIGHUP);
    if (get_sigchain_mask_enable()) {
        EXPECT_EQ("sigchain_intercept_sigprocmask_005", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
    } else {
        EXPECT_EQ("sigchain_intercept_sigprocmask_005", g_count, 0);
    }
    return t_status;
}