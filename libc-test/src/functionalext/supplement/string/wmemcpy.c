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
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemcpy_0100
 * @tc.desc      : Copies specified number of characters from one string to another
 * @tc.level     : Level 0
 */
void wmemcpy_0100(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"This is a c test for wmemcpy";
    int n = 13;
    wmemcpy(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0200
 * @tc.desc      : Test wmemcpy when 'n' is equal to the number characters in src string
 * @tc.level     : Level 0
 */
void wmemcpy_0200(void)
{
    wchar_t dest[3];
    wchar_t src[] = L"aaa";
    int n = 3;
    wmemcpy(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0300
 * @tc.desc      : Test wmemcpy when 'n' is equal to 0
 * @tc.level     : Level 0
 */
void wmemcpy_0300(void)
{
    wchar_t dest[3];
    wchar_t src[] = L"aaa";
    int n = 0;
    wmemcpy(dest, src, n);
    if (!wcscmp(dest, src)) {
        t_error("%s The string of dest is equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0400
 * @tc.desc      : Copies zero characters (n = 0)
 * @tc.level     : Level 1
 */
void wmemcpy_0400(void)
{
    wchar_t dest[30] = L"Original content";
    wchar_t src[] = L"Should not be copied";
    wmemcpy(dest, src, 0);
    if (wcscmp(dest, L"Original content")) {
        t_error("%s Destination was modified when n=0\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0500
 * @tc.desc      : Copies from empty source string
 * @tc.level     : Level 1
 */
void wmemcpy_0500(void)
{
    wchar_t dest[30] = L"Original content";
    wchar_t src[] = L"";
    size_t n = 0;
    wmemcpy(dest, src, n);
    if (wcscmp(dest, L"Original content")) {
        t_error("%s Destination was modified when copying empty string\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0600
 * @tc.desc      : Verifies return value is the destination pointer
 * @tc.level     : Level 1
 */
void wmemcpy_0600(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"Test return value";
    size_t n = wcslen(src);
    wchar_t *ret = wmemcpy(dest, src, n);
    if (ret != dest) {
        t_error("%s Return value is not destination pointer\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0700
 * @tc.desc      : Copies to the exact size of destination buffer
 * @tc.level     : Level 1
 */
void wmemcpy_0700(void)
{
    wchar_t src[] = L"ExactFit";
    wchar_t dest[8]; // Exactly fits the 7 characters + no null terminator
    size_t n = 7; // Copy exactly 7 characters
    
    wmemcpy(dest, src, n);
    
    if (wcsncmp(dest, src, n)) {
        t_error("%s Exact buffer copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0800
 * @tc.desc      : Tests copying with restrict qualifier compliance
 * @tc.level     : Level 2
 */
void wmemcpy_0800(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"Testing restrict semantics";
    size_t n = 10;
    
    wmemcpy(dest, src, n);
    
    // Verify independent memory regions
    src[0] = L'X'; // Modify source
    if (dest[0] == L'X') {
        t_error("%s restrict qualifier violation detected\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0900
 * @tc.desc      : Large copy operation
 * @tc.level     : Level 1
 */
void wmemcpy_0900(void)
{
    const size_t BUFFER_SIZE = 1000;
    wchar_t *src = (wchar_t *)malloc(BUFFER_SIZE * sizeof(wchar_t));
    wchar_t *dest = (wchar_t *)malloc(BUFFER_SIZE * sizeof(wchar_t));
    
    if (src == NULL || dest == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    // Fill source with pattern
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        src[i] = L'A' + (i % 26);
    }
    
    wmemcpy(dest, src, BUFFER_SIZE);
    
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        if (dest[i] != src[i]) {
            t_error("%s Large copy failed at position %zu\n", __func__, i);
            break;
        }
    }
    
    free(src);
    free(dest);
}

/**
 * @tc.name      : wmemcpy_1000
 * @tc.desc      : Partial copy leaving remaining destination unchanged
 * @tc.level     : Level 1
 */
void wmemcpy_1000(void)
{
    wchar_t dest[20];
    wmemset(dest, L'Z', 20); // Fill with 'Z'
    wchar_t src[] = L"Partial";
    size_t n = 7; // Copy 7 characters
    
    wmemcpy(dest, src, n);
    
    // Check copied part
    if (wcsncmp(dest, src, n)) {
        t_error("%s Partial copy failed\n", __func__);
    }
    
    // Check remaining part is still 'Z'
    for (size_t i = n; i < 20; i++) {
        if (dest[i] != L'Z') {
            t_error("%s Destination after n was modified\n", __func__);
            break;
        }
    }
}

/**
 * @tc.name      : wmemcpy_1100
 * @tc.desc      : Copies entire string including null terminator
 * @tc.level     : Level 0
 */
void wmemcpy_1100(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"Complete copy test";
    size_t n = wcslen(src) + 1; // Include null terminator
    wmemcpy(dest, src, n);
    if (wcscmp(dest, src)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_1200
 * @tc.desc      : Copies zero characters (n = 0)
 * @tc.level     : Level 1
 */
void wmemcpy_1200(void)
{
    wchar_t dest[20] = L"Original content";
    wchar_t src[] = L"New content";
    wchar_t original[20];
    wcscpy(original, dest);
    
    wmemcpy(dest, src, 0);
    
    if (wcscmp(dest, original)) {
        t_error("%s Destination was modified when n=0\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_1300
 * @tc.desc      : Copies wide characters including non-ASCII characters
 * @tc.level     : Level 0
 */
void wmemcpy_1300(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"中文测试 こんにちは Привет";
    size_t n = 8; // Copy first 8 wide characters
    wmemcpy(dest, src, n);
    
    dest[n] = L'\0'; // Null terminate for comparison
    
    wchar_t expected[9];
    wcsncpy(expected, src, n);
    expected[n] = L'\0';
    
    if (wcscmp(dest, expected)) {
        t_error("%s Wide character copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_1400
 * @tc.desc      : Verifies return value is the destination pointer
 * @tc.level     : Level 1
 */
void wmemcpy_1400(void)
{
    wchar_t dest[20];
    wchar_t src[] = L"Test return value";
    size_t n = wcslen(src);
    wchar_t *ret = wmemcpy(dest, src, n);
    
    if (ret != dest) {
        t_error("%s Return value is not destination pointer\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemcpy_0100();
    wmemcpy_0200();
    wmemcpy_0300();
    wmemcpy_0400();
    wmemcpy_0500();
    wmemcpy_0600();
    wmemcpy_0700();
    wmemcpy_0800();
    wmemcpy_0900();
    wmemcpy_1000();
    wmemcpy_1100();
    wmemcpy_1200();
    wmemcpy_1300();
    wmemcpy_1400();
    return t_status;
}