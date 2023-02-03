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
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGHUP);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGABRT);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGSEGV);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler4(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGURG);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler5(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGSYS);
    return true;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler6(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_37);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler7(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_43);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler8(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_50);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler9(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_56);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler10(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_010", signo, SIGCHAIN_SIGNAL_64);
    return false;
}

/**
 * @tc.name      : sigchain_add_special_handler_010
 * @tc.desc      : Add multiple special handlers for the different signals that is not registered with
 *                 the kernel in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_010()
{
    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGHUP, &sigsegv);

    struct signal_chain_action sigsegv1 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGABRT, &sigsegv1);

    struct signal_chain_action sigsegv2 = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv2);

    struct signal_chain_action sigsegv3 = {
        .sca_sigaction = sigchain_special_handler4,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGURG, &sigsegv3);

    struct signal_chain_action sigsegv4 = {
        .sca_sigaction = sigchain_special_handler5,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGSYS, &sigsegv4);

    struct signal_chain_action sigsegv5 = {
        .sca_sigaction = sigchain_special_handler6,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_37, &sigsegv5);

    struct signal_chain_action sigsegv6 = {
        .sca_sigaction = sigchain_special_handler7,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_43, &sigsegv6);

    struct signal_chain_action sigsegv7 = {
        .sca_sigaction = sigchain_special_handler8,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_50, &sigsegv7);

    struct signal_chain_action sigsegv8 = {
        .sca_sigaction = sigchain_special_handler9,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_56, &sigsegv8);

    struct signal_chain_action sigsegv9 = {
        .sca_sigaction = sigchain_special_handler10,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_64, &sigsegv9);

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
    return t_status;
}