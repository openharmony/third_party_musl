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
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include "hook.h"

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {

#define TEST_RESULT_ZERO 0
#define TEST_RESULT_ONE 1

class IsasciiTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name: isascii_test_001
 * @tc.desc: Pass the parameter 'a' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_001, TestSize.Level0)
{
    int result = isascii('a');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_002
 * @tc.desc: Pass the parameter 'A' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_002, TestSize.Level0)
{
    int result = isascii('A');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_003
 * @tc.desc: Pass the parameter '0' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_003, TestSize.Level0)
{
    int result = isascii('0');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_004
 * @tc.desc: Pass the parameter ' ' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_004, TestSize.Level0)
{
    int result = isascii(' ');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_005
 * @tc.desc: Pass the parameter '\t' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_005, TestSize.Level0)
{
    int result = isascii('\t');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_006
 * @tc.desc: Pass the parameter '\n' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_006, TestSize.Level0)
{
    int result = isascii('\n');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_007
 * @tc.desc: Pass the parameter '\r' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_007, TestSize.Level0)
{
    int result = isascii('\r');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_008
 * @tc.desc: Pass the parameter 0x7F to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_008, TestSize.Level0)
{
    int result = isascii(0x7F);
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_009
 * @tc.desc: Pass the parameter 0x80 to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_009, TestSize.Level0)
{
    int result = isascii(0x80);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

/**
 * @tc.name: isascii_test_010
 * @tc.desc: Pass the parameter 0xFF to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_010, TestSize.Level0)
{
    int result = isascii(0xFF);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

/**
 * @tc.name: isascii_test_011
 * @tc.desc: Pass the parameter '\0' to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_011, TestSize.Level0)
{
    int result = isascii('\0');
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_012
 * @tc.desc: Pass the parameter 0x1F to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_012, TestSize.Level0)
{
    int result = isascii(0x1F);
    EXPECT_EQ(result, TEST_RESULT_ONE);
}

/**
 * @tc.name: isascii_test_013
 * @tc.desc: Pass the parameter 0xA0 to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_013, TestSize.Level0)
{
    int result = isascii(0xA0);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

/**
 * @tc.name: isascii_test_014
 * @tc.desc: Pass the parameter 0xC0 to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_014, TestSize.Level0)
{
    int result = isascii(0xC0);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

/**
 * @tc.name: isascii_test_015
 * @tc.desc: Pass the parameter 0xE0 to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_015, TestSize.Level0)
{
    int result = isascii(0xE0);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

/**
 * @tc.name: isascii_test_016
 * @tc.desc: Pass the parameter 0xF0 to the isascii interface
 * @tc.type: FUNC
 */
HWTEST_F(IsasciiTest, isascii_test_016, TestSize.Level0)
{
    int result = isascii(0xF0);
    EXPECT_EQ(result, TEST_RESULT_ZERO);
}

}
}
