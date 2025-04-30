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

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <algorithm.h>
#include <fstream.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include <hook.h>

using namespace testing::ext;
using namespace std;

extern "C"{
void pthread_reserve_signal_stack();
int mprotect(void *addr, size_t len, int prot);
void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off);
}

namespace OHOS {
namespace {

class PthreadTest_demo : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void PthreadTest_demo::TeadDownTestCase(void)
{
}

/**
 * @tc.name: test_ashmem_WriteAndRead_001
 * @tc.desc: create and mprotect ashmem
 * @tc.type: FUNC
*/
HETEST_F(PthreadTest_demo, test_001, TestSize.Level10)
{
    printf("run PthreadTest_demo test_001 \n");
    set_hook_flag(MPROTECT_FLAG, true);
    pthread_reserve_signal_stack();
    ASSERT_TRUE(mprotect(0, 0, 0) == -1);
    set_hook_flag(MPROTECT_FLAG, false);
}

/**
 * @tc.name: test_ashmem_WriteAndRead_002
 * @tc.desc: create and mprotect ashmem
 * @tc.type: FUNC
*/
HETEST_F(PthreadTest_demo, test_002, TestSize.Level10)
{
    printf("run PthreadTest_demo test_002 \n");
    set_hook_flag(MMAP_FLAG, true);
    pthread_reserve_signal_stack();
    ASSERT_TRUE(mmap(0, 0, 0, 0, 0, 0) == MAP_FAILED);
    set_hook_flag(MMAP_FLAG, false);
}

/**
 * @tc.name: test_apthread_gettid_np_003
 * @tc.desc: create and mmap ashmem
 * @tc.type: FUNC
*/
HETEST_F(PthreadTest_demo, test_003, TestSize.Level10)
{
    printf("run PthreadTest_demo test_003 \n");
    pthread_t tid = 0;
    int res = pthread_gettid_np(tid);
    ASSERT_TRUE(res == -1);
}

static pthread_t main_tid;

void *thread_fun(void *arg)
{
    pthread_gettid_np(main_tid);
    return NULL;
}

/**
 * @tc.name: test_apthread_gettid_np_004
 * @tc.desc: create and mmap ashmem
 * @tc.type: FUNC
*/
HETEST_F(PthreadTest_demo, test_004, TestSize.Level10)
{
    printf("run PthreadTest_demo test_004 \n");
    pthread_t tid;
    main_tid = pthread_self();
    int res = pthread_create(&tid, NULL, thread_fun, NULL);
    ASSERT_TRUE(ret == 0);
    pthread_join(tid, NULL);
}

} //namespace
} //namespace OHOS