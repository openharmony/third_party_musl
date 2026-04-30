/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcscspn_0100
 * @tc.desc      : All characters in s are in reject set, should return 0
 * @tc.level     : Level 0
 */
void wcscspn_0100(void)
{
    size_t result = wcscspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    if (result != 0U) {
        t_error("%s wcscspn get result is %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0200
 * @tc.desc      : Reject set contains space, should return length before first space
 * @tc.level     : Level 1
 */
void wcscspn_0200(void)
{
    size_t result = wcscspn(L"hello world", L" ");
    size_t want = 5U;  // "hello" = 5 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0300
 * @tc.desc      : Reject character not in s, should return full length
 * @tc.level     : Level 1
 */
void wcscspn_0300(void)
{
    size_t result = wcscspn(L"hello world", L"!");
    size_t want = 11U;  // Full length
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0400
 * @tc.desc      : Empty reject string, should return full length
 * @tc.level     : Level 1
 */
void wcscspn_0400(void)
{
    size_t result = wcscspn(L"hello world", L"");
    size_t want = 11U;  // Full length when reject is empty
    if (result != want) {
        t_error("%s wcscspn with empty reject get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0500
 * @tc.desc      : Single reject character at start of s
 * @tc.level     : Level 1
 */
void wcscspn_0500(void)
{
    size_t result = wcscspn(L"hello", L"h");
    if (result != 0U) {
        t_error("%s wcscspn get result is %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0600
 * @tc.desc      : Single reject character in middle of s
 * @tc.level     : Level 1
 */
void wcscspn_0600(void)
{
    size_t result = wcscspn(L"hello world", L"w");
    size_t want = 6U;  // "hello " = 6 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0700
 * @tc.desc      : Multiple reject characters, first match is '.'
 * @tc.level     : Level 0
 */
void wcscspn_0700(void)
{
    size_t result = wcscspn(L"123.456-789", L".-");
    size_t want = 3U;  // "123" = 3 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0800
 * @tc.desc      : Test with wide characters (Unicode)
 * @tc.level     : Level 1
 */
void wcscspn_0800(void)
{
    size_t result = wcscspn(L"中文测试English", L"E");
    size_t want = 4U;  // "中文测试" = 4 wide characters
    if (result != want) {
        t_error("%s wcscspn with wide chars get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_0900
 * @tc.desc      : Empty source string
 * @tc.level     : Level 1
 */
void wcscspn_0900(void)
{
    size_t result = wcscspn(L"", L"abc");
    if (result != 0U) {
        t_error("%s wcscspn with empty source get %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1000
 * @tc.desc      : Both source and reject are empty
 * @tc.level     : Level 1
 */
void wcscspn_1000(void)
{
    size_t result = wcscspn(L"", L"");
    if (result != 0U) {
        t_error("%s wcscspn with both empty get %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1100
 * @tc.desc      : Reject character at end of string
 * @tc.level     : Level 1
 */
void wcscspn_1100(void)
{
    size_t result = wcscspn(L"hello", L"o");
    size_t want = 4U;  // "hell" = 4 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1200
 * @tc.desc      : All characters in reject set (different from 0100)
 * @tc.level     : Level 0
 */
void wcscspn_1200(void)
{
    size_t result = wcscspn(L"abcde", L"abcde");
    if (result != 0U) {
        t_error("%s wcscspn get result is %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1300
 * @tc.desc      : Overlapping characters in reject, first char in reject
 * @tc.level     : Level 2
 */
void wcscspn_1300(void)
{
    size_t result = wcscspn(L"test string", L"st ");
    if (result != 0U) {  // 't' is in reject set at position 0
        t_error("%s wcscspn get result is %zu, not 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1400
 * @tc.desc      : Control characters in reject
 * @tc.level     : Level 2
 */
void wcscspn_1400(void)
{
    wchar_t str[] = L"line1\nline2\tend";
    size_t result = wcscspn(str, L"\n\t");
    size_t want = 5U;  // "line1" = 5 characters
    if (result != want) {
        t_error("%s wcscspn with control chars get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1500
 * @tc.desc      : Numeric reject characters
 * @tc.level     : Level 1
 */
void wcscspn_1500(void)
{
    size_t result = wcscspn(L"item123 price45", L"0123456789");
    size_t want = 4U;  // "item" = 4 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1600
 * @tc.desc      : Test the single-character optimization path
 * @tc.level     : Level 2
 */
void wcscspn_1600(void)
{
    // Tests the "if (!c[1])" branch
    size_t result = wcscspn(L"hello world", L"d");
    size_t want = 10U;  // "hello worl" = 10 characters
    if (result != want) {
        t_error("%s wcscspn single-char optimization get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1700
 * @tc.desc      : Test the empty reject optimization path
 * @tc.level     : Level 2
 */
void wcscspn_1700(void)
{
    // Tests the "if (!c[0])" branch
    size_t result = wcscspn(L"test string", L"");
    size_t want = 11U;  // Full length
    if (result != want) {
        t_error("%s wcscspn empty reject optimization get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1800
 * @tc.desc      : String contains null character in middle
 * @tc.level     : Level 2
 */
void wcscspn_1800(void)
{
    wchar_t str[] = L"hello\0world";  // Embedded null
    size_t result = wcscspn(str, L"w");
    // Should stop at null terminator, not at 'w'
    size_t want = 5U;  // "hello" = 5 characters
    if (result != want) {
        t_error("%s wcscspn with null char get %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_1900
 * @tc.desc      : Test with punctuation characters
 * @tc.level     : Level 1
 */
void wcscspn_1900(void)
{
    size_t result = wcscspn(L"name@domain.com", L"@.");
    size_t want = 4U;  // "name" = 4 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcscspn_2000
 * @tc.desc      : Long string with late reject match
 * @tc.level     : Level 1
 */
void wcscspn_2000(void)
{
    wchar_t long_str[] = L"aaaaaaaaaaaaaaaaaaaaXbbbbbbbbbb";
    size_t result = wcscspn(long_str, L"X");
    size_t want = 20U;  // 20 'a' characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

int main(int argc, char *argv[])
{
    wcscspn_0100();
    wcscspn_0200();
    wcscspn_0300();
    wcscspn_0400();
    wcscspn_0500();
    wcscspn_0600();
    wcscspn_0700();
    wcscspn_0800();
    wcscspn_0900();
    wcscspn_1000();
    wcscspn_1100();
    wcscspn_1200();
    wcscspn_1300();
    wcscspn_1400();
    wcscspn_1500();
    wcscspn_1600();
    wcscspn_1700();
    wcscspn_1800();
    wcscspn_1900();
    wcscspn_2000();

    return t_status;
}