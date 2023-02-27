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
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigprocmask_001", signo, SIGSEGV);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigprocmask_001", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_intercept_sigprocmask_001
 * @tc.desc      : The signals are registered with the special handler, test the influence of sigchain
 *                 on sigprocmask.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_001()
{
    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
    };
    sigaction(SIGSEGV, &siga2, NULL);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGSEGV};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_001", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    raise(SIGSEGV);
    if (get_sigchain_mask_enable()) {
        EXPECT_EQ("sigchain_intercept_sigprocmask_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
    } else {
        EXPECT_EQ("sigchain_intercept_sigprocmask_001", g_count, 0);
    }
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_FALSE("sigchain_intercept_sigprocmask_002", true);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler3(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigprocmask_002", signo, SIGSYS);
}

/**
 * @tc.name      : sigchain_intercept_sigprocmask_002
 * @tc.desc      : The signals are registered with the special handler, and remove the special handler.
 *                 Test the influence of sigchain on sigprocmask.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_002()
{
    struct sigaction siga2 = {
        .sa_handler = signal_handler3,
    };
    sigaction(SIGSYS, &siga2, NULL);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSYS, &sigsegv);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGSYS};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_002", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);

    remove_special_signal_handler(SIGSYS, sigchain_special_handler3);

    raise(SIGSYS);
    if (get_sigchain_mask_enable()) {
        EXPECT_EQ("sigchain_intercept_sigprocmask_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_3);
    } else {
        EXPECT_EQ("sigchain_intercept_sigprocmask_002", g_count, 0);
    }  
}


/**
 * @brief the signal handler
 */
static void signal_handler1(int signo)
{
    g_count++;
    EXPECT_FALSE("sigchain_intercept_sigprocmask_001", true);
}

/**
 * @tc.name      : sigchain_intercept_sigprocmask_003
 * @tc.desc      : The signals are not registered with the special handler, test the influence of sigchain
 *                 on sigprocmask.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_003()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
    };
    sigaction(SIGHUP, &siga1, NULL);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGHUP};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_003", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    raise(SIGHUP);
    if (get_sigchain_mask_enable()) {
        EXPECT_EQ("sigchain_intercept_sigprocmask_003", g_count, SIGCHIAN_TEST_SIGNAL_NUM_3);
    } else {
        EXPECT_EQ("sigchain_intercept_sigprocmask_003", g_count, 0);
    }
}

/**
 * @tc.name      : sigchain_intercept_sigprocmask_004
 * @tc.desc      : The new set is null, call sigprocmask.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_004()
{
    int result = sigprocmask(SIG_BLOCK, NULL, NULL);
    if (result != 0) {                                    
        EXPECT_FALSE("sigchain_intercept_sigprocmask_004", true);
    }   
}
int main(void)
{
    sigchain_intercept_sigprocmask_001();
    sigchain_intercept_sigprocmask_002();
    sigchain_intercept_sigprocmask_003();
    sigchain_intercept_sigprocmask_004();
    return t_status;
}