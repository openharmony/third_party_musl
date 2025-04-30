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

#define SEEK_1 9089
#define SEEK_2 8992

using namespace testing::ext;
using namespace std;

extern "C" {

size_t strlcpy(char *d, const char *s, size_t n);

FILE *fopen(const char *__restrict filename, const char *__restrict mode);

}

namespace OHOS {

class DlopenZipTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void DlopenZipTest::TearDownTestCase(void)
{

}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: open zip so succeed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_001, TestSize.Level0)
{
    void* handle = dlopen("libdlopen_zip_test.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);

    Dl_namespace dlns;
    dlns_init(&dlns, "lspath1");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    int* value = reinterpret_cast<int*>(dlsym(handle, "g_testNumber"));
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 1000);

    dlclose(handle);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test path size > 512
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_002, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath2");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(STRLCPY_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
    ASSERT_TRUE(strlcpy(nullptr, "/data/local/tmp/libzipalign_lspath.zip!/libso/libdlopen_zip_test.so", 512) == 512);
    set_hook_flag(STRLCPY_FLAG, false);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip path wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_003, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath3");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath11111.zip!/libso");

    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so path wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_004, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath4");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test11111.so", RTLD_NOW);
    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so zipfile fseek wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_005, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath5");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FSEEK_SEEK_END_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(fseek(nullptr, 0, SEEK_END) == -1);
    set_hook_flag(FSEEK_SEEK_END_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so zipfile ftell wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_006, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath6");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FTELL_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(ftell(nullptr) == -1);
    set_hook_flag(FTELL_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so zipfile fseek wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_007, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath7");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FSEEK_SEEK_SET_1_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(fseek(nullptr, SEEK_1, SEEK_SET) == -1);
    set_hook_flag(FSEEK_SEEK_SET_1_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so zipfile fseek wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_008, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath8");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FSEEK_SEEK_SET_2_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(fseek(nullptr, SEEK_2, SEEK_SET) == -1);
    set_hook_flag(FSEEK_SEEK_SET_2_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: test zip so zipfile fseek wrong
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_009, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath9");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FSEEK_SEEK_SET_3_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    ASSERT_TRUE(fseek(nullptr, 0, SEEK_SET) == -1);
    set_hook_flag(FSEEK_SEEK_SET_3_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: Open uncompressed library: fread end locator failed
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_010, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath10");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FREAD_1_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    set_hook_flag(FREAD_1_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: Open uncompressed library: fread centra dir entry failed.
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_011, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath11");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FREAD_2_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    set_hook_flag(FREAD_2_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: Open uncompressed library: fread file name failed.
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_012, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath12");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FREAD_3_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    set_hook_flag(FREAD_3_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

/**
 * @tc.name: dlopen_zip_test
 * @tc.desc: Open uncompressed library: fread file name failed.
 * @tc.type: FUNC
 */
HWTEST_F(DlopenZipTest, dlopen_zip_test_013, TestSize.Level0)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "lspath13");
    dlns_create(&dlns, "/data/local/tmp/libzipalign_lspath.zip!/libso");

    set_hook_flag(FREAD_4_FLAG, true);
    void* handle = dlopen_ns(&dlns, "libdlopen_zip_test.so", RTLD_NOW);
    set_hook_flag(FREAD_4_FLAG, false);

    EXPECT_TRUE(handle == nullptr);
}

}