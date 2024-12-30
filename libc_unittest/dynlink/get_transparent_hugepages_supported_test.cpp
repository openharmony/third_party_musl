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
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>
#include <pthread.h>
#include "hook.h"

using namespace testing::ext;
using namespace std;

extern "C"{
    bool get_transparent_hugepages_supported(void);
    int open(const char *filename, int flags, ...);
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
 * @tc.name: get_transparent_hugepages_supported_test001
 * @tc.desc: open failed
 * @tc.type: FUNC
 */
HWTEST_F(DynlinkTest, get_transparent_hugepages_supported_test_001, TestSize.Level0)
{
    bool b = get_transparent_hugepages_supported();
    EXPECT_TRUE(b == false);
}


/**
 * @tc.name: get_transparent_hugepages_supported_test002
 * @tc.desc: open failed
 * @tc.type: FUNC
 */
HWTEST_F(DynlinkTest, get_transparent_hugepages_supported_test_002, TestSize.Level0)
{
    set_hook_flag(OPEN_FLAG, true);
    bool b = get_transparent_hugepages_supported();
    set_hook_flag(OPEN_FLAG, false);
    EXPECT_TRUE(b == true);
}


/**
 * @tc.name: get_transparent_hugepages_supported_test003
 * @tc.desc: read failed
 * @tc.type: FUNC
 */
HWTEST_F(DynlinkTest, get_transparent_hugepages_supported_test_003, TestSize.Level0)
{
    set_hook_flag(OPEN_FLAG, true);
    set_hook_flag(READ_FLAG_3, true);
    bool b = get_transparent_hugepages_supported();
    set_hook_flag(READ_FLAG_3, false);
    set_hook_flag(OPEN_FLAG, false);

    EXPECT_TRUE(b == false);
}


/**
 * @tc.name: get_transparent_hugepages_supported_test004
 * @tc.desc: read failed
 * @tc.type: FUNC
 */
HWTEST_F(DynlinkTest, get_transparent_hugepages_supported_test_004, TestSize.Level0)
{
    set_hook_flag(OPEN_FLAG, true);
    set_hook_flag(STRSTR_FLAG, true);
    bool b = get_transparent_hugepages_supported();
    set_hook_flag(STRSTR_FLAG, false);
    set_hook_flag(OPEN_FLAG, false);
    EXPECT_TRUE(b == false);
}

} //namespace

} //namespace OHOS
