/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name       : strcmp_0100
 * @tc.desc       : Verify strcmp returns 0 when two strings are equal.
 * @tc.level      : Level 0
 */
void strcmp_0100(void)
{
    const char *str1 = "hello";
    const char *str2 = "hello";
    int ret = strcmp(str1, str2);
    EXPECT_EQ("strcmp_0100", ret, 0);
}

/**
 * @tc.name       : strcmp_0200
 * @tc.desc       : Verify strcmp returns negative when first string is less than second.
 * @tc.level      : Level 0
 */
void strcmp_0200(void)
{
    const char *str1 = "apple";
    const char *str2 = "banana";
    int ret = strcmp(str1, str2);
    EXPECT_LT("strcmp_0200", ret, 0);
}

/**
 * @tc.name       : strcmp_0300
 * @tc.desc       : Verify strcmp returns positive when first string is greater than second.
 * @tc.level      : Level 0
 */
void strcmp_0300(void)
{
    const char *str1 = "zebra";
    const char *str2 = "apple";
    int ret = strcmp(str1, str2);
    EXPECT_MT("strcmp_0300", ret, 0);
}

/**
 * @tc.name       : strcmp_0400
 * @tc.desc       : Verify strcmp returns 0 when both strings are empty.
 * @tc.level      : Level 1
 */
void strcmp_0400(void)
{
    const char *str1 = "";
    const char *str2 = "";
    int ret = strcmp(str1, str2);
    EXPECT_EQ("strcmp_0400", ret, 0);
}

/**
 * @tc.name       : strcmp_0500
 * @tc.desc       : Verify strcmp returns negative when first string is empty.
 * @tc.level      : Level 1
 */
void strcmp_0500(void)
{
    const char *str1 = "";
    const char *str2 = "hello";
    int ret = strcmp(str1, str2);
    EXPECT_LT("strcmp_0500", ret, 0);
}

/**
 * @tc.name       : strcmp_0600
 * @tc.desc       : Verify strcmp returns positive when second string is empty.
 * @tc.level      : Level 1
 */
void strcmp_0600(void)
{
    const char *str1 = "hello";
    const char *str2 = "";
    int ret = strcmp(str1, str2);
    EXPECT_MT("strcmp_0600", ret, 0);
}

/**
 * @tc.name       : strcmp_0700
 * @tc.desc       : Verify strcmp is case-sensitive (uppercase vs lowercase).
 * @tc.level      : Level 0
 */
void strcmp_0700(void)
{
    const char *str1 = "Hello";
    const char *str2 = "hello";
    int ret = strcmp(str1, str2);
    EXPECT_NE("strcmp_0700", ret, 0);
}

/**
 * @tc.name       : strcmp_0800
 * @tc.desc       : Verify strcmp compares strings with same prefix but different lengths.
 * @tc.level      : Level 1
 */
void strcmp_0800(void)
{
    const char *str1 = "hello";
    const char *str2 = "hello world";
    int ret = strcmp(str1, str2);
    EXPECT_LT("strcmp_0800", ret, 0);
}

/**
 * @tc.name       : strcmp_0900
 * @tc.desc       : Verify strcmp with strings containing special characters.
 * @tc.level      : Level 1
 */
void strcmp_0900(void)
{
    const char *str1 = "hello@world";
    const char *str2 = "hello@world";
    int ret = strcmp(str1, str2);
    EXPECT_EQ("strcmp_0900", ret, 0);
}

/**
 * @tc.name       : strcmp_1000
 * @tc.desc       : Verify strcmp with strings containing numbers.
 * @tc.level      : Level 1
 */
void strcmp_1000(void)
{
    const char *str1 = "test123";
    const char *str2 = "test123";
    int ret = strcmp(str1, str2);
    EXPECT_EQ("strcmp_1000", ret, 0);
}

int main(void)
{
    strcmp_0100();
    strcmp_0200();
    strcmp_0300();
    strcmp_0400();
    strcmp_0500();
    strcmp_0600();
    strcmp_0700();
    strcmp_0800();
    strcmp_0900();
    strcmp_1000();
    return t_status;
}