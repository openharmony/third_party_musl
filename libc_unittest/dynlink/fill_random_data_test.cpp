/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <fcntl.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>
 
#include <hook.h>

using namespace testing::ext;
using namespace std;

extern "C" {

void fill_random_data(void *buf, size_t buflen);

int open(const char *filename, int falgs, ...);

ssize_t read(int fd, void *buf, size_t count);

}

namespace OHOS {
namespace {

class DynlinkTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};
        
void DynlinkTest::TearDownTestCase(void)
{
        
}

/**
 * @tc.name: test fill_random_data1
 * @tc.desc: test fill_random_data1
 * @tc.type: FUNC
 */
HWTEST_F(DynlinkTest, fill_random_data1, TestSize.Level0)
{
    char buf[1024];

    set_hook_flag(OPEN_FLAG, true);
    set_hook_flag(READ_FLAG_3, true);
    fill_random_data((void*)buf, 1024);
    set_hook_flag(OPEN_FLAG, false);
    set_hook_flag(READ_FLAG_3, false);
}

}
}