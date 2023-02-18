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
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGHUP);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGABRT);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGSEGV);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler4(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGURG);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler5(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGSYS);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler6(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_37);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler7(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_43);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler8(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_50);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler9(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_56);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler10(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_64);
    return false;
}

/**
 * @tc.name      : sigchain_add_special_handler_010
 * @tc.desc      : Multiple different signals((Coverage signal range)) are not registered with the kernel,
 *                 call add_special_signal_handler
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_010()
{
    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    struct signal_chain_action sigabrt = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigabrt);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    struct signal_chain_action sigurg = {
        .sca_sigaction = sigchain_special_handler4,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGURG, &sigurg);

    struct signal_chain_action sigsys = {
        .sca_sigaction = sigchain_special_handler5,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSYS, &sigsys);

    struct signal_chain_action sig37 = {
        .sca_sigaction = sigchain_special_handler6,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_37, &sig37);

    struct signal_chain_action sig43 = {
        .sca_sigaction = sigchain_special_handler7,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_43, &sig43);

    struct signal_chain_action sig50 = {
        .sca_sigaction = sigchain_special_handler8,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_50, &sig50);

    struct signal_chain_action sig56 = {
        .sca_sigaction = sigchain_special_handler9,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_56, &sig56);

    struct signal_chain_action sig64 = {
        .sca_sigaction = sigchain_special_handler10,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_64, &sig64);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_10] = {SIGHUP, SIGABRT,
                                                SIGSEGV, SIGURG, SIGSYS,
                                                SIGCHAIN_SIGNAL_37,
                                                SIGCHAIN_SIGNAL_43,
                                                SIGCHAIN_SIGNAL_50,
                                                SIGCHAIN_SIGNAL_56,
                                                SIGCHAIN_SIGNAL_64};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_add_special_handler_010", signo, SIGCHIAN_TEST_SIGNAL_NUM_10);
    }
}

int main(void)
{
    sigchain_add_special_handler_010();
    raise(SIGHUP);
    raise(SIGABRT);
    raise(SIGSEGV);
    raise(SIGURG);
    raise(SIGSYS);
    raise(SIGCHAIN_SIGNAL_37);
    raise(SIGCHAIN_SIGNAL_43);
    raise(SIGCHAIN_SIGNAL_50);
    raise(SIGCHAIN_SIGNAL_56);
    raise(SIGCHAIN_SIGNAL_64);
    EXPECT_EQ("sigchain_add_special_handler_010", g_count, SIGCHIAN_TEST_SIGNAL_NUM_10);
    return t_status;
}