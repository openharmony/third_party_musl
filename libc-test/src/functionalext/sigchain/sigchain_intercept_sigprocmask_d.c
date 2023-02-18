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

static int g_count = 0;
/**
 * @brief the special handler
 */
static bool sigchain_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    return false;
}


/**
 * @brief the signal handler
 */
static void signal_handler(int signo)
{
    g_count++;
}


/**
 * @tc.name      : sigchain_intercept_sigprocmask_004
 * @tc.desc      : The signals are registered with the special handler.Test the multiple threads call
 *                 sigprocmask and add_special_signal_handler.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_004(int signo)
{
    struct sigaction siga = {
        .sa_handler = signal_handler,
    };
    sigaction(signo, &siga, NULL);

    struct signal_chain_action sig_ca = {
        .sca_sigaction = sigchain_special_handler,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(signo, &sig_ca);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signal[SIGCHIAN_TEST_SIGNAL_NUM_1] = {signo};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigprocmask_004", signal, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }
}

void thread_func1(void *data)
{
    sigchain_intercept_sigprocmask_004(SIGHUP);
}

void thread_func2(void *data)
{
    sigchain_intercept_sigprocmask_004(SIGSEGV);
}

int main(void)
{
    thrd_t t1, t2;
    thrd_create(&t1, (thrd_start_t)thread_func1, NULL);
    thrd_create(&t2, (thrd_start_t)thread_func2, NULL);
    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    raise(SIGHUP);
    raise(SIGSEGV);
    EXPECT_EQ("sigchain_intercept_sigprocmask_004", g_count, SIGCHIAN_TEST_SIGNAL_NUM_4);
    return t_status;
}