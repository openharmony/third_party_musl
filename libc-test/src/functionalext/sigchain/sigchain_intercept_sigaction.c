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
#include <errno.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

static int g_count = 0;
#define ZERO (0)
#define VAL_INT (1024)

void wait_until_sigabrt(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    // 设置超时结构，设置超时为 0.5 秒
    struct timespec timeout;
    timeout.tv_sec = 0;          // 秒部分为 0
    timeout.tv_nsec = 500000000; // 纳秒部分为 500000000，即 0.5 秒
    (void)sigtimedwait(&set, NULL, &timeout);
}

static void sigaction_abort(int signo, siginfo_t *info, void *context)
{
    EXPECT_EQ("sigchain_intercept_sigaction_000 check sival_int failed", info->si_value.sival_int, VAL_INT);
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_001", signo, SIGABRT);
}

/**
 * @brief the SIGABRT handler
 */
static void signal_handler_sigabrt(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_002", signo, SIGABRT);
}

/**
 * @brief the special handler
 */
static void signal_handler1(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_001", signo, SIGHUP);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_001
 * @tc.desc      : The signal is not registered with the special handler, test the influence of sigchain
 *                 on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_001()
{
    struct sigaction siga1 = {
        .sa_handler = signal_handler1,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGHUP, &siga1, NULL);

    raise(SIGHUP);
    EXPECT_EQ("sigchain_intercept_sigaction_001", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler2(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_002", signo, SIGUSR2);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler2(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_002", signo, SIGUSR2);
}


/**
 * @tc.name      : sigchain_intercept_sigaction_002
 * @tc.desc      : The signals is registered with the special handler, test the influence of sigchain on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_002()
{
    g_count = 0;
    struct signal_chain_action sigusr2 = {
        .sca_sigaction = sigchain_special_handler2,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGUSR2, &sigusr2);

    struct sigaction siga2 = {
        .sa_handler = signal_handler2,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGUSR2, &siga2, NULL);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGUSR2};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_002", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    raise(SIGUSR2);
    EXPECT_EQ("sigchain_intercept_sigaction_002", g_count, SIGCHIAN_TEST_SIGNAL_NUM_2);
}

/**
 * @brief the special handler
 */
static bool sigchain_special_handler3(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGURG);
    return false;
}

/**
 * @brief the signal handler
 */
static void signal_handler3(int signo)
{
    g_count++;
    EXPECT_EQ("sigchain_intercept_sigaction_003", signo, SIGURG);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_003
 * @tc.desc      : the signal is registered with the special handler, and remove the special handler. Test
 *                 the influence of sigchain on sigaction.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_003()
{
    g_count = 0;
    struct signal_chain_action sigurg = {
        .sca_sigaction = sigchain_special_handler3,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGURG, &sigurg);

    struct sigaction siga2 = {
        .sa_handler = signal_handler3,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGURG, &siga2, NULL);

    if (get_sigchain_mask_enable()) {
        sigset_t set = {0};
        int signo[SIGCHIAN_TEST_SIGNAL_NUM_1] = {SIGURG};
        SIGCHAIN_TEST_SET_MASK(set, "sigchain_intercept_sigaction_003", signo, SIGCHIAN_TEST_SIGNAL_NUM_1);
    }

    remove_special_signal_handler(SIGURG, sigchain_special_handler3);
    raise(SIGURG);
    EXPECT_EQ("sigchain_intercept_sigaction_003", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

/**
 * @tc.name      : sigchain_intercept_sigaction_sigabort
 * @tc.desc      : Test whether the user manually sets sa_sigaction for the SIGABRT signal to execute normally.
 *                 Test whether the user manually sets sa_handler for the SIGABRT signal to execute normally.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigaction_sigabort(void)
{
    // 解除signal6的信号屏蔽
    sigset_t remove_signal6_mask;
    sigemptyset(&remove_signal6_mask);
    sigaddset(&remove_signal6_mask, SIGABRT);
    if (sigprocmask(SIG_UNBLOCK, &remove_signal6_mask, NULL) == -1) {
        t_error("sigprocmask remove signal 6 failed errno=%d\n", errno);
    }
    g_count = 0;
    struct sigaction action = {
        .sa_sigaction = sigaction_abort,
        .sa_flags = SA_SIGINFO,
    };
    int result = sigaction(SIGABRT, &action, NULL);
    if (result != ZERO) {
        t_error("sigchain_intercept_sigaction_sigabort sigaction 1 failed, errno=%d\n", errno);
    }
    // 1. 创建 sigval 结构体并设置附加数据，校验sa_sigaction场景是否有效，并校验参数是否已经带上
    union sigval value;
    value.sival_int = VAL_INT;
    result = sigqueue(getpid(), SIGABRT, value);
    if (result != ZERO) {
        t_error("sigchain_intercept_sigaction_sigabort sigqueue failed errno=%d\n", errno);
        return;
    }
    wait_until_sigabrt();
    EXPECT_EQ("sigchain_intercept_sigaction_sigabort", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);

    // 2. 清理状态
    g_count = 0;
    memset(&action, 0, sizeof(action));
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    result = sigaction(SIGABRT, &action, NULL);
    if (result != ZERO) {
        t_error("sigchain_intercept_sigaction_sigabort sigaction 2 failed, errno=%d\n", errno);
    }
    // 3. 使用sa_handler的方式处理sigabrt，并判断是否走到了信号处理函数
    action.sa_handler = signal_handler_sigabrt;

    action.sa_flags = SA_RESTART;
    result = sigaction(SIGABRT, &action, NULL);
    if (result != ZERO) {
        t_error("sigchain_intercept_sigaction_sigabort sigaction 3 failed, errno=%d\n", errno);
    }
    (void)raise(SIGABRT);
    wait_until_sigabrt();
    EXPECT_EQ("sigchain_intercept_sigaction_sigabort", g_count, SIGCHIAN_TEST_SIGNAL_NUM_1);
}

int main(void)
{
    sigchain_intercept_sigaction_001();
    sigchain_intercept_sigaction_002();
    sigchain_intercept_sigaction_003();
    sigchain_intercept_sigaction_sigabort();
    return t_status;
}