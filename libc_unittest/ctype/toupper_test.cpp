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
#include <string.h>
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

class ToupperTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name: toupper_test_001
 * @tc.desc: Convert lowercase to uppercase via toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_001, TestSize.Level0)
{
    char ch = 'a';
    char result = toupper(ch);
    EXPECT_EQ(result, 'A');
}

/**
 * @tc.name: toupper_test_002
 * @tc.desc: Test the result of passing numbers into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_002, TestSize.Level0)
{
    char ch = 2;
    char result = toupper(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: toupper_test_003
 * @tc.desc: Test the result of passing capital letters into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_003, TestSize.Level0)
{
    char ch = 'A';
    char result = toupper(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: toupper_test_004
 * @tc.desc: Test the result of passing special characters into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_004, TestSize.Level0)
{
    char ch = '$';
    char result = toupper(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: toupper_test_005
 * @tc.desc: Test the result of passing whitespace character into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_005, TestSize.Level0)
{
    char ch = ' ';
    char result = toupper(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: toupper_test_006
 * @tc.desc: Test the result of passing all lowercase characters into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_006, TestSize.Level0)
{
    char str1[] = "helloworld";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = toupper(str1[i]);
    }
    char wantStr[] = "HELLOWORLD";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: toupper_test_007
 * @tc.desc: Test the result of passing all uppercase characters into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_007, TestSize.Level0)
{
    char str1[] = "HELLOWORLD";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = toupper(str1[i]);
    }
    char wantStr[] = "HELLOWORLD";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: toupper_test_008
 * @tc.desc: Test the result of passing Uncertain characters into toupper
 * @tc.type: FUNC
 */
HWTEST_F(ToupperTest, toupper_test_008, TestSize.Level0)
{
    char str1[] = "Hello world!";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = toupper(str1[i]);
    }
    char wantStr[] = "HELLO WORLD!";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

}
}

