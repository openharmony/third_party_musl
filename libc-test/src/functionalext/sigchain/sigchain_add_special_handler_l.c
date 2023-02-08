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
#include <signal.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGHUP);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGABRT);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGSEGV);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler4(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGURG);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler5(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGSYS);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler6(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_37);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler7(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_43);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler8(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_50);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler9(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_56);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler10(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_64);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    if (signo == SIGHUP) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGHUP);
    } else if (signo == SIGABRT) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGABRT);
    } else if (signo == SIGSEGV) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGSEGV);
    } else if (signo == SIGURG) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGURG);
    } else if (signo == SIGSYS) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGSYS);
    } else if (signo == SIGCHAIN_SIGNAL_37) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_37);
    } else if (signo == SIGCHAIN_SIGNAL_43) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_43);
    } else if (signo == SIGCHAIN_SIGNAL_50) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_50);
    } else if (signo ==SIGCHAIN_SIGNAL_56) {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_56);
    } else {
        EXPECT_EQ("sigchain_add_special_handler_012", signo, SIGCHAIN_SIGNAL_64);
    }
}

/**
 * @tc.name      : sigchain_add_special_handler_012
 * @tc.desc      : Add multiple special handlers for the different signals that is registered with
 *                 the kernel (Using sigaction interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_add_special_handler_012()
{
    struct sigaction sigaction0 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGHUP, &sigaction0, NULL);

    struct sigaction sigaction1 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGABRT, &sigaction1, NULL);

    struct sigaction sigaction2 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGSEGV, &sigaction2, NULL);

    struct sigaction sigaction3 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGURG, &sigaction3, NULL);

    struct sigaction sigaction4 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGSYS, &sigaction4, NULL);

    struct sigaction sigaction5 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_37, &sigaction5, NULL);

    struct sigaction sigaction6 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_43, &sigaction6, NULL);

    struct sigaction sigaction7 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_50, &sigaction7, NULL);

    struct sigaction sigaction8 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_56, &sigaction8, NULL);

    struct sigaction sigaction9 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(SIGCHAIN_SIGNAL_64, &sigaction9, NULL);

    struct signal_chain_action sighup = {
        .sca_sigaction = sigchain_special_handler1,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGHUP, &sighup);

    struct signal_chain_action sigabrt = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGABRT, &sigabrt);

    struct signal_chain_action sigsegv = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGSEGV, &sigsegv);

    struct signal_chain_action sigurg = {
        .sca_sigaction = sigchain_special_handler4,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGURG, &sigurg);

    struct signal_chain_action sigsys = {
        .sca_sigaction = sigchain_special_handler5,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGSYS, &sigsys);

    struct signal_chain_action sig37 = {
        .sca_sigaction = sigchain_special_handler6,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_37, &sig37);

    struct signal_chain_action sig43 = {
        .sca_sigaction = sigchain_special_handler7,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_43, &sig43);

    struct signal_chain_action sig50 = {
        .sca_sigaction = sigchain_special_handler8,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_50, &sig50);

    struct signal_chain_action sig56 = {
        .sca_sigaction = sigchain_special_handler9,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_56, &sig56);

    struct signal_chain_action sig64 = {
        .sca_sigaction = sigchain_special_handler10,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGCHAIN_SIGNAL_64, &sig64);

    sigset_t set = {0};
    int signo[SIGCHIAN_TEST_SIGNAL_NUM_10] = {SIGHUP, SIGABRT, SIGSEGV, SIGURG, SIGSYS,
                                              SIGCHAIN_SIGNAL_37,
                                              SIGCHAIN_SIGNAL_43,
                                              SIGCHAIN_SIGNAL_50,
                                              SIGCHAIN_SIGNAL_56,
                                              SIGCHAIN_SIGNAL_64};
    SIGCHAIN_TEST_SET_MASK(set, "sigchain_add_special_handler_012", signo, SIGCHIAN_TEST_SIGNAL_NUM_10);
}

int main(void)
{
    sigchain_add_special_handler_012();
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