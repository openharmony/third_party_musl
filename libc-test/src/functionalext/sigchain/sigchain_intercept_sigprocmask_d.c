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
#include <threads.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_intercept_sigprocmask_004", signo, SIGHUP);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_intercept_sigprocmask_004", signo, SIGHUP);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler1(int signo)
{
    EXPECT_EQ("sigchain_intercept_sigprocmask_004", signo, SIGHUP);
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    EXPECT_EQ("sigchain_intercept_sigprocmask_004", signo, SIGSEGV);
}

/**
 * @tc.name      : sigchain_intercept_sigprocmask_004
 * @tc.desc      : Test the influence of sigchain on sigprocmask. the signals are registered with
 *                 the special handler, and the case is called by multiple threads
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_004()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
    };
    sigaction(SIGHUP, &siga1, NULL);

    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
    };
    sigaction(SIGSEGV, &siga2, NULL);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sigsegv);

    struct signal_chain_action sigsegv1 = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv1);

    sigset_t set = {0};
    int signo[2] = {SIGHUP, SIGSEGV};
    SIGCHIAN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_004", signo, 2);
}

void thread_func(void *data)
{
    sigchain_intercept_sigprocmask_004();
    raise(SIGHUP);
    raise(SIGSEGV);
}

int main(void)
{
    thrd_t t1, t2, t3, t4;
    thrd_create(&t1, (thrd_start_t)thread_func, NULL);
    thrd_create(&t2, (thrd_start_t)thread_func, NULL);
    thrd_create(&t3, (thrd_start_t)thread_func, NULL);
    thrd_create(&t4, (thrd_start_t)thread_func, NULL);
    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    thrd_join(t3, NULL);
    thrd_join(t4, NULL);
    return t_status;
}