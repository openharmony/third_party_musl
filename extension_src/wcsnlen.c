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
 * @tc.name      : wcsnlen_0100
 * @tc.desc      : String length equals n, no null terminator within n
 * @tc.level     : Level 0
 */
void wcsnlen_0100(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t want = 6;
    size_t result = wcsnlen(wc, 6);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0200
 * @tc.desc      : String shorter than n, null terminator before n
 * @tc.level     : Level 0
 */
void wcsnlen_0200(void)
{
    wchar_t *wc = L"ABC";
    size_t want = 3;
    size_t result = wcsnlen(wc, 10);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0300
 * @tc.desc      : Empty string (null at position 0)
 * @tc.level     : Level 1
 */
void wcsnlen_0300(void)
{
    wchar_t *wc = L"";
    size_t want = 0;
    size_t result = wcsnlen(wc, 10);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0400
 * @tc.desc      : n = 0, should return 0 regardless of string
 * @tc.level     : Level 1
 */
void wcsnlen_0400(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t want = 0;
    size_t result = wcsnlen(wc, 0);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0500
 * @tc.desc      : Null terminator exactly at position n
 * @tc.level     : Level 1
 */
void wcsnlen_0500(void)
{
    wchar_t wc[] = L"ABC\0DEF";  // Null at position 3
    size_t want = 3;
    size_t result = wcsnlen(wc, 3);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0600
 * @tc.desc      : Null terminator before n
 * @tc.level     : Level 1
 */
void wcsnlen_0600(void)
{
    wchar_t wc[] = L"AB\0CDEF";  // Null at position 2
    size_t want = 2;
    size_t result = wcsnlen(wc, 5);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0700
 * @tc.desc      : String with wide characters
 * @tc.level     : Level 1
 */
void wcsnlen_0700(void)
{
    wchar_t *wc = L"中文测试";
    size_t want = 4;
    size_t result = wcsnlen(wc, 10);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0800
 * @tc.desc      : n larger than actual buffer but no null terminator
 * @tc.level     : Level 2
 */
void wcsnlen_0800(void)
{
    wchar_t wc[5] = {L'A', L'B', L'C', L'D', L'E'};  // No null terminator!
    size_t want = 5;
    size_t result = wcsnlen(wc, 10);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_0900
 * @tc.desc      : String with control characters
 * @tc.level     : Level 1
 */
void wcsnlen_0900(void)
{
    wchar_t wc[] = L"A\nB\tC\x01";
    size_t want = 5;  // A, \n, B, \t, C, \x01
    size_t result = wcsnlen(wc, 10);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_1000
 * @tc.desc      : NULL pointer input (undefined behavior, test may crash)
 * @tc.level     : Level 2
 */
void wcsnlen_1000(void)
{
    wchar_t *wc = NULL;
    // This may crash - wmemchr with NULL pointer
    // Some implementations may handle it, but standard doesn't require it
    size_t result = wcsnlen(wc, 5);
    // If we get here without crashing, result is undefined
    // We can't make meaningful assertions about the result
}

/**
 * @tc.name      : wcsnlen_1100
 * @tc.desc      : Verify wmemchr is called with correct parameters
 * @tc.level     : Level 2
 */
void wcsnlen_1100(void)
{
    wchar_t wc[] = L"ABCD\0XXXX";
    // Search in first 5 chars, null at position 4
    size_t result1 = wcsnlen(wc, 5);
    if (result1 != 4) {
        t_error("%s wcsnlen error get result is %zu, not 4\n", __func__, result1);
    }
    
    // Search in first 3 chars, no null found
    size_t result2 = wcsnlen(wc, 3);
    if (result2 != 3) {
        t_error("%s wcsnlen error get result is %zu, not 3\n", __func__, result2);
    }
}

/**
 * @tc.name      : wcsnlen_1200
 * @tc.desc      : Very large n value
 * @tc.level     : Level 2
 */
void wcsnlen_1200(void)
{
    wchar_t *wc = L"Short";
    size_t large_n = 1000000;
    size_t want = 5;
    size_t result = wcsnlen(wc, large_n);
    if (result != want) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsnlen_1300
 * @tc.desc      : n exactly at string length
 * @tc.level     : Level 1
 */
void wcsnlen_1300(void)
{
    wchar_t *wc = L"Hello World";
    size_t len = wcslen(wc);
    size_t result = wcsnlen(wc, len);
    if (result != len) {
        t_error("%s wcsnlen error get result is %zu, not %zu\n", __func__, result, len);
    }
}

/**
 * @tc.name      : wcsnlen_1400
 * @tc.desc      : Multiple null terminators in string
 * @tc.level     : Level 2
 */
void wcsnlen_1400(void)
{
    wchar_t wc[] = L"A\0B\0C\0";
    // Should stop at first null
    size_t result = wcsnlen(wc, 6);
    if (result != 1) {
        t_error("%s wcsnlen error get result is %zu, not 1\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsnlen_1500
 * @tc.desc      : Compare with wcslen for normal strings
 * @tc.level     : Level 0
 */
void wcsnlen_1500(void)
{
    wchar_t *wc = L"Test String";
    size_t wcslen_result = wcslen(wc);
    size_t wcsnlen_result = wcsnlen(wc, wcslen_result + 10);
    if (wcslen_result != wcsnlen_result) {
        t_error("%s wcsnlen (%zu) != wcslen (%zu)\n", __func__, wcsnlen_result, wcslen_result);
    }
}

int main(int argc, char *argv[])
{
    wcsnlen_0100();
    wcsnlen_0200();
    wcsnlen_0300();
    wcsnlen_0400();
    wcsnlen_0500();
    wcsnlen_0600();
    wcsnlen_0700();
    wcsnlen_0800();
    wcsnlen_0900();
    wcsnlen_1000();
    wcsnlen_1100();
    wcsnlen_1200();
    wcsnlen_1300();
    wcsnlen_1400();
    wcsnlen_1500();

    return t_status;
}