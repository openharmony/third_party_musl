/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <algorithm.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include <csignal>
#include <sigchain.h>

#define SIGNAL_CHAIN_SPECIAL_ACTION_MAX 4

extern "C"{
    bool intercept_sigchain(int signo, const struct sigaction *__restrict sa, struct sigaction *__restrict old);
    int sigchain_sigmask(int how, const sigset_t *__restrict set, sigset_t *__restrict old);
    void add_special_handler_at_last(int signo, struct signal_chain_action* sa);
}

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {

class SigchainTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void SigchainTest::TearDownTestCase(void)
{
}

// The special handler
static bool sigaction_special_handler(int signo, siginfo_t *siginfo, void *ucontext_raw)
{
    return false;
}

/**
 *@tc.name: sigchain_sigmask_test_001
 *@tc.desc: Test the add_special_handler_at_last function.
            When the input how - SIG_BLOCK > 2U , the expectation is EINVAL.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_sigmask_test_001, TestSize.Level0)
{
    sigset_t set;
    int res = sigaction_sigmask(10, &set, NULL);
    EXPECT_TRUE(res == EINVAL);
}

/**
 *@tc.name: sigchain_sigmask_test_002
 *@tc.desc: Test the add_special_handler_at_last function. When the input set is NULL, the expectation is EINVAL.
 *@tc.type: FUNC
 */ 
HWTEST_F(SigchainTest, sigchain_sigmask_test_002, TestSize.Level0)
{
    int res = sigaction_sigmask(10, NULL, NULL);
    EXPECT_TRUE(res == EINVAL);
}

/**
 *@tc.name: sigchain_add_special_handler_at_last_test_001
 *@tc.desc: Test the add_special_handler_at_last function. When the input signo is
            out of range (0, _NSIG), the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_add_special_handler_at_last_test_001, TestSize.Level0)
{
    struct signal_chain_action mock_sa = {
        .sca_sigaction = sigaction_special_handler,
        .sca_mask = {};
        .sca_flags = 0;
    };
    EXPECT_DEATH(add_special_handler_at_last(-1, &mock_sa), ".*");
    EXPECT_DEATH(add_special_handler_at_last(_NSIG, &mock_sa), ".*");
}

/**
 *@tc.name: sigchain_add_special_handler_at_last_test_002
 *@tc.desc: Test the add_special_handler_at_last function. When adding more than one
            signal_chain_action to the same signal, the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_add_special_handler_at_last_test_002, TestSize.Level0)
{
    remove_all_special_handler(45);
    struct signal_chain_action mock_sa = {
        .sca_sigaction = sigaction_special_handler,
        .sca_mask = {};
        .sca_flags = 0;
    };
    add_special_handler_at_last(45, &mock_sa);
    EXPECT_DEATH(add_special_handler_at_last(45, &mock_sa), ".*");
    remove_all_special_handler(45);
}

/**
 *@tc.name: sigchain_add_special_signal_handler_test_001
 *@tc.desc: Test the add_special_handler function. When the input signo is
            out of range (0, _NSIG), the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_add_special_signal_handler_test_001, TestSize.Level0)
{
    struct signal_chain_action mock_sa = {
        .sca_sigaction = sigaction_special_handler,
        .sca_mask = {};
        .sca_flags = 0;
    };
    EXPECT_DEATH(add_special_signal_handler(-1, &mock_sa), ".*");
    EXPECT_DEATH(add_special_signal_handler(_NSIG, &mock_sa), ".*");
}

/**
 *@tc.name: sigchain_add_special_signal_handler_test_002
 *@tc.desc: Test the add_special_handler function by add_special_handler.
            When adding more max number of special special actions to a signal,
            the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_add_special_signal_handler_test_002, TestSize.Level0)
{
    remove_all_special_handler(45);
    struct signal_chain_action mock_sa = {
        .sca_sigaction = sigaction_special_handler,
        .sca_mask = {};
        .sca_flags = 0;
    };
    for (int i = 0; i < SIGNAL_CHAIN_SPECIAL_ACTION_MAX; i++) {
        add_special_signal_handler(45, &mock_sa);
    }
    EXPECT_DEATH(add_special_signal_handler(45, &mock_sa), ".*");
    remove_all_special_handler(45);
}

/**
 *@tc.name: sigchain_remove_special_signal_handler_test_001
 *@tc.desc: Test the remove_special_signal_handler function. When the input signo is
            out of range (0, _NSIG), the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_remove_special_signal_handler_test_001, TestSize.Level0)
{
    EXPECT_DEATH(remove_special_signal_handler(-1, sigchain_special_handler), ".*");
    EXPECT_DEATH(remove_special_signal_handler(_NSIG, sigchain_special_handler), ".*");
}

/**
 *@tc.name: sigchain_remove_special_signal_handler_test_002
 *@tc.desc: Test the remove_special_signal_handler function. When removing
            a signal which is not marked(special action list is empty),
            nothing happen.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_remove_special_signal_handler_test_002, TestSize.Level0)
{
    remove_all_special_handler(45);
    remove_special_signal_handler(45, sigchain_special_handler);
}

/**
 *@tc.name: sigchain_remove_all_special_handler_test_001
 *@tc.desc: Test the remove_all_special_handler function. When the input signo is
            out of range (0, _NSIG), the expectation is abort.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_remove_all_special_handler_test_001, TestSize.Level0)
{
    EXPECT_DEATH(remove_all_special_handler(-1), ".*");
    EXPECT_DEATH(remove_all_special_handler(_NSIG), ".*");
}

/**
 *@tc.name: sigchain_remove_all_special_handler_test_002
 *@tc.desc: Test the remove_all_special_handler function. When removing
            a signal which is not marked(special action list is empty)
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, sigchain_remove_all_special_handler_test_002, TestSize.Level0)
{
    remove_all_special_handler(45);
    remove_all_special_handler(45);
}

/**
 *@tc.name: intercept_sigaction_test_001
 *@tc.desc: Test the intercept_sigaction function. When the input signo is
            out of range (0, _NSIG), the result is false.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, intercept_sigaction_test_001, TestSize.Level0)
{
    struct sigaction sa, old;
    bool res = intercept_sigaction(-1, &sa, &old);
    ASSERT_TRUE(res == false);
}

/**
 *@tc.name: intercept_sigaction_test_002
 *@tc.desc: Test the intercept_sigaction function. When the input signo is
            out of range(0, _NSIG), the result is false.
 *@tc.type: FUNC
 */
HWTEST_F(SigchainTest, intercept_sigaction_test_002, TestSize.Level0)
{
    struct sigaction sa, old;
    bool res = intercept_sigaction(_NSIG, &sa, &old);
    ASSERT_TRUE(res == false);
}

} //namespace
} //namespace OHOS