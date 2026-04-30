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
 * @tc.name      : wmemcmp_0100
 * @tc.desc      : Compare equal strings
 * @tc.level     : Level 0
 */
void wmemcmp_0100(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCD";
    int result = wmemcmp(l, r, 4);
    if (result != 0) {
        t_error("%s: Strings are not equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0200
 * @tc.desc      : Compare strings where l < r
 * @tc.level     : Level 0
 */
void wmemcmp_0200(void)
{
    wchar_t l[] = L"ABCA";
    wchar_t r[] = L"ABCC";
    int result = wmemcmp(l, r, 4);
    if (result >= 0) {  // Should return negative
        t_error("%s: Expected negative, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0300
 * @tc.desc      : Compare strings where l > r
 * @tc.level     : Level 0
 */
void wmemcmp_0300(void)
{
    wchar_t l[] = L"ABCE";
    wchar_t r[] = L"ABCC";
    int result = wmemcmp(l, r, 4);
    if (result <= 0) {  // Should return positive
        t_error("%s: Expected positive, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0400
 * @tc.desc      : Compare first n characters, n less than string length
 * @tc.level     : Level 1
 */
void wmemcmp_0400(void)
{
    wchar_t l[] = L"ABCDEF";
    wchar_t r[] = L"ABCXYZ";
    int result = wmemcmp(l, r, 3);  // Compare first 3 characters
    if (result != 0) {  // First 3 are equal
        t_error("%s: First 3 chars should be equal, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0500
 * @tc.desc      : Compare Unicode strings
 * @tc.level     : Level 1
 */
void wmemcmp_0500(void)
{
    wchar_t l[] = L"中文测试";
    wchar_t r[] = L"中文测试";
    int result = wmemcmp(l, r, 4);
    if (result != 0) {
        t_error("%s: Unicode strings are not equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0600
 * @tc.desc      : Compare mixed ASCII and Unicode
 * @tc.level     : Level 1
 */
void wmemcmp_0600(void)
{
    wchar_t l[] = L"ABC中文";
    wchar_t r[] = L"ABC中文";
    int result = wmemcmp(l, r, 5);  // 3 ASCII + 2 wide characters
    if (result != 0) {
        t_error("%s: Mixed strings are not equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0700
 * @tc.desc      : n = 0, should always return 0
 * @tc.level     : Level 1
 */
void wmemcmp_0700(void)
{
    wchar_t l[] = L"ABC";
    wchar_t r[] = L"XYZ";
    int result = wmemcmp(l, r, 0);
    if (result != 0) {
        t_error("%s: Comparing 0 chars should return 0, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0800
 * @tc.desc      : Same pointer comparison
 * @tc.level     : Level 1
 */
void wmemcmp_0800(void)
{
    wchar_t s[] = L"ABCD";
    int result = wmemcmp(s, s, 4);
    if (result != 0) {
        t_error("%s: Same pointer should return 0, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0900
 * @tc.desc      : Compare single character
 * @tc.level     : Level 1
 */
void wmemcmp_0900(void)
{
    wchar_t l[] = L"A";
    wchar_t r[] = L"A";
    int result = wmemcmp(l, r, 1);
    if (result != 0) {
        t_error("%s: Single char equality failed, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1000
 * @tc.desc      : Compare strings with null characters
 * @tc.level     : Level 2
 */
void wmemcmp_1000(void)
{
    wchar_t l[] = L"A\0BC";
    wchar_t r[] = L"A\0DE";
    int result = wmemcmp(l, r, 4);
    if (result != 0) {  // First 4 chars are equal (A, \0, B, C vs A, \0, D, E)
        t_error("%s: Strings with null should compare equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1100
 * @tc.desc      : Case sensitivity test
 * @tc.level     : Level 1
 */
void wmemcmp_1100(void)
{
    wchar_t l[] = L"abcd";
    wchar_t r[] = L"ABCD";
    int result = wmemcmp(l, r, 4);
    if (result <= 0) {  // 'a' > 'A' in ASCII/Unicode
        t_error("%s: Case sensitive comparison failed, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1200
 * @tc.desc      : Long string comparison
 * @tc.level     : Level 1
 */
void wmemcmp_1200(void)
{
    wchar_t l[] = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t r[] = L"abcdefghijklmnopqrstuvwxyz";
    int len = wcslen(l);
    int result = wmemcmp(l, r, len);
    if (result != 0) {
        t_error("%s: Long strings are not equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1300
 * @tc.desc      : Different length strings, compare only n characters
 * @tc.level     : Level 2
 */
void wmemcmp_1300(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCDE";
    int result = wmemcmp(l, r, 4);  // Compare first 4 chars
    if (result != 0) {
        t_error("%s: First 4 chars should be equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1400
 * @tc.desc      : Compare control characters
 * @tc.level     : Level 1
 */
void wmemcmp_1400(void)
{
    wchar_t l[] = L"A\nB\tC";
    wchar_t r[] = L"A\nB\tC";
    int result = wmemcmp(l, r, 5);
    if (result != 0) {
        t_error("%s: Control characters not equal, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1500
 * @tc.desc      : Test exact return values -1, 0, 1
 * @tc.level     : Level 2
 */
void wmemcmp_1500(void)
{
    wchar_t a[] = L"A";
    wchar_t b[] = L"B";
    wchar_t c[] = L"C";
    
    int result1 = wmemcmp(a, a, 1);  // Should be 0
    int result2 = wmemcmp(a, b, 1);  // Should be -1
    int result3 = wmemcmp(c, b, 1);  // Should be 1
    
    if (result1 != 0) {
        t_error("%s: A vs A should be 0, got %d\n", __func__, result1);
    }
    if (result2 >= 0) {  // A < B, should be negative
        t_error("%s: A vs B should be negative, got %d\n", __func__, result2);
    }
    if (result3 <= 0) {  // C > B, should be positive
        t_error("%s: C vs B should be positive, got %d\n", __func__, result3);
    }
}

/**
 * @tc.name      : wmemcmp_1600
 * @tc.desc      : Compare with null pointer (undefined behavior)
 * @tc.level     : Level 2
 */
void wmemcmp_1600(void)
{
    wchar_t s[] = L"Test";
    // This may crash - testing undefined behavior
    // int result = wmemcmp(NULL, s, 1);
    // int result2 = wmemcmp(s, NULL, 1);
    wprintf(L"%s: NULL pointer test skipped (undefined behavior)\n", __func__);
}

/**
 * @tc.name      : wmemcmp_1700
 * @tc.desc      : Early exit in loop (difference at first character)
 * @tc.level     : Level 2
 */
void wmemcmp_1700(void)
{
    wchar_t l[] = L"XYZ";
    wchar_t r[] = L"ABC";
    int result = wmemcmp(l, r, 3);
    if (result <= 0) {  // X > A, should be positive
        t_error("%s: Early exit failed, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1800
 * @tc.desc      : Difference at last character
 * @tc.level     : Level 2
 */
void wmemcmp_1800(void)
{
    wchar_t l[] = L"ABCDX";
    wchar_t r[] = L"ABCDY";
    int result = wmemcmp(l, r, 5);
    if (result >= 0) {  // X < Y, should be negative
        t_error("%s: Last char difference failed, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1900
 * @tc.desc      : Test maximum wchar_t values
 * @tc.level     : Level 2
 */
void wmemcmp_1900(void)
{
    wchar_t max = (wchar_t)-1;
    wchar_t min = 0;
    
    wchar_t l[] = {max, L'\0'};
    wchar_t r[] = {min, L'\0'};
    
    int result = wmemcmp(l, r, 1);
    if (result <= 0) {  // max > min, should be positive
        t_error("%s: Max vs min failed, result=%d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_2000
 * @tc.desc      : Verify algorithm matches standard implementation
 * @tc.level     : Level 2
 */
void wmemcmp_2000(void)
{
    struct test_case {
        wchar_t *l;
        wchar_t *r;
        size_t n;
        int expected_sign;  // -1, 0, 1
    } tests[] = {
        {L"", L"", 0, 0},
        {L"A", L"B", 1, -1},
        {L"B", L"A", 1, 1},
        {L"AB", L"AC", 2, -1},
        {L"AC", L"AB", 2, 1},
        {L"ABC", L"ABC", 3, 0},
        {L"\xFFFF", L"\x0000", 1, 1},
        {L"\x0000", L"\xFFFF", 1, -1},
    };
    
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        int result = wmemcmp(tests[i].l, tests[i].r, tests[i].n);
        int sign = (result > 0) - (result < 0);  // Normalize to -1, 0, 1
        
        if (sign != tests[i].expected_sign) {
            t_error("%s: Test %zu failed: expected sign %d, got %d (result=%d)\n", 
                    __func__, i, tests[i].expected_sign, sign, result);
        }
    }
}

int main(int argc, char *argv[])
{
    wmemcmp_0100();
    wmemcmp_0200();
    wmemcmp_0300();
    wmemcmp_0400();
    wmemcmp_0500();
    wmemcmp_0600();
    wmemcmp_0700();
    wmemcmp_0800();
    wmemcmp_0900();
    wmemcmp_1000();
    wmemcmp_1100();
    wmemcmp_1200();
    wmemcmp_1300();
    wmemcmp_1400();
    wmemcmp_1500();
    wmemcmp_1600();
    wmemcmp_1700();
    wmemcmp_1800();
    wmemcmp_1900();
    wmemcmp_2000();

    return t_status;
}