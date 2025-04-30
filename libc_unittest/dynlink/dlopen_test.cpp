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

#include "hook.h"

using namespace testing::ext;
using namespace std;

extern "C" {

int mprotect(void *addr, size_t len, int prot);

void *mmap_fixed(void *p, size_t n, int prot, int flags, int fd, off_t off);

}

namespace OHOS {
namespace {

class DlopenTest : public testing::Test {
public:
    static void TearDownTestCase();
};

void DlopenTest::TearDownTestCase(void)
{

}

/**
 * @tc.name: test_DlopenTest_001
 * @tc.desc: dlopen so mprotect failed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_001, TestSize.Level0)
{
    set_hook_flag(MPROTECT_FLAG, true);
    void* handle = dlopen("/data/local/tmp/libdlopen_dso.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
    ASSERT_TRUE(mprotect(0, 0, 0) == -1);
    set_hook_flag(MPROTECT_FLAG, false);
}

/**
 * @tc.name: test_DlopenTest_002
 * @tc.desc: dlopen so zip succeed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_002, TestSize.Level0)
{
    void* handle = dlopen("/data/local/tmp/libzipalign_lspath.zip!/libso/libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
}

/**
 * @tc.name: test_DlopenTest_003
 * @tc.desc: dlopen so zip path wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_003, TestSize.Level0)
{
    void* handle = dlopen("/data/local/tmp/libzipalign_lspath1111.zip!/libso/libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(handle == nullptr);
    dlclose(handle);
}

/**
 * @tc.name: test_DlopenTest_004
 * @tc.desc: dlopen so create_loadtasks failed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_004, TestSize.Level0)
{
    set_hook_flag(CREATE_LOADTASKS_FLAG, true);
    void* handle = dlopen("/data/local/tmp/libdlopen_dso.so", RTLD_NOW);
    set_hook_flag(CREATE_LOADTASKS_FLAG, false);
    ASSERT_TRUE(handle == nullptr);
    dlclose(handle);
}

/**
 * @tc.name: test_DlopenTest_005
 * @tc.desc: dlopen so create_loadtasks failed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_005, TestSize.Level0)
{
    set_hook_flag(CREATE_LOADTASKS_FLAG, true);
    void* handle = dlopen("/data/local/tmp/libdlopen_dso.so", RTLD_NOW);
    set_hook_flag(CREATE_LOADTASKS_FLAG, false);
    ASSERT_TRUE(handle == nullptr);
    dlclose(handle);
}

/**
 * @tc.name: test_DlopenTest_006
 * @tc.desc: mmap_fixed read failed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_006, TestSize.Level0)
{
    int fd = -1;
    const char *path = "/dev/random";
    fd = open(path, O_RDWR);

    if (fd == -1) {
        ASSERT_TRUE(false);
        return;
    }

    char buf[128];
    set_hook_flag(READ_FLAG_3, true);
    void *p = mmap_fixed(buf, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    set_hook_flag(READ_FLAG_3, false);
    ASSERT_TRUE(p == MAP_FAILED);
    if (fd != -1) {
        close(fd);
    }
}

/**
 * @tc.name: test_DlopenTest_007
 * @tc.desc: test dlvsym
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_007, TestSize.Level0)
{
    void* handle = dlvsym(RTLD_DEFAULT, "dso_hard_symver_if", "dso_version_invalid");
    ASSERT_TRUE(handle == nullptr);
    dlclose(handle);
}

/**
 * @tc.name: test_DlopenTest_008
 * @tc.desc: test dlvsym
 * @tc.type: FUNC
 */
HWTEST_F(DlopenTest, DlopenTest_008, TestSize.Level0)
{
    void* handle = dlvsym(RTLD_NEXT, "dso_hard_symver_if", "dso_version_invalid");
    ASSERT_TRUE(handle == nullptr);
    dlclose(handle);
}

}
} // namespace OHOS

