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

/**
 * @brief the special handler
 */
static bool sigchain_special_handler1(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler4(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler5(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler6(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler7(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler8(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler9(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler10(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    EXPECT_FALSE("sigchain_rm_special_handler_012", false);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_sigaction(int signo)
{
    if (signo == SIGHUP) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, SIGHUP);
    } else if (signo == SIGABRT) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, SIGABRT);
    } else if (signo == SIGSEGV) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, SIGSEGV);
    } else if (signo == SIGURG) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, SIGURG);
    } else if (signo == SIGSYS) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, SIGSYS);
    } else if (signo == 37) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, 37);
    } else if (signo == 43) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, 43);
    } else if (signo == 50) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, 50);
    } else if (signo == 56) {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, 56);
    } else {
        EXPECT_EQ("sigchain_rm_special_handler_012", signo, 64);
    }
}

/**
 * @tc.name      : sigchain_rm_special_handler_012
 * @tc.desc      : Remove multiple special handlers for the different signals that is registered with
 *                 the kernel (Using sigaction interface) in sigchain.
 * @tc.level     : Level 0
 */
static void sigchain_rm_special_handler_012()
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
    sigaction(37, &sigaction5, NULL);

    struct sigaction sigaction6 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(43, &sigaction6, NULL);

    struct sigaction sigaction7 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(50, &sigaction7, NULL);

    struct sigaction sigaction8 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(56, &sigaction8, NULL);

    struct sigaction sigaction9 = {
        .sa_handler = signal_sigaction,
    };
    sigaction(64, &sigaction9, NULL);

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
    add_special_signal_handler(37, &sigsegv5);

    struct signal_chain_action sigsegv6 = {
        .sca_sigaction = sigchain_special_handler7,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(43, &sigsegv6);

    struct signal_chain_action sigsegv7 = {
        .sca_sigaction = sigchain_special_handler8,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(50, &sigsegv7);

    struct signal_chain_action sigsegv8 = {
        .sca_sigaction = sigchain_special_handler9,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(56, &sigsegv8);

    struct signal_chain_action sigsegv9 = {
        .sca_sigaction = sigchain_special_handler10,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(64, &sigsegv9);

    remove_special_signal_handler(SIGHUP, sigchain_special_handler1);
    remove_special_signal_handler(SIGABRT, sigchain_special_handler2);
    remove_special_signal_handler(SIGSEGV, sigchain_special_handler3);
    remove_special_signal_handler(SIGURG, sigchain_special_handler4);
    remove_special_signal_handler(SIGSYS, sigchain_special_handler5);
    remove_special_signal_handler(37, sigchain_special_handler6);
    remove_special_signal_handler(43, sigchain_special_handler7);
    remove_special_signal_handler(50, sigchain_special_handler8);
    remove_special_signal_handler(56, sigchain_special_handler9);
    remove_special_signal_handler(64, sigchain_special_handler10);
}

int main(void)
{
    sigchain_rm_special_handler_012();
    raise(SIGHUP);
    raise(SIGABRT);
    raise(SIGSEGV);
    raise(SIGURG);
    raise(SIGSYS);
    raise(37);
    raise(43);
    raise(50);
    raise(56);
    raise(64);
    return t_status;
}