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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <gtest/gtest.h>

#include "hook.h"

using namespace testing::ext;
using namespace std;

extern "C"{
int mprotect(void *addr, size_t len, int prot);
void init_file_lock(FILE *f);
void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off);
}

namespace OHOS {
namespace {
    
class PthreadCreateTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void PthreadCreateTest::TearDownTestCase(void)
{
}

void *thread_fun(void *arg)
{
    sleep(1);
    return NULL;
}

/**
 * @tc.name: PthreadCreateTest_001
 * @tc.desc: create and mprotect fail
 * @tc.type: FUNC
 */
HWTEST_F(PthreadCreateTest, PthreadCreateTest_001, TestSize.Level0)
{
    printf("run PthreadCreateTest_001 \n");
    pthread_t tid;
    set_hook_flag(MPROTECT_FLAG, true);
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    set_hook_flag(MPROTECT_FLAG, false);
    ASSERT_TRUE(ret == EAGAIN);
}

/**
 * @tc.name: PthreadCreateTest_002
 * @tc.desc: create and mmap fail
 * @tc.type: FUNC
 */
HWTEST_F(PthreadCreateTest, PthreadCreateTest_002, TestSize.Level0)
{
    printf("run PthreadCreateTest_002 \n");
    pthread_t tid;
    set_hook_flag(MMAP_FLAG, true);
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    set_hook_flag(MMAP_FLAG, false);
    ASSERT_TRUE(ret == EAGAIN);
}

/**
 * @tc.name: PthreadCreateTest_003
 * @tc.desc: create and mprotect fail
 * @tc.type: FUNC
 */
HWTEST_F(PthreadCreateTest, PthreadCreateTest_003, TestSize.Level0)
{
    printf("run PthreadCreateTest_003 \n");
    pthread_t tid;
    set_hook_flag(MPROTECT_1_FLAG, true);
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    set_hook_flag(MPROTECT_1_FLAG, false);
    ASSERT_TRUE(ret == 0);
    pthread_join(tid, NULL);
}

/**
 * @tc.name: PthreadCreateTest_004
 * @tc.desc: create and mprotect fail
 * @tc.type: FUNC
 */
HWTEST_F(PthreadCreateTest, PthreadCreateTest_004, TestSize.Level0)
{
    printf("run PthreadCreateTest_004 \n");
    pthread_t tid;
    pthread_attr_t a;
    pthread_attr_init(&a);
    int res = pthread_attr_setinheritsched(&a, 1);
    printf("run pthread_attr_setinheritsched res : %d \n", res);
    int ret = pthread_create(&tid, &a, thread_fun, NULL);
    ASSERT_TRUE(ret == 0);
    pthread_join(tid, NULL);
}

HWTEST_F(PthreadCreateTest, PthreadCreateTest_005, TestSize.Level0)
{
    printf("run PthreadCreateTest_005 \n");
    FILE f;
    init_file_lock(NULL);
    init_file_lock(&f);
}

}   // namespace
}   // namespace OHOS