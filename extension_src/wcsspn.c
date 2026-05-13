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
 * @tc.name      : wcsspn_0100
 * @tc.desc      : All characters in accept set
 * @tc.level     : Level 0
 */
void wcsspn_0100(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz ");
    size_t want = 11U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0200
 * @tc.desc      : First character not in accept set
 * @tc.level     : Level 0
 */
void wcsspn_0200(void)
{
    size_t result = wcsspn(L"hello world", L"xyz");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0300
 * @tc.desc      : Partial match, first 5 characters in accept
 * @tc.level     : Level 0
 */
void wcsspn_0300(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0400
 * @tc.desc      : Empty string
 * @tc.level     : Level 1
 */
void wcsspn_0400(void)
{
    size_t result = wcsspn(L"", L"abc");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0500
 * @tc.desc      : Empty accept set
 * @tc.level     : Level 1
 */
void wcsspn_0500(void)
{
    size_t result = wcsspn(L"hello", L"");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0600
 * @tc.desc      : Empty string and empty accept set
 * @tc.level     : Level 1
 */
void wcsspn_0600(void)
{
    size_t result = wcsspn(L"", L"");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0700
 * @tc.desc      : Single character accept set
 * @tc.level     : Level 1
 */
void wcsspn_0700(void)
{
    size_t result = wcsspn(L"aaaaab", L"a");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0800
 * @tc.desc      : Unicode characters in accept set
 * @tc.level     : Level 1
 */
void wcsspn_0800(void)
{
    size_t result = wcsspn(L"中文English混合", L"中");
    size_t want = 1U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_0900
 * @tc.desc      : String starts with null character
 * @tc.level     : Level 2
 */
void wcsspn_0900(void)
{
    wchar_t s[] = L"\0hello";
    size_t result = wcsspn(s, L"abc");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1000
 * @tc.desc      : Accept set contains null character
 * @tc.level     : Level 2
 */
void wcsspn_1000(void)
{
    wchar_t accept[] = L"a\0b\0c";
    size_t result = wcsspn(L"aab", accept);
    size_t want = 2U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1100
 * @tc.desc      : Digits accept set
 * @tc.level     : Level 1
 */
void wcsspn_1100(void)
{
    size_t result = wcsspn(L"123abc456", L"0123456789");
    size_t want = 3U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1200
 * @tc.desc      : Whitespace accept set
 * @tc.level     : Level 1
 */
void wcsspn_1200(void)
{
    size_t result = wcsspn(L"  \t\nhello", L" \t\n");
    size_t want = 3U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1300
 * @tc.desc      : Control characters in accept
 * @tc.level     : Level 2
 */
void wcsspn_1300(void)
{
    wchar_t s[] = L"\x01\x02\x03ABC";
    size_t result = wcsspn(s, L"\x01\x02\x03");
    size_t want = 3U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1400
 * @tc.desc      : Verify wcschr is called correctly
 * @tc.level     : Level 2
 */
void wcsspn_1400(void)
{
    size_t result1 = wcsspn(L"abcde", L"abc");
    if (result1 != 3) {
        t_error("%s wcsspn get result is %zu, not 3\n", __func__, result1);
    }
    
    size_t result2 = wcsspn(L"abcde", L"abcdefgh");
    if (result2 != 5) {
        t_error("%s wcsspn get result is %zu, not 5\n", __func__, result2);
    }
}

/**
 * @tc.name      : wcsspn_1500
 * @tc.desc      : Long string with all characters in accept
 * @tc.level     : Level 2
 */
void wcsspn_1500(void)
{
    wchar_t long_str[1000];
    for (int i = 0; i < 999; i++) {
        long_str[i] = L'a' + (i % 26);
    }
    long_str[999] = L'\0';
    
    wchar_t accept[27];
    for (int i = 0; i < 26; i++) {
        accept[i] = L'a' + i;
    }
    accept[26] = L'\0';
    
    size_t result = wcsspn(long_str, accept);
    if (result != 999) {
        t_error("%s wcsspn get result is %zu, not 999\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1600
 * @tc.desc      : String contains embedded null
 * @tc.level     : Level 2
 */
void wcsspn_1600(void)
{
    wchar_t s[] = L"ab\0cd";
    size_t result = wcsspn(s, L"ab");
    size_t want = 2U;
    if (result != want) {
        t_error("%s wcsspn get result is %zu, not %zu\n", __func__, result, want);
    }
}

/**
 * @tc.name      : wcsspn_1700
 * @tc.desc      : Compare with wcscspn (inverse)
 * @tc.level     : Level 2
 */
void wcsspn_1700(void)
{
    wchar_t *s = L"abc123def";
    wchar_t *accept = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t *reject = L"0123456789";
    
    size_t spn_result = wcsspn(s, accept);
    size_t cspn_result = wcscspn(s, reject);
    
    if (spn_result != 3) {
        t_error("%s wcsspn get result is %zu, not 3\n", __func__, spn_result);
    }
    if (cspn_result != 3) {
        t_error("%s wcscspn get result is %zu, not 3\n", __func__, cspn_result);
    }
    if (spn_result != cspn_result) {
        t_error("%s wcsspn (%zu) != wcscspn (%zu)\n", __func__, spn_result, cspn_result);
    }
}

/**
 * @tc.name      : wcsspn_1800
 * @tc.desc      : Maximum wchar_t value in accept
 * @tc.level     : Level 2
 */
void wcsspn_1800(void)
{
    wchar_t max_char = (wchar_t)-1;
    wchar_t s[] = {max_char, L'a', L'b', L'\0'};
    wchar_t accept[] = {max_char, L'\0'};
    
    size_t result = wcsspn(s, accept);
    if (result != 1) {
        t_error("%s wcsspn get result is %zu, not 1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsspn_0100();
    wcsspn_0200();
    wcsspn_0300();
    wcsspn_0400();
    wcsspn_0500();
    wcsspn_0600();
    wcsspn_0700();
    wcsspn_0800();
    wcsspn_0900();
    wcsspn_1000();
    wcsspn_1100();
    wcsspn_1200();
    wcsspn_1300();
    wcsspn_1400();
    wcsspn_1500();
    wcsspn_1600();
    wcsspn_1700();
    wcsspn_1800();
    return t_status;
}