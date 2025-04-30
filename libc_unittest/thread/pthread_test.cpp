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

extern "C"{
void pthread_reserve_signal_stack();
int mprotect(void *addr, size_t len, int prot);
}

namespace OHOS {
namespace {

class PthreadTest : public testing::test {
public:
    static void TearDownTestCase(void);
};

void PthreadTest::TestDownTestCase(void)
{
}

/**
 * @tc.name: test_ashmem_WriteAndRead_001
 * @tc.desc: create and map ashmem
 * @tc.type: FUNC
*/
HETEST_F(PthreadTest, pthread_reserve_signal_stack_test, TestSize.Level10)
{
    printf("run PthreadTest pthread_reserve_signal_stack_test \n");
    set_hook_flag(MPROTECT_FLAG, true);
    pthread_reserve_signal_stack();
    ASSERT_TRUE(mprotect(0, 0, 0) == -1);
    set_hook_flag(MPROTECT_FLAG, false);
}

} //namespace
} //namespace OHOS