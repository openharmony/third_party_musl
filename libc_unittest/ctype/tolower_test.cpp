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

class TolowerTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name: tolower_test_001
 * @tc.desc: Convert uppercase to lowercase by tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_001, TestSize.Level0)
{
    char ch = 'A';
    char result = tolower(ch);
    EXPECT_EQ(result, 'a');
}

/**
 * @tc.name: tolower_test_002
 * @tc.desc: Test the result obtained by passing a number to tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_002, TestSize.Level0)
{
    char ch = 2;
    char result = tolower(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: tolower_test_003
 * @tc.desc: Test the result of passing lowercase letters to tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_003, TestSize.Level0)
{
    char ch = 'a';
    char result = tolower(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: tolower_test_004
 * @tc.desc: Test the result of passing special characters into tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_004, TestSize.Level0)
{
    char ch = '$';
    char result = tolower(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: tolower_test_005
 * @tc.desc: Test the result of passing whitespace character into tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_005, TestSize.Level0)
{
    char ch = ' ';
    char result = tolower(ch);
    EXPECT_EQ(result, ch);
}

/**
 * @tc.name: tolower_test_006
 * @tc.desc: Test the result of passing all lowercase characters into tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_006, TestSize.Level0)
{
    char str1[] = "helloworld";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = tolower(str1[i]);
    }
    char wantStr[] = "helloworld";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: tolower_test_007
 * @tc.desc: Test the result of passing all uppercase characters into tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_007, TestSize.Level0)
{
    char str1[] = "HELLOWORLD";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = tolower(str1[i]);
    }
    char wantStr[] = "helloworld";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: tolower_test_008
 * @tc.desc: Test the result of passing uncertain characters into tolower
 * @tc.type: FUNC
 */
HWTEST_F(TolowerTest, tolower_test_008, TestSize.Level0)
{
    char str1[] = "Hello world!";
    size_t sizeStr = sizeof(str1);
    char str2[sizeStr];
    for (int i = 0; str1[i]; i++) {
        str2[i] = tolower(str1[i]);
    }
    char wantStr[] = "hello world!";
    int result = strcmp(wantStr, str2);
    EXPECT_EQ(result, 0);
}

}
}

