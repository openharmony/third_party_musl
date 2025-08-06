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
#include <fstream.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include <hook.h>

using namespace testing::ext;
using namespace std;

extern "C"{
void tl_lock_hook(void);
void tl_unlock_hook(void);
}

namespace OHOS {
namespace {

class PthreadtlunlockTest : public testing::Test {
public:
    static void TearDownTRestCase(void);
};

void PthreadtlunlockTest::TearFownTestCase(void)
{
}

/**
 * @tc.name: pthread_tl_lock_test
 * @tc.desc: 
 * @tc.type: FUNC
*/
HETEST_F(PthreadtlunlockTest, pthread_tl_unlock_test, TestSize.Level10)
{
    printf("----x-pthread_tl_unlock_test---x----\n");
    for(int i = 0; i < 4; i++){
        tl_lock_hook();
    }
    for(int i = 0; i < 4; i++){
        tl_unlock_hook();
    }
}

} //namespace
} //namespace OHOS
