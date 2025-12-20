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

#include <signal.h>
#include <stdlib.h>
#include <wchar.h>

#include "test.h"

#define SIZEOF_WCHAR(x) (sizeof(x) / sizeof(wchar_t))

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : wcsncpy_0100
 * @tc.desc      : copy a fixed-size string of wide characters
 * @tc.level     : Level 0
 */
void wcsncpy_0100(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src) + 1];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, SIZEOF_WCHAR(src))) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }
}

/**
 * @tc.name      : wcsncpy_0200
 * @tc.desc      : copy a 0-size string of wide characters
 * @tc.level     : Level 1
 */
void wcsncpy_0200(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src)];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, 0)) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }
}

/**
 * @tc.name      : wcsncpy_0300
 * @tc.desc      : copy a fixed-size string of wide characters with a larger size
 * @tc.level     : Level 1
 */
void wcsncpy_0300(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t buf[SIZEOF_WCHAR(src) + SIZEOF_WCHAR(src)];
    wmemset(buf, L'A', SIZEOF_WCHAR(buf));
    wchar_t *dest = (wchar_t *)buf;

    if (dest != wcsncpy(dest, src, SIZEOF_WCHAR(src) + SIZEOF_WCHAR(src) - 1)) {
        t_error("%s failed: wcsncpy. src = %ls, dest = %ls\n", __func__, src, dest);
    }

    int i = 0;
    for (i = SIZEOF_WCHAR(src) + 1; i < SIZEOF_WCHAR(buf) - 1; i++) {
        if (buf[i] != L'\0') {
            t_error("%s failed: buf[%d] = %lc\n", __func__, i, buf[i]);
        }
    }

    if (buf[i] != L'A') {
        t_error("%s failed: buf[%d] = %lc\n", __func__, i, buf[i]);
    }
}

/**
 * @tc.name      : wcsncpy_0400
 * @tc.desc      : copy a fixed-size string of wide characters to a NULL pointer
 * @tc.level     : Level 2
 */
void wcsncpy_0400(void)
{
    signal(SIGSEGV, handler);

    const wchar_t src[] = L"Hello";
    wcsncpy(NULL, src, SIZEOF_WCHAR(src));
}

/**
 * @tc.name      : wcsncpy_0500
 * @tc.desc      : Copy n = 0
 * @tc.level     : Level 1
 */
void wcsncpy_0500(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t dest[10];
    wmemset(dest, L'Z', 10);
    
    wchar_t *result = wcsncpy(dest, src, 0);
    
    if (result != dest) {
        t_error("%s Return value incorrect\n", __func__);
    }
    
    // Destination should be unchanged
    for (int i = 0; i < 10; i++) {
        if (dest[i] != L'Z') {
            t_error("%s Buffer modified when n=0\n", __func__);
        }
    }
}

/**
 * @tc.name      : wcsncpy_0600
 * @tc.desc      : Copy wide characters including non-ASCII
 * @tc.level     : Level 0
 */
void wcsncpy_0600(void)
{
    const wchar_t src[] = L"中文测试Hello";
    wchar_t dest[10];
    
    wcsncpy(dest, src, 7);  // Copy 4 Chinese chars + 3 English
    
    wchar_t expected[] = L"中文测";
    if (wcsncmp(dest, expected, 3) != 0) {  // Compare first 3 wide chars
        t_error("%s Wide character copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wcsncpy_0700
 * @tc.desc      : Test restrict qualifier compliance
 * @tc.level     : Level 2
 */
void wcsncpy_0700(void)
{
    wchar_t buffer[20] = L"Original Content";
    wchar_t *src = buffer;
    wchar_t *dest = buffer + 5;
    
    // This tests restrict behavior - may be undefined but we test
    wcsncpy(dest, src, 8);
    
    // Verify the copy happened
    if (wcsncmp(dest, L"Original", 8) != 0) {
        t_error("%s Overlap copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wcsncpy_0800
 * @tc.desc      : Copy exactly to buffer size
 * @tc.level     : Level 1
 */
void wcsncpy_0800(void)
{
    const wchar_t src[] = L"Exactly";
    wchar_t dest[8];  // 7 chars + 1 null
    
    wcsncpy(dest, src, 7);
    
    // All 7 chars copied
    if (wcsncmp(dest, src, 7) != 0) {
        t_error("%s Exact copy failed\n", __func__);
    }
    
    // Should NOT be null-terminated because n=7
    // dest[7] is uninitialized, not guaranteed null
}

/**
 * @tc.name      : wcsncpy_0900
 * @tc.desc      : String with null character in middle
 * @tc.level     : Level 2
 */
void wcsncpy_0900(void)
{
    wchar_t src[] = L"A\0BC";  // Null in middle
    wchar_t dest[10];
    wmemset(dest, L'X', 10);
    
    wcsncpy(dest, src, 8);
    
    // Should copy 'A' and null, then pad with nulls
    if (dest[0] != L'A' || dest[1] != L'\0') {
        t_error("%s Copy of string with null failed\n", __func__);
    }
    
    // Positions 2-7 should be null
    for (int i = 2; i < 8; i++) {
        if (dest[i] != L'\0') {
            t_error("%s Position %d not null\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wcsncpy_1000
 * @tc.desc      : Large n value
 * @tc.level     : Level 2
 */
void wcsncpy_1000(void)
{
    const wchar_t src[] = L"Short";
    wchar_t dest[1000];
    
    wcsncpy(dest, src, 1000);
    
    // First 5 chars should be copied
    if (wcsncmp(dest, src, 5) != 0) {
        t_error("%s Source not copied\n", __func__);
    }
    
    // Next 995 chars should be null
    for (int i = 5; i < 1000; i++) {
        if (dest[i] != L'\0') {
            t_error("%s Position %d not null\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wcsncpy_1100
 * @tc.desc      : Verify null padding when source is shorter
 * @tc.level     : Level 1
 */
void wcsncpy_1100(void)
{
    const wchar_t src[] = L"AB";
    wchar_t dest[6] = {L'1', L'2', L'3', L'4', L'5', L'6'};
    
    wcsncpy(dest, src, 5);
    
    // dest should be: 'A', 'B', '\0', '\0', '\0', '6'
    if (dest[0] != L'A' || dest[1] != L'B') {
        t_error("%s Source not copied\n", __func__);
    }
    
    // Positions 2-4 should be null
    for (int i = 2; i < 5; i++) {
        if (dest[i] != L'\0') {
            t_error("%s Position %d not null\n", __func__, i);
        }
    }
    
    // Position 5 should be unchanged
    if (dest[5] != L'6') {
        t_error("%s Position 5 was modified\n", __func__);
    }
}

/**
 * @tc.name      : wcsncpy_1200
 * @tc.desc      : Case sensitivity test
 * @tc.level     : Level 0
 */
void wcsncpy_1200(void)
{
    const wchar_t src[] = L"Hello";
    wchar_t dest[10];
    
    wcsncpy(dest, src, 5);
    
    // Should copy exactly, case sensitive
    wchar_t lower[] = L"hello";
    if (wcsncmp(dest, lower, 5) == 0) {
        t_error("%s Case sensitivity issue\n", __func__);
    }
    
    if (wcsncmp(dest, src, 5) != 0) {
        t_error("%s Copy not exact\n", __func__);
    }
}

/**
 * @tc.name      : wcsncpy_1300
 * @tc.desc      : Test with spaces and special characters
 * @tc.level     : Level 1
 */
void wcsncpy_1300(void)
{
    const wchar_t src[] = L"  Hello\tWorld\n";
    wchar_t dest[20];
    
    wcsncpy(dest, src, 10);
    
    if (wcsncmp(dest, src, 10) != 0) {
        t_error("%s Special chars not copied correctly\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcsncpy_0100();
    wcsncpy_0200();
    wcsncpy_0300();
    wcsncpy_0400();
    wcsncpy_0500();
    wcsncpy_0600();
    wcsncpy_0700();
    wcsncpy_0800();
    wcsncpy_0900();
    wcsncpy_1000();
    wcsncpy_1100();
    wcsncpy_1200();
    wcsncpy_1300();
    return t_status;
}
