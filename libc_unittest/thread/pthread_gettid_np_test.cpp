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
}

namespace OHOS {
namespace {

class PthreadGettidNpTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void PthreadGettidNpTest::TearDownTestCase(void)
{
}

void *thread_fun(void *arg)
{
    return NULL;
}

/**
 * @tc.name: PthreadGettidNpTest_001
 * @tc.desc: pthread_t doesn't exist
 * @tc.type: FUNC
 */
HWTEST_F(PthreadGettidNpTest, PthreadGettidNpTest_001, TestSize.Level0)
{
    pthread_t t1;
    pthread_create(&t1, NULL, thread_fun, NULL);
    pthread_t t2;
    pthread_create(&t2, NULL, thread_fun, NULL);
    pthread_t t3;

    pthread_create(&t3, NULL, thread_fun, NULL);
    pthread_join(t3, NULL);

    pthread_t t4;
    pthread_create(&t4, NULL, thread_fun, NULL);

    pid_t recv_result = pthread_gettid_np(t3);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t4, NULL);

    EXPECT_EQ(recv_result, -1);
}

}   // namespace
}   // namespace OHOS