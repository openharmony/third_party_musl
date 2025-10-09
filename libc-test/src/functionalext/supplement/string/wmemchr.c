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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemchr_0100
 * @tc.desc      : Find the first occurrence of a wide character in the target array
 * @tc.level     : Level 0
 */
void wmemchr_0100(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'C';
    size_t sz = sizeof(str) / sizeof(*str);
    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s not found target ch\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0200
 * @tc.desc      : Test the return value of the function when there are no wide characters in the destination array
 * @tc.level     : Level 1
 */
void wmemchr_0200(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'H';
    size_t sz = sizeof(str) / sizeof(*str);
    wchar_t *result = wmemchr(str, target, sz);
    if (result) {
        t_error("%s found target ch\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0300
 * @tc.desc      : Test the return value of the function when n=0
 * @tc.level     : Level 1
 */
void wmemchr_0300(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'C';
    size_t sz = 0;
    wchar_t *result = wmemchr(str, target, sz);
    if (result) {
        t_error("%s found target ch", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0400
 * @tc.desc      : Find wide character at the beginning of the array
 * @tc.level     : Level 0
 */
void wmemchr_0400(void)
{
    wchar_t str[] = L"Hello World";
    wchar_t target = L'H';
    size_t sz = wcslen(str);
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Target character 'H' not found at beginning\n", __func__);
        return;
    }
    if (result != str) {
        t_error("%s: Expected pointer to beginning of string\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0500
 * @tc.desc      : Find wide character at the end of the array
 * @tc.level     : Level 0
 */
void wmemchr_0500(void)
{
    wchar_t str[] = L"Hello World";
    wchar_t target = L'd';
    size_t sz = wcslen(str);
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Target character 'd' not found at end\n", __func__);
        return;
    }
    if (result != str + 10) { // 'd' is at position 10 in "Hello World"
        t_error("%s: Expected pointer to end of string\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0600
 * @tc.desc      : Test with multiple occurrences of the same character
 * @tc.level     : Level 1
 */
void wmemchr_0600(void)
{
    wchar_t str[] = L"abacaba";
    wchar_t target = L'a';
    size_t sz = wcslen(str);
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Character 'a' not found\n", __func__);
        return;
    }
    // Should return the first occurrence
    if (result != str) {
        t_error("%s: Expected first occurrence of 'a'\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0700
 * @tc.desc      : Test with null character in the middle of the array
 * @tc.level     : Level 2
 */
void wmemchr_0700(void)
{
    wchar_t str[] = L"Hello\0World";
    size_t sz = 11; // Include the null character and beyond
    wchar_t target = L'W';
    wchar_t *result = wmemchr(str, target, sz);

    // wmemchr continues searching past null characters
    if (!result) {
        t_error("%s: Character 'W' not found after null character\n", __func__);
        return;
    }
    if (result != str + 6) { // 'W' is at position 6 (after null)
        t_error("%s: Wrong position after null character\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0800
 * @tc.desc      : Test searching for null character
 * @tc.level     : Level 1
 */
void wmemchr_0800(void)
{
    wchar_t str[] = L"Hello";
    wchar_t target = L'\0';
    size_t sz = wcslen(str) + 1; // Include the null terminator
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Null terminator not found\n", __func__);
        return;
    }
    if (result != str + 5) {
        t_error("%s: Null terminator at wrong position\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0900
 * @tc.desc      : Test searching for null character
 * @tc.level     : Level 1
 */
void wmemchr_0900(void)
{
    wchar_t str[] = L"Hello";
    wchar_t target = L'\0';
    size_t sz = wcslen(str) + 1; // Include the null terminator
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Null terminator not found\n", __func__);
        return;
    }
    if (result != str + 5) {
        t_error("%s: Null terminator at wrong position\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1000
 * @tc.desc      : Test with wide character array containing various character values
 * @tc.level     : Level 2
 */
void wmemchr_1000(void)
{
    wchar_t str[] = {0x41, 0x1234, 0x5678, 0x9ABC, 0x0};
    wchar_t target = 0x5678;
    size_t sz = 4; // All characters including the null

    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s: Wide character 0x5678 not found\n", __func__);
        return;
    }
    if (result != str + 2) {
        t_error("%s: Wide character at wrong position\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1100
 * @tc.desc      : Test with very large wide character values
 * @tc.level     : Level 2
 */
void wmemchr_1100(void)
{
    wchar_t str[] = {L'A', 0xFFFF, 0x7FFF, L'Z', L'\0'};
    wchar_t target = 0x7FFF;
    size_t sz = 4;

    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s: Large wide character 0x7FFF not found\n", __func__);
        return;
    }
    if (result != str + 2) {
        t_error("%s: Large wide character at wrong position\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1200
 * @tc.desc      : Test with negative wide character values (if supported)
 * @tc.level     : Level 2
 */
void wmemchr_1200(void)
{
    // Note: wchar_t is usually unsigned, but test with signed interpretation
    wchar_t str[] = {L'A', L'B', (wchar_t)-100, L'D', L'\0'};
    wchar_t target = (wchar_t)-100;
    size_t sz = 4;

    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s: Negative wide character not found\n", __func__);
        return;
    }
    if (result != str + 2) {
        t_error("%s: Negative wide character at wrong position\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1300
 * @tc.desc      : Test with exactly matching search range
 * @tc.level     : Level 1
 */
void wmemchr_1300(void)
{
    wchar_t str[] = L"Test";
    wchar_t target = L't';
    size_t sz = 4; // Exact length without null terminator

    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s: Character 't' not found in exact range\n", __func__);
        return;
    }

    // Should not find character beyond the exact range
    target = L'\0';
    result = wmemchr(str, target, sz);
    if (result) {
        t_error("%s: Found null terminator outside search range\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1400
 * @tc.desc      : Test with repeated pattern and boundary conditions
 * @tc.level     : Level 1
 */
void wmemchr_1400(void)
{
    wchar_t str[] = L"AAAABBBBCCCC";

    // Find first 'B' after multiple 'A's
    wchar_t target = L'B';
    size_t sz = wcslen(str);
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Character 'B' not found\n", __func__);
        return;
    }
    if (result != str + 4) {
        t_error("%s: Character 'B' at wrong position in repeated pattern\n", __func__);
    }

    // Test boundary: search exactly to the character position
    result = wmemchr(str, target, 5); // Search includes first 'B'
    if (!result) {
        t_error("%s: Character 'B' not found with boundary search\n", __func__);
    }

    result = wmemchr(str, target, 4); // Search stops before first 'B'
    if (result) {
        t_error("%s: Found character 'B' outside search boundary\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_1500
 * @tc.desc      : Test return value pointer verification
 * @tc.level     : Level 1
 */
void wmemchr_1500(void)
{
    wchar_t str[] = L"Verification Test";
    wchar_t target = L'T';
    size_t sz = wcslen(str);
    wchar_t *result = wmemchr(str, target, sz);

    if (!result) {
        t_error("%s: Character 'T' not found\n", __func__);
        return;
    }

    // Verify the returned pointer points to the correct character
    if (*result != target) {
        t_error("%s: Returned pointer does not point to target character\n", __func__);
    }

    // Verify the character before is not the target
    if (result > str && *(result - 1) == target) {
        t_error("%s: Returned pointer not pointing to first occurrence\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemchr_0100();
    wmemchr_0200();
    wmemchr_0300();
    wmemchr_0400();
    wmemchr_0500();
    wmemchr_0600();
    wmemchr_0700();
    wmemchr_0800();
    wmemchr_0900();
    wmemchr_1000();
    wmemchr_1100();
    wmemchr_1200();
    wmemchr_1300();
    wmemchr_1400();
    wmemchr_1500();
    return t_status;
}