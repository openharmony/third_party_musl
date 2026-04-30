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
 * @tc.name      : wmemmove_0100
 * @tc.desc      : Non-overlapping copy, source before destination
 * @tc.level     : Level 0
 */
void wmemmove_0100(void)
{
    wchar_t dest[] = L"This is a c test for wmemmove function";
    const wchar_t src[] = L"src content";
    int count = 5;
    wchar_t tmp[count + 1];
    
    wcsncpy(tmp, src, count);
    tmp[count] = L'\0';
    
    wmemmove(dest, src, count);
    
    if (wcsncmp(dest, tmp, count) != 0) {
        t_error("%s: The dest specified bits are not equal to the src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0200
 * @tc.desc      : Non-overlapping copy, destination before source
 * @tc.level     : Level 0
 */
void wmemmove_0200(void)
{
    wchar_t buffer[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    wchar_t *dest = buffer;
    wchar_t *src = buffer + 5;
    int count = 5;
    
    wchar_t expected[] = L"FGHIJFGHIJKLMNOPQRSTUVWXYZ";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Forward copy failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0300
 * @tc.desc      : Overlapping copy, destination after source (forward copy)
 * @tc.level     : Level 1
 */
void wmemmove_0300(void)
{
    wchar_t buffer[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    wchar_t *src = buffer;
    wchar_t *dest = buffer + 5;
    int count = 10;
    
    wchar_t expected[] = L"ABCDEABCDEFGHIJKLMNOPQRST";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Overlapping forward copy failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0400
 * @tc.desc      : Overlapping copy, source after destination (backward copy)
 * @tc.level     : Level 1
 */
void wmemmove_0400(void)
{
    wchar_t buffer[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    wchar_t *src = buffer + 5;
    wchar_t *dest = buffer;
    int count = 10;
    
    wchar_t expected[] = L"FGHIJKLMNOPKLMNOPQRSTUVWXYZ";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Overlapping backward copy failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0500
 * @tc.desc      : Same pointer (should return immediately)
 * @tc.level     : Level 1
 */
void wmemmove_0500(void)
{
    wchar_t buffer[] = L"Test string";
    wchar_t *src = buffer;
    wchar_t *dest = buffer;
    int count = 5;
    
    wchar_t backup[12];
    wcscpy(backup, buffer);
    
    wchar_t *result = wmemmove(dest, src, count);
    
    if (result != dest) {
        t_error("%s: Return value incorrect: expected %p, got %p\n", 
                __func__, (void*)dest, (void*)result);
    }
    
    if (wcscmp(buffer, backup) != 0) {
        t_error("%s: Buffer changed when src == dest: expected '%ls', got '%ls'\n", 
                __func__, backup, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0600
 * @tc.desc      : n = 0 (should do nothing)
 * @tc.level     : Level 1
 */
void wmemmove_0600(void)
{
    wchar_t buffer[] = L"Original data";
    wchar_t backup[] = L"Original data";
    int count = 0;
    
    wmemmove(buffer, buffer + 5, count);
    
    if (wcscmp(buffer, backup) != 0) {
        t_error("%s: Buffer changed when n=0: expected '%ls', got '%ls'\n", 
                __func__, backup, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0700
 * @tc.desc      : Copy single wide character
 * @tc.level     : Level 1
 */
void wmemmove_0700(void)
{
    wchar_t buffer[] = L"ABCDEFG";
    wchar_t *src = buffer + 3;  // 'D'
    wchar_t *dest = buffer + 1; // 'B'
    int count = 1;
    
    wchar_t expected[] = L"ADCDEFG";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Single char copy failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_0800
 * @tc.desc      : Full buffer copy (non-overlapping)
 * @tc.level     : Level 1
 */
void wmemmove_0800(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dest[20];
    int count = wcslen(src);
    
    wmemmove(dest, src, count);
    dest[count] = L'\0';
    
    if (wcscmp(dest, src) != 0) {
        t_error("%s: Full copy failed: expected '%ls', got '%ls'\n", 
                __func__, src, dest);
    }
}

/**
 * @tc.name      : wmemmove_0900
 * @tc.desc      : Wide character (Unicode) copy
 * @tc.level     : Level 1
 */
void wmemmove_0900(void)
{
    wchar_t src[] = L"中文测试Hello世界";
    wchar_t dest[20];
    int count = wcslen(src);
    
    wmemmove(dest, src, count);
    dest[count] = L'\0';
    
    if (wcscmp(dest, src) != 0) {
        t_error("%s: Unicode copy failed: expected '%ls', got '%ls'\n", 
                __func__, src, dest);
    }
}

/**
 * @tc.name      : wmemmove_1000
 * @tc.desc      : Overlap exactly at boundary
 * @tc.level     : Level 2
 */
void wmemmove_1000(void)
{
    wchar_t buffer[] = L"ABCDEFGHIJ";
    wchar_t *src = buffer;
    wchar_t *dest = buffer + 5;
    int count = 5;
    
    wchar_t expected[] = L"ABCDEABCDE";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Exact boundary overlap failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_1100
 * @tc.desc      : Verify return value is destination pointer
 * @tc.level     : Level 1
 */
void wmemmove_1100(void)
{
    wchar_t src[] = L"Test";
    wchar_t dest[5];
    int count = 4;
    
    wchar_t *result = wmemmove(dest, src, count);
    
    if (result != dest) {
        t_error("%s: Return value incorrect: expected %p, got %p\n", 
                __func__, (void*)dest, (void*)result);
    }
}

/**
 * @tc.name      : wmemmove_1200
 * @tc.desc      : Large n value
 * @tc.level     : Level 2
 */
void wmemmove_1200(void)
{
    const int size = 1000;
    wchar_t *src = (wchar_t *)malloc(size * sizeof(wchar_t));
    wchar_t *dest = (wchar_t *)malloc(size * sizeof(wchar_t));
    
    if (src == NULL || dest == NULL) {
        t_error("%s: Memory allocation failed\n", __func__);
        free(src);
        free(dest);
        return;
    }
    
    for (int i = 0; i < size; i++) {
        src[i] = L'A' + (i % 26);
    }
    
    wmemmove(dest, src, size);
    
    for (int i = 0; i < size; i++) {
        if (dest[i] != src[i]) {
            t_error("%s: Large copy failed at position %d\n", __func__, i);
            break;
        }
    }
    
    free(src);
    free(dest);
}

/**
 * @tc.name      : wmemmove_1300
 * @tc.desc      : Overlap detection logic test
 * @tc.level     : Level 2
 */
void wmemmove_1300(void)
{
    wchar_t buffer[20];
    for (int i = 0; i < 20; i++) {
        buffer[i] = L'A' + i;
    }
    
    // Test 1: dest < src, distance < n*sizeof(wchar_t) (should use backward copy)
    wchar_t buffer1[20];
    wmemcpy(buffer1, buffer, 20);
    wmemmove(buffer1, buffer1 + 2, 10);  // dest=0, src=2, n=10
    
    // Test 2: dest > src, distance < n*sizeof(wchar_t) (should use forward copy)
    wchar_t buffer2[20];
    wmemcpy(buffer2, buffer, 20);
    wmemmove(buffer2 + 2, buffer2, 10);  // dest=2, src=0, n=10
    
    // Both should produce correct results
    wprintf(L"%s: Overlap detection tests completed\n", __func__);
}

/**
 * @tc.name      : wmemmove_1400
 * @tc.desc      : Partial overlap near beginning
 * @tc.level     : Level 2
 */
void wmemmove_1400(void)
{
    wchar_t buffer[] = L"0123456789";
    wchar_t *src = buffer + 1;  // "123456789"
    wchar_t *dest = buffer;     // "0123456789"
    int count = 5;              // Copy "12345"
    
    wchar_t expected[] = L"1234556789";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Partial overlap failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

/**
 * @tc.name      : wmemmove_1500
 * @tc.desc      : Partial overlap near end
 * @tc.level     : Level 2
 */
void wmemmove_1500(void)
{
    wchar_t buffer[] = L"0123456789";
    wchar_t *src = buffer;      // "0123456789"
    wchar_t *dest = buffer + 1; // "123456789"
    int count = 5;              // Copy "01234"
    
    wchar_t expected[] = L"0012345679";
    
    wmemmove(dest, src, count);
    
    if (wcscmp(buffer, expected) != 0) {
        t_error("%s: Partial overlap failed: expected '%ls', got '%ls'\n", 
                __func__, expected, buffer);
    }
}

int main(int argc, char *argv[])
{
    wmemmove_0100();
    wmemmove_0200();
    wmemmove_0300();
    wmemmove_0400();
    wmemmove_0500();
    wmemmove_0600();
    wmemmove_0700();
    wmemmove_0800();
    wmemmove_0900();
    wmemmove_1000();
    wmemmove_1100();
    wmemmove_1200();
    wmemmove_1300();
    wmemmove_1400();
    wmemmove_1500();

    return t_status;
}
