/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include "hook.h"
#include "functionalext.h"

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {

const int COUNT = 62;
const int ISALNUM_SUCCESS_RETURN = 1;
const int COUNT_DEFAULT_LOCALE_UTF_8_1_BYTE = 62;
const int COUNT_DEFAULT_LOCALE_UTF_8_2_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_3_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE_NEGTIVE = 0;
const int UTF_8_1_BYTE_MIN = 0x00;
const int UTF_8_1_BYTE_MAX = 0x7f;
const int UTF_8_2_BYTE_MIN = 0x0080;
const int UTF_8_2_BYTE_MAX = 0x07ff;
const int UTF_8_3_BYTE_MIN = 0x0800;
const int UTF_8_3_BYTE_MAX = 0xffff;
const int UTF_8_4_BYTE_MIN = 0x10000;
const int UTF_8_4_BYTE_MAX = 0x10ffff;
const int EOK = 0;

class IsalnumlTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name      : isalnum_test_001
 * @tc.desc      : The parameter c is an English letter, and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
HWTEST_F(IsalnumTest, isalnum_test_001, TestSize.Level0)
{
    int ret = isalnum('a');
    EXPECT_NE("isalnum_test_001", ret, EOK);
}

/**
 * @tc.name      : isalnum_test_002
 * @tc.desc      : The parameter c is an English number, and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
HWTEST_F(IsalnumTest, isalnum_test_002, TestSize.Level0)
{
    int ret = isalnum('1');
    EXPECT_NE("isalnum_test_002", ret, EOK);
}

/**
 * @tc.name      : isalnum_test_003
 * @tc.desc      : The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
HWTEST_F(IsalnumTest, isalnum_test_003, TestSize.Level2)
{
    int ret = isalnum('*');
    EXPECT_EQ("isalnum_test_003", ret, EOK);
}

/**
 * @tc.name      : isalnum_test_004
 * @tc.desc      : Determine the number of letters and numbers in the ascii code table.
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_004, TestSize.Level1)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isalnum((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_004", total, COUNT);
}

/**
 * @tc.name      : isalnum_test_005
 * @tc.desc      : Test all input:0x00 - 0x7f(UTF-8 1 byte).
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_005, TestSize.Level1)
{
    int total = 0;
    for (int i = UTF_8_1_BYTE_MIN; i <= UTF_8_1_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_test_005", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_005", total, COUNT_DEFAULT_LOCALE_UTF_8_1_BYTE);
}

/**
 * @tc.name      : isalnum_test_006
 * @tc.desc      : Test all input:0x0080 - 0x07ff(UTF-8 2 byte).
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_006, TestSize.Level1)
{
    int total = 0;
    for (int i = UTF_8_2_BYTE_MIN; i <= UTF_8_2_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_test_006", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_006", total, COUNT_DEFAULT_LOCALE_UTF_8_2_BYTE);
}

/**
 * @tc.name      : isalnum_test_007
 * @tc.desc      : Test all input:0x0800 - 0xffff(UTF-8 3 byte).
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_007, TestSize.Level1)
{
    int total = 0;
    for (int i = UTF_8_3_BYTE_MIN; i <= UTF_8_3_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_test_007", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_007", total, COUNT_DEFAULT_LOCALE_UTF_8_3_BYTE);
}

/**
 * @tc.name      : isalnum_test_008
 * @tc.desc      : Test all input:0x10000 - 0x10ffff(UTF-8 4 byte).
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_008, TestSize.Level1)
{
    int total = 0;
    for (int i = UTF_8_4_BYTE_MIN; i <= UTF_8_4_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_test_008", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_008", total, COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE);
}

/**
 * @tc.name      : isalnum_test_009
 * @tc.desc      : Test zero input.
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_009, TestSize.Level1)
{
    int ret = isalnum(0);
    EXPECT_EQ("isalnum_test_009", ret, 0);
}

/**
 * @tc.name      : isalnum_test_010
 * @tc.desc      : Test negative input -1.
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_010, TestSize.Level1)
{
    int ret = isalnum(-1);
    EXPECT_EQ("isalnum_test_010", ret, 0);
}

/**
 * @tc.name      : isalnum_test_011
 * @tc.desc      : Test negative input, max -1 * 0x10ffff.
 * @tc.level     : Level 1
 */
HWTEST_F(IsalnumTest, isalnum_test_011, TestSize.Level1)
{
    int total = 0;
    for (int i = -1; i >= -1 * UTF_8_4_BYTE_MAX; i--) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_test_011", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_test_011", total, COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE_NEGTIVE);
}

}
}