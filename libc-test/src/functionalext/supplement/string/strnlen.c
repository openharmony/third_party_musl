/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : strnlen_0100
 * @tc.desc      : Test strnlen function to get string length
 * @tc.level     : Level 0
 */
void strnlen_0100(void)
{
    size_t want = 5;
    char *str = "123456";
    size_t result = strnlen(str, 5);
    if (result != want) {
        t_error("%s strnlen get result is %d are not 5\n", __func__, result);
    }
}

/**
 * @tc.name      : strnlen_0200
 * @tc.desc      : The incoming length is greater than the string length
 * @tc.level     : Level 1
 */
void strnlen_0200(void)
{
    size_t want = 6;
    char *str = "123456";
    size_t result = strnlen(str, 9);
    if (result != want) {
        t_error("%s strnlen get result is %d are not 6\n", __func__, result);
    }
}

/**
 * @tc.name      : strnlen_0300
 * @tc.desc      : Basic test, maxlen is less than the string length.
 * @tc.level     : Level 0
 */
void strnlen_0300(void)
{
    size_t want = 5;
    char *str = "123456";
    size_t result = strnlen(str, 5);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 5, result, want);
    }
}

/**
 * @tc.name      : strnlen_0400
 * @tc.desc      : Basic test, maxlen is greater than the string length.
 * @tc.level     : Level 0
 */
void strnlen_0400(void)
{
    size_t want = 6;
    char *str = "123456";
    size_t result = strnlen(str, 9);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 9, result, want);
    }
}

/**
 * @tc.name      : strnlen_0500
 * @tc.desc      : Basic test, maxlen is equal to the string length.
 * @tc.level     : Level 0
 */
void strnlen_0500(void)
{
    size_t want = 6;
    char *str = "123456";
    size_t result = strnlen(str, 6);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 6, result, want);
    }
}

/**
 * @tc.name      : strnlen_0600
 * @tc.desc      : Test with an empty string.
 * @tc.level     : Level 0
 */
void strnlen_0600(void)
{
    size_t want = 0;
    char *str = "";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_0700
 * @tc.desc      : Test with a string of length 1.
 * @tc.level     : Level 0
 */
void strnlen_0700(void)
{
    size_t want = 1;
    char *str = "a";
    size_t result = strnlen(str, 1);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 1, result, want);
    }
}

/**
 * @tc.name      : strnlen_0800
 * @tc.desc      : Test with a string of length 1 and maxlen greater.
 * @tc.level     : Level 0
 */
void strnlen_0800(void)
{
    size_t want = 1;
    char *str = "b";
    size_t result = strnlen(str, 5);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 5, result, want);
    }
}

/**
 * @tc.name      : strnlen_0900
 * @tc.desc      : Test with a string containing spaces.
 * @tc.level     : Level 0
 */
void strnlen_0900(void)
{
    size_t want = 11;
    char *str = "hello world";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_1000
 * @tc.desc      : Test with a string containing tabs and newlines.
 * @tc.level     : Level 0
 */
void strnlen_1000(void)
{
    size_t want = 13;
    char *str = "hello\tworld\n";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_1100
 * @tc.desc      : Test with a string containing special characters.
 * @tc.level     : Level 0
 */
void strnlen_1100(void)
{
    size_t want = 5;
    char *str = "!@#$%";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_1200
 * @tc.desc      : Test with a mixed alphanumeric string.
 * @tc.level     : Level 0
 */
void strnlen_1200(void)
{
    size_t want = 8;
    char *str = "a1b2c3d4";
    size_t result = strnlen(str, 8);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 8, result, want);
    }
}

/**
 * @tc.name      : strnlen_1300
 * @tc.desc      : Test with maxlen equal to 0.
 * @tc.level     : Level 1
 */
void strnlen_1300(void)
{
    size_t want = 0;
    char *str = "hello";
    size_t result = strnlen(str, 0);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 0, result, want);
    }
}

/**
 * @tc.name      : strnlen_1400
 * @tc.desc      : Test with maxlen equal to 1.
 * @tc.level     : Level 1
 */
void strnlen_1400(void)
{
    size_t want = 1;
    char *str = "test";
    size_t result = strnlen(str, 1);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", %zu) returned %zu, expected %zu\n", __func__, str, 1, result, want);
    }
}

/**
 * @tc.name      : strnlen_1500
 * @tc.desc      : Test with maxlen set to SIZE_MAX.
 * @tc.level     : Level 1
 */
void strnlen_1500(void)
{
    size_t want = 10;
    char *str = "0123456789";
    size_t result = strnlen(str, SIZE_MAX);
    if (result != want) {
        t_error("%s: strnlen(\"%s\", SIZE_MAX) returned %zu, expected %zu\n", __func__, str, result, want);
    }
}

/**
 * @tc.name      : strnlen_1600
 * @tc.desc      : Test where the null terminator is exactly at maxlen.
 * @tc.level     : Level 1
 */
void strnlen_1600(void)
{
    size_t want = 5;
    char *str = "12345\0abcde";
    size_t result = strnlen(str, 6);
    if (result != want) {
        t_error("%s: strnlen(\"12345\\\\0...\", %zu) returned %zu, expected %zu\n", __func__, 6, result, want);
    }
}

/**
 * @tc.name      : strnlen_1700
 * @tc.name      : strnlen_1700
 * @tc.desc      : Test with a long string, maxlen is less than the string length.
 * @tc.level     : Level 1
 */
void strnlen_1700(void)
{
    size_t want = 100;
    char str[256];
    memset(str, 'x', 255);
    str[255] = '\0';
    size_t result = strnlen(str, 100);
    if (result != want) {
        t_error("%s: strnlen(long_str, %zu) returned %zu, expected %zu\n", __func__, 100, result, want);
    }
}

/**
 * @tc.name      : strnlen_1800
 * @tc.desc      : Test with a long string, maxlen is greater than the string length.
 * @tc.level     : Level 1
 */
void strnlen_1800(void)
{
    size_t want = 255;
    char str[256];
    memset(str, 'y', 255);
    str[255] = '\0';
    size_t result = strnlen(str, 500);
    if (result != want) {
        t_error("%s: strnlen(long_str, %zu) returned %zu, expected %zu\n", __func__, 500, result, want);
    }
}

/**
 * @tc.name      : strnlen_1900
 * @tc.desc      : Test with a non-null-terminated char array, maxlen < array length.
 * @tc.level     : Level 1
 */
void strnlen_1900(void)
{
    size_t want = 3;
    char arr[] = {'a', 'b', 'c', 'd'};
    size_t result = strnlen(arr, 3);
    if (result != want) {
        t_error("%s: strnlen(arr, %zu) returned %zu, expected %zu\n", __func__, 3, result, want);
    }
}

/**
 * @tc.name      : strnlen_2000
 * @tc.desc      : Test with a non-null-terminated char array, maxlen == array length.
 * @tc.level     : Level 1
 */
void strnlen_2000(void)
{
    size_t want = 4;
    char arr[] = {'a', 'b', 'c', 'd'};
    size_t result = strnlen(arr, 4);
    if (result != want) {
        t_error("%s: strnlen(arr, %zu) returned %zu, expected %zu\n", __func__, 4, result, want);
    }
}

/**
 * @tc.name      : strnlen_2100
 * @tc.desc      : Test with a non-null-terminated char array, maxlen > array length.
 * @tc.level     : Level 1
 */
void strnlen_2100(void)
{
    size_t want = 4;
    char arr[] = {'a', 'b', 'c', 'd'};
    size_t result = strnlen(arr, 10);
    if (result != want) {
        t_error("%s: strnlen(arr, %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2200
 * @tc.desc      : Test with a string consisting of just a null terminator.
 * @tc.level     : Level 1
 */
void strnlen_2200(void)
{
    size_t want = 0;
    char *str = "\0";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"\\\\0\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2300
 * @tc.desc      : Test with a string starting with multiple null terminators.
 * @tc.level     : Level 1
 */
void strnlen_2300(void)
{
    size_t want = 0;
    char *str = "\0\0\0abc";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"\\\\0\\\\0...\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2400
 * @tc.desc      : Test with a string containing only null terminators.
 * @tc.level     : Level 1
 */
void strnlen_2400(void)
{
    size_t want = 0;
    char *str = "\0\0\0\0";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"\\\\0\\\\0...\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2500
 * @tc.desc      : Test with a string of all zeros (non-null).
 * @tc.level     : Level 1
 */
void strnlen_2500(void)
{
    size_t want = 5;
    char str[] = {'0', '0', '0', '0', '0', '\0'};
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"00000\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2600
 * @tc.desc      : Test with a string containing backslashes.
 * @tc.level     : Level 1
 */
void strnlen_2600(void)
{
    size_t want = 3;
    char *str = "a\\b";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a\\\\b\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2700
 * @tc.desc      : Test with a string containing double quotes.
 * @tc.level     : Level 1
 */
void strnlen_2700(void)
{
    size_t want = 5;
    char *str = "a\"bcd";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a\\\"bcd\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2800
 * @tc.desc      : Test with a string containing single quotes.
 * @tc.level     : Level 1
 */
void strnlen_2800(void)
{
    size_t want = 5;
    char *str = "a'bcd";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a'bcd\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_2900
 * @tc.desc      : Test with a string containing parentheses.
 * @tc.level     : Level 1
 */
void strnlen_2900(void)
{
    size_t want = 7;
    char *str = "(hello)";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"(hello)\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3000
 * @tc.desc      : Test with a string containing brackets.
 * @tc.level     : Level 1
 */
void strnlen_3000(void)
{
    size_t want = 7;
    char *str = "[world]";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"[world]\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3100
 * @tc.desc      : Test with a string containing braces.
 * @tc.level     : Level 1
 */
void strnlen_3100(void)
{
    size_t want = 7;
    char *str = "{foo}";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"{foo}\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3200
 * @tc.desc      : Test with a string containing angle brackets.
 * @tc.level     : Level 1
 */
void strnlen_3200(void)
{
    size_t want = 7;
    char *str = "<bar>";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"<bar>\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3300
 * @tc.desc      : Test with a string containing underscores.
 * @tc.level     : Level 1
 */
void strnlen_3300(void)
{
    size_t want = 9;
    char *str = "snake_case";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"snake_case\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3400
 * @tc.desc      : Test with a string containing hyphens.
 * @tc.level     : Level 1
 */
void strnlen_3400(void)
{
    size_t want = 11;
    char *str = "kebab-case";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"kebab-case\", %zu) returned %zu, expected %zu\n", __func__, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_3500
 * @tc.desc      : Test with a string containing plus signs.
 * @tc.level     : Level 1
 */
void strnlen_3500(void)
{
    size_t want = 5;
    char *str = "a+b+c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a+b+c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3600
 * @tc.desc      : Test with a string containing equals signs.
 * @tc.level     : Level 1
 */
void strnlen_3600(void)
{
    size_t want = 7;
    char *str = "key=value";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"key=value\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3700
 * @tc.desc      : Test with a string containing asterisks.
 * @tc.level     : Level 1
 */
void strnlen_3700(void)
{
    size_t want = 5;
    char *str = "a*b*c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a*b*c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_3800
 * @tc.desc      : Test with a string containing slashes.
 * @tc.level     : Level 1
 */
void strnlen_3800(void)
{
    size_t want = 9;
    char *str = "path/to/file";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"path/to/file\", %zu) returned %zu, expected %zu\n", __func__, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_3900
 * @tc.desc      : Test with a string containing backslashes (file path).
 * @tc.level     : Level 1
 */
void strnlen_3900(void)
{
    size_t want = 11;
    char *str = "C:\\Users\\test";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"C:\\\\Users\\\\test\", %zu) returned %zu, expected %zu\n", __func__, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_4000
 * @tc.desc      : Test with a string containing pipes.
 * @tc.level     : Level 1
 */
void strnlen_4000(void)
{
    size_t want = 7;
    char *str = "a|b|c|d";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a|b|c|d\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4100
 * @tc.desc      : Test with a string containing carets.
 * @tc.level     : Level 1
 */
void strnlen_4100(void)
{
    size_t want = 5;
    char *str = "a^b^c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a^b^c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4200
 * @tc.desc      : Test with a string containing tildes.
 * @tc.level     : Level 1
 */
void strnlen_4200(void)
{
    size_t want = 5;
    char *str = "a~b~c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a~b~c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4300
 * @tc.desc      : Test with a string containing colons.
 * @tc.level     : Level 1
 */
void strnlen_4300(void)
{
    size_t want = 5;
    char *str = "a:b:c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a:b:c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4400
 * @tc.desc      : Test with a string containing semicolons.
 * @tc.level     : Level 1
 */
void strnlen_4400(void)
{
    size_t want = 5;
    char *str = "a;b;c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a;b;c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4500
 * @tc.desc      : Test with a string containing commas.
 * @tc.level     : Level 1
 */
void strnlen_4500(void)
{
    size_t want = 7;
    char *str = "a,b,c,d";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a,b,c,d\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4600
 * @tc.desc      : Test with a string containing periods.
 * @tc.level     : Level 1
 */
void strnlen_4600(void)
{
    size_t want = 7;
    char *str = "a.b.c.d";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a.b.c.d\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4700
 * @tc.desc      : Test with a string containing exclamation marks.
 * @tc.level     : Level 1
 */
void strnlen_4700(void)
{
    size_t want = 7;
    char *str = "Hello!!!";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"Hello!!!\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4800
 * @tc.desc      : Test with a string containing question marks.
 * @tc.level     : Level 1
 */
void strnlen_4800(void)
{
    size_t want = 7;
    char *str = "What???";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"What???\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_4900
 * @tc.desc      : Test with a string containing at symbols.
 * @tc.level     : Level 1
 */
void strnlen_4900(void)
{
    size_t want = 11;
    char *str = "user@example.com";
    size_t result = strnlen(str, 20);
    if (result != want) {
        t_error("%s: strnlen(\"user@example.com\", %zu) returned %zu, expected %zu\n", __func__, 20, result, want);
    }
}

/**
 * @tc.name      : strnlen_5000
 * @tc.desc      : Test with a string containing hash symbols.
 * @tc.level     : Level 1
 */
void strnlen_5000(void)
{
    size_t want = 5;
    char *str = "a#b#c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a#b#c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_5100
 * @tc.desc      : Test with a string containing dollar signs.
 * @tc.level     : Level 1
 */
void strnlen_5100(void)
{
    size_t want = 5;
    char *str = "a$b$c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a$b$c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

/**
 * @tc.name      : strnlen_5200
 * @tc.desc      : Test with a string containing percent signs.
 * @tc.level     : Level 1
 */
void strnlen_5200(void)
{
    size_t want = 5;
    char *str = "a%b%c";
    size_t result = strnlen(str, 10);
    if (result != want) {
        t_error("%s: strnlen(\"a%%b%%c\", %zu) returned %zu, expected %zu\n", __func__, 10, result, want);
    }
}

int main(int argc, char *argv[])
{
    strnlen_0100();
    strnlen_0200();
    strnlen_0300();
    strnlen_0400();
    strnlen_0500();
    strnlen_0600();
    strnlen_0700();
    strnlen_0800();
    strnlen_0900();
    strnlen_1000();
    strnlen_1100();
    strnlen_1200();
    strnlen_1300();
    strnlen_1400();
    strnlen_1500();
    strnlen_1600();
    strnlen_1700();
    strnlen_1800();
    strnlen_1900();
    strnlen_2000();
    strnlen_2100();
    strnlen_2200();
    strnlen_2300();
    strnlen_2400();
    strnlen_2500();
    strnlen_2600();
    strnlen_2700();
    strnlen_2800();
    strnlen_2900();
    strnlen_3000();
    strnlen_3100();
    strnlen_3200();
    strnlen_3300();
    strnlen_3400();
    strnlen_3500();
    strnlen_3600();
    strnlen_3700();
    strnlen_3800();
    strnlen_3900();
    strnlen_4000();
    strnlen_4100();
    strnlen_4200();
    strnlen_4300();
    strnlen_4400();
    strnlen_4500();
    strnlen_4600();
    strnlen_4700();
    strnlen_4800();
    strnlen_4900();
    strnlen_5000();
    strnlen_5100();
    strnlen_5200();

    return t_status;
}