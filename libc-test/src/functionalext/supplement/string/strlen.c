/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

const int INIT_LEN = 0;

/**
 * @tc.name       :strlen_0100
 * @tc.desc       :Verify strlen is successed when src is "this is a test 1234~@#$%".
 * @tc.desc       :level 0
 */
void strlen_0100(void)
{
    char src[] = "this is a test 1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0100", ret, srcLen);
}

/**
 * @tc.name       :strlen_0200
 * @tc.desc       :Verify strlen is successed when src is "测试1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0200(void)
{
    char src[] = "测试1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0200", ret, srcLen);
}

/**
 * @tc.name       :strlen_0300
 * @tc.desc       :Verify strlen is successed when src is "".
 * @tc.desc       :level 1
 */
void strlen_0300(void)
{
    char src[] = "";
    int srcLen = INIT_LEN;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0300", ret, srcLen);
}

/**
 * @tc.name       :strlen_0400
 * @tc.desc       :Verify strlen is successed when src is "テスト".
 * @tc.desc       :level 1
 */
void strlen_0400(void)
{
    char src[] = "テスト";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0400", ret, srcLen);
}

/**
 * @tc.name       :strlen_0500
 * @tc.desc       :Verify strlen is successed when src is "測試1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0500(void)
{
    char src[] = "測試1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0500", ret, srcLen);
}

/**
 * @tc.name       :strlen_0600
 * @tc.desc       :Verify strlen is successed when src is "测试測試1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0600(void)
{
    char src[] = "测试測試1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0600", ret, srcLen);
}

/**
 * @tc.name       :strlen_0700
 * @tc.desc       :Verify strlen is successed when src is "测试測試テスト1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0700(void)
{
    char src[] = "测试測試テスト1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0700", ret, srcLen);
}

/**
 * @tc.name       :strlen_0800
 * @tc.desc       :Verify strlen is successed when src is "시험".
 * @tc.desc       :level 1
 */
void strlen_0800(void)
{
    char src[] = "시험";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0800", ret, srcLen);
}

/**
 * @tc.name       :strlen_0900
 * @tc.desc       :Verify strlen is successed when src is "测试測試テスト시험1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0900(void)
{
    char src[] = "测试測試テスト시험1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0900", ret, srcLen);
}

/**
 * @tc.name       :strlen_1000
 * @tc.desc       :Verify strlen is successed when src is "тест".
 * @tc.desc       :level 1
 */
void strlen_1000(void)
{
    char src[] = "тест";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_1000", ret, srcLen);
}

/**
 * @tc.name      : strlen_1100
 * @tc.desc      : Basic test with a simple string.
 * @tc.level     : Level 0
 */
void strlen_1100(void)
{
    size_t want = 5;
    char *str = "hello";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1200
 * @tc.desc      : Test with an empty string.
 * @tc.level     : Level 0
 */
void strlen_1200(void)
{
    size_t want = 0;
    char *str = "";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1300
 * @tc.desc      : Test with a string of length 1.
 * @tc.level     : Level 0
 */
void strlen_1300(void)
{
    size_t want = 1;
    char *str = "a";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1400
 * @tc.desc      : Test with a string containing spaces.
 * @tc.level     : Level 0
 */
void strlen_1400(void)
{
    size_t want = 11;
    char *str = "hello world";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1500
 * @tc.desc      : Test with a string containing tabs and newlines.
 * @tc.level     : Level 0
 */
void strlen_1500(void)
{
    size_t want = 13;
    char *str = "hello\tworld\n";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1600
 * @tc.desc      : Test with a string containing special characters.
 * @tc.level     : Level 0
 */
void strlen_1600(void)
{
    size_t want = 5;
    char *str = "!@#$%";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1700
 * @tc.name      : strlen_1700
 * @tc.desc      : Test with a mixed alphanumeric string.
 * @tc.level     : Level 0
 */
void strlen_1700(void)
{
    size_t want = 8;
    char *str = "a1b2c3d4";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1800
 * @tc.desc      : Test with a string containing only digits.
 * @tc.level     : Level 0
 */
void strlen_1800(void)
{
    size_t want = 10;
    char *str = "1234567890";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_1900
 * @tc.desc      : Test with a string containing only uppercase letters.
 * @tc.level     : Level 0
 */
void strlen_1900(void)
{
    size_t want = 6;
    char *str = "ABCDEF";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_2000
 * @tc.desc      : Test with a string containing only lowercase letters.
 * @tc.level     : Level 0
 */
void strlen_2000(void)
{
    size_t want = 6;
    char *str = "abcdef";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"%s\") returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strlen_2100
 * @tc.desc      : Test with a string containing backslashes.
 * @tc.level     : Level 1
 */
void strlen_2100(void)
{
    size_t want = 3;
    char *str = "a\\b";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a\\\\b\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2200
 * @tc.desc      : Test with a string containing double quotes.
 * @tc.level     : Level 1
 */
void strlen_2200(void)
{
    size_t want = 5;
    char *str = "a\"bcd";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a\\\"bcd\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2300
 * @tc.desc      : Test with a string containing single quotes.
 * @tc.level     : Level 1
 */
void strlen_2300(void)
{
    size_t want = 5;
    char *str = "a'bcd";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a'bcd\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2400
 * @tc.desc      : Test with a string containing parentheses.
 * @tc.level     : Level 1
 */
void strlen_2400(void)
{
    size_t want = 7;
    char *str = "(hello)";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"(hello)\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2500
 * @tc.desc      : Test with a string containing brackets.
 * @tc.level     : Level 1
 */
void strlen_2500(void)
{
    size_t want = 7;
    char *str = "[world]";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"[world]\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2600
 * @tc.desc      : Test with a string containing braces.
 * @tc.level     : Level 1
 */
void strlen_2600(void)
{
    size_t want = 7;
    char *str = "{foo}";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"{foo}\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2700
 * @tc.desc      : Test with a string containing angle brackets.
 * @tc.level     : Level 1
 */
void strlen_2700(void)
{
    size_t want = 7;
    char *str = "<bar>";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"<bar>\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2800
 * @tc.desc      : Test with a string containing underscores.
 * @tc.level     : Level 1
 */
void strlen_2800(void)
{
    size_t want = 9;
    char *str = "snake_case";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"snake_case\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_2900
 * @tc.desc      : Test with a string containing hyphens.
 * @tc.level     : Level 1
 */
void strlen_2900(void)
{
    size_t want = 11;
    char *str = "kebab-case";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"kebab-case\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3000
 * @tc.desc      : Test with a string containing plus signs.
 * @tc.level     : Level 1
 */
void strlen_3000(void)
{
    size_t want = 5;
    char *str = "a+b+c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a+b+c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3100
 * @tc.desc      : Test with a string containing equals signs.
 * @tc.level     : Level 1
 */
void strlen_3100(void)
{
    size_t want = 7;
    char *str = "key=value";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"key=value\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3200
 * @tc.desc      : Test with a string containing asterisks.
 * @tc.level     : Level 1
 */
void strlen_3200(void)
{
    size_t want = 5;
    char *str = "a*b*c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a*b*c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3300
 * @tc.desc      : Test with a string containing slashes.
 * @tc.level     : Level 1
 */
void strlen_3300(void)
{
    size_t want = 9;
    char *str = "path/to/file";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"path/to/file\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3400
 * @tc.desc      : Test with a string containing backslashes (file path).
 * @tc.level     : Level 1
 */
void strlen_3400(void)
{
    size_t want = 11;
    char *str = "C:\\Users\\test";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"C:\\\\Users\\\\test\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3500
 * @tc.desc      : Test with a string containing pipes.
 * @tc.level     : Level 1
 */
void strlen_3500(void)
{
    size_t want = 7;
    char *str = "a|b|c|d";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a|b|c|d\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3600
 * @tc.desc      : Test with a string containing carets.
 * @tc.level     : Level 1
 */
void strlen_3600(void)
{
    size_t want = 5;
    char *str = "a^b^c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a^b^c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3700
 * @tc.desc      : Test with a string containing tildes.
 * @tc.level     : Level 1
 */
void strlen_3700(void)
{
    size_t want = 5;
    char *str = "a~b~c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a~b~c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3800
 * @tc.desc      : Test with a string containing colons.
 * @tc.level     : Level 1
 */
void strlen_3800(void)
{
    size_t want = 5;
    char *str = "a:b:c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a:b:c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_3900
 * @tc.desc      : Test with a string containing semicolons.
 * @tc.level     : Level 1
 */
void strlen_3900(void)
{
    size_t want = 5;
    char *str = "a;b;c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a;b;c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4000
 * @tc.desc      : Test with a string containing commas.
 * @tc.level     : Level 1
 */
void strlen_4000(void)
{
    size_t want = 7;
    char *str = "a,b,c,d";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a,b,c,d\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4100
 * @tc.desc      : Test with a string containing periods.
 * @tc.level     : Level 1
 */
void strlen_4100(void)
{
    size_t want = 7;
    char *str = "a.b.c.d";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a.b.c.d\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4200
 * @tc.desc      : Test with a string containing exclamation marks.
 * @tc.level     : Level 1
 */
void strlen_4200(void)
{
    size_t want = 7;
    char *str = "Hello!!!";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"Hello!!!\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4300
 * @tc.desc      : Test with a string containing question marks.
 * @tc.level     : Level 1
 */
void strlen_4300(void)
{
    size_t want = 7;
    char *str = "What???";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"What???\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4400
 * @tc.desc      : Test with a string containing at symbols.
 * @tc.level     : Level 1
 */
void strlen_4400(void)
{
    size_t want = 11;
    char *str = "user@example.com";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"user@example.com\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4500
 * @tc.desc      : Test with a string containing hash symbols.
 * @tc.level     : Level 1
 */
void strlen_4500(void)
{
    size_t want = 5;
    char *str = "a#b#c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a#b#c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4600
 * @tc.desc      : Test with a string containing dollar signs.
 * @tc.level     : Level 1
 */
void strlen_4600(void)
{
    size_t want = 5;
    char *str = "a$b$c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a$b$c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4700
 * @tc.desc      : Test with a string containing percent signs.
 * @tc.level     : Level 1
 */
void strlen_4700(void)
{
    size_t want = 5;
    char *str = "a%b%c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a%%b%%c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4800
 * @tc.desc      : Test with a string containing ampersands.
 * @tc.level     : Level 1
 */
void strlen_4800(void)
{
    size_t want = 5;
    char *str = "a&b&c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a&b&c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_4900
 * @tc.desc      : Test with a string containing asterisks and slashes.
 * @tc.level     : Level 1
 */
void strlen_4900(void)
{
    size_t want = 7;
    char *str = "a*/b*/c";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"a*/b*/c\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : strlen_5000
 * @tc.desc      : Test with a string containing a mix of many special characters.
 * @tc.level     : Level 1
 */
void strlen_5000(void)
{
    size_t want = 15;
    char *str = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
    size_t result = strlen(str);
    if (result != want) {
        t_error("%s: strlen(\"!@#$%%^&*()_+-=[]{}|;':\\\",./<>?\") returned %zu, expected %zu\n", __func__, result, want);
    }
}

int main(void)
{
    strlen_0100();
    strlen_0200();
    strlen_0300();
    strlen_0400();
    strlen_0500();
    strlen_0600();
    strlen_0700();
    strlen_0800();
    strlen_0900();
    strlen_1000();
    strlen_1100();
    strlen_1200();
    strlen_1300();
    strlen_1400();
    strlen_1500();
    strlen_1600();
    strlen_1700();
    strlen_1800();
    strlen_1900();
    strlen_2000();
    strlen_2100();
    strlen_2200();
    strlen_2300();
    strlen_2400();
    strlen_2500();
    strlen_2600();
    strlen_2700();
    strlen_2800();
    strlen_2900();
    strlen_3000();
    strlen_3100();
    strlen_3200();
    strlen_3300();
    strlen_3400();
    strlen_3500();
    strlen_3600();
    strlen_3700();
    strlen_3800();
    strlen_3900();
    strlen_4000();
    strlen_4100();
    strlen_4200();
    strlen_4300();
    strlen_4400();
    strlen_4500();
    strlen_4600();
    strlen_4700();
    strlen_4800();
    strlen_4900();
    strlen_5000();

    return t_status;
}
