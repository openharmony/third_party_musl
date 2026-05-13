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
 * @tc.name      : wcsrchr_0100
 * @tc.desc      : Character at beginning of string
 * @tc.level     : Level 0
 */
void wcsrchr_0100(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'h');
    if (result != ch) {
        t_error("%s wcsrchr get result is %p, not %p (expected 'h' at start)\n", 
                __func__, (void*)result, (void*)ch);
    }
}

/**
 * @tc.name      : wcsrchr_0200
 * @tc.desc      : Character at end of string
 * @tc.level     : Level 0
 */
void wcsrchr_0200(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'd');
    wchar_t *expected = ch + wcslen(ch) - 1;
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (expected 'd' at end)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_0300
 * @tc.desc      : Character in middle, multiple occurrences
 * @tc.level     : Level 0
 */
void wcsrchr_0300(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'l');
    wchar_t *expected = ch + 10;
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (expected last 'l')\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_0400
 * @tc.desc      : Character not found, should return NULL
 * @tc.level     : Level 0
 */
void wcsrchr_0400(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'x');
    if (result != NULL) {
        t_error("%s wcsrchr get result is %p, not NULL (character not found)\n", 
                __func__, (void*)result);
    }
}

/**
 * @tc.name      : wcsrchr_0500
 * @tc.desc      : Search for null terminator
 * @tc.level     : Level 1
 */
void wcsrchr_0500(void)
{
    wchar_t *ch = L"hello";
    wchar_t *result = wcsrchr(ch, L'\0');
    wchar_t *expected = ch + wcslen(ch);
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (expected null terminator)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_0600
 * @tc.desc      : Empty string, search for any character
 * @tc.level     : Level 1
 */
void wcsrchr_0600(void)
{
    wchar_t *ch = L"";
    wchar_t *result = wcsrchr(ch, L'a');
    if (result != NULL) {
        t_error("%s wcsrchr get result is %p, not NULL (empty string)\n", 
                __func__, (void*)result);
    }
}

/**
 * @tc.name      : wcsrchr_0700
 * @tc.desc      : Empty string, search for null terminator
 * @tc.level     : Level 1
 */
void wcsrchr_0700(void)
{
    wchar_t *ch = L"";
    wchar_t *result = wcsrchr(ch, L'\0');
    if (result != ch) {
        t_error("%s wcsrchr get result is %p, not %p (empty string null terminator)\n", 
                __func__, (void*)result, (void*)ch);
    }
}

/**
 * @tc.name      : wcsrchr_0800
 * @tc.desc      : Unicode character search
 * @tc.level     : Level 1
 */
void wcsrchr_0800(void)
{
    wchar_t *ch = L"中文测试中文";
    wchar_t *result = wcsrchr(ch, L'文');
    wchar_t *expected = ch + 3;
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (expected last Unicode char)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_0900
 * @tc.desc      : Search for control character
 * @tc.level     : Level 1
 */
void wcsrchr_0900(void)
{
    wchar_t ch[] = L"hello\nworld\t";
    wchar_t *result = wcsrchr(ch, L'\n');
    wchar_t *expected = ch + 5;
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (expected newline)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_1000
 * @tc.desc      : Single character string, search matches
 * @tc.level     : Level 1
 */
void wcsrchr_1000(void)
{
    wchar_t *ch = L"a";
    wchar_t *result = wcsrchr(ch, L'a');
    if (result != ch) {
        t_error("%s wcsrchr get result is %p, not %p (single char match)\n", 
                __func__, (void*)result, (void*)ch);
    }
}

/**
 * @tc.name      : wcsrchr_1100
 * @tc.desc      : Single character string, search doesn't match
 * @tc.level     : Level 1
 */
void wcsrchr_1100(void)
{
    wchar_t *ch = L"a";
    wchar_t *result = wcsrchr(ch, L'b');
    if (result != NULL) {
        t_error("%s wcsrchr get result is %p, not NULL (single char no match)\n", 
                __func__, (void*)result);
    }
}

/**
 * @tc.name      : wcsrchr_1200
 * @tc.desc      : Compare with wcschr (forward search)
 * @tc.level     : Level 2
 */
void wcsrchr_1200(void)
{
    wchar_t ch[] = L"multiple l characters";
    
    wchar_t *forward = wcschr(ch, L'l');
    wchar_t *backward = wcsrchr(ch, L'l');
    
    if (forward == backward) {
        t_error("%s wcschr (%p) should not equal wcsrchr (%p) for multiple occurrences\n", 
                __func__, (void*)forward, (void*)backward);
    }
    
    if (forward != ch + 2) {
        t_error("%s wcschr get result is %p, not %p\n", 
                __func__, (void*)forward, (void*)(ch + 2));
    }
    
    if (backward != ch + 20) {
        t_error("%s wcsrchr get result is %p, not %p\n", 
                __func__, (void*)backward, (void*)(ch + 20));
    }
}

/**
 * @tc.name      : wcsrchr_1300
 * @tc.desc      : String with embedded null characters
 * @tc.level     : Level 2
 */
void wcsrchr_1300(void)
{
    wchar_t ch[] = L"ab\0cd\0ef";
    wchar_t *result = wcsrchr(ch, L'c');
    if (result != NULL) {
        t_error("%s wcsrchr get result is %p, not NULL (string with embedded null)\n", 
                __func__, (void*)result);
    }
}

/**
 * @tc.name      : wcsrchr_1400
 * @tc.desc      : Search for character equal to 0 (null)
 * @tc.level     : Level 1
 */
void wcsrchr_1400(void)
{
    wchar_t *ch = L"hello";
    wchar_t zero = 0;
    wchar_t *result = wcsrchr(ch, zero);
    wchar_t *expected = ch + wcslen(ch);
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (search for zero)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_1500
 * @tc.desc      : Verify backward search from end
 * @tc.level     : Level 2
 */
void wcsrchr_1500(void)
{
    wchar_t ch[] = L"aabbcc";

    wchar_t *result1 = wcsrchr(ch, L'a');
    if (result1 != ch + 1) {
        t_error("%s wcsrchr for 'a' get result is %p, not %p\n", 
                __func__, (void*)result1, (void*)(ch + 1));
    }
    
    wchar_t *result2 = wcsrchr(ch, L'b');
    if (result2 != ch + 3) {
        t_error("%s wcsrchr for 'b' get result is %p, not %p\n", 
                __func__, (void*)result2, (void*)(ch + 3));
    }
}

/**
 * @tc.name      : wcsrchr_1600
 * @tc.desc      : Maximum wchar_t value search
 * @tc.level     : Level 2
 */
void wcsrchr_1600(void)
{
    wchar_t max_char = (wchar_t)-1;
    wchar_t ch[] = {L'a', max_char, L'b', L'\0'};
    
    wchar_t *result = wcsrchr(ch, max_char);
    if (result != ch + 1) {
        t_error("%s wcsrchr get result is %p, not %p (max wchar_t)\n", 
                __func__, (void*)result, (void*)(ch + 1));
    }
}

/**
 * @tc.name      : wcsrchr_1700
 * @tc.desc      : Very long string
 * @tc.level     : Level 2
 */
void wcsrchr_1700(void)
{
    wchar_t long_str[1000];
    for (int i = 0; i < 999; i++) {
        long_str[i] = L'A' + (i % 26);
    }
    long_str[999] = L'\0';

    long_str[998] = L'Z';
    long_str[500] = L'Z';
    
    wchar_t *result = wcsrchr(long_str, L'Z');
    wchar_t *expected = long_str + 998;
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (long string)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_1800
 * @tc.desc      : Verify wcslen is used for initialization
 * @tc.level     : Level 2
 */
void wcsrchr_1800(void)
{
    wchar_t ch[] = L"test";

    wchar_t *start = ch;
    wchar_t *end = ch + wcslen(ch);

    wchar_t *result = wcsrchr(ch, L't');
    wchar_t *expected = ch + 3;
    
    if (result != expected) {
        t_error("%s wcsrchr get result is %p, not %p (verify wcslen usage)\n", 
                __func__, (void*)result, (void*)expected);
    }
}

/**
 * @tc.name      : wcsrchr_1900
 * @tc.desc      : Character appears only once
 * @tc.level     : Level 1
 */
void wcsrchr_1900(void)
{
    wchar_t *ch = L"unique";
    wchar_t *result = wcsrchr(ch, L'u');
    if (result != ch) {
        t_error("%s wcsrchr get result is %p, not %p (single occurrence)\n", 
                __func__, (void*)result, (void*)ch);
    }
}

int main(int argc, char *argv[])
{
    wcsrchr_0100();
    wcsrchr_0200();
    wcsrchr_0300();
    wcsrchr_0400();
    wcsrchr_0500();
    wcsrchr_0600();
    wcsrchr_0700();
    wcsrchr_0800();
    wcsrchr_0900();
    wcsrchr_1000();
    wcsrchr_1100();
    wcsrchr_1200();
    wcsrchr_1300();
    wcsrchr_1400();
    return t_status;
}