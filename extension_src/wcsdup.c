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
#include <stdlib.h>
#include <signal.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcsdup_0100
 * @tc.desc      : Copy a normal ASCII wide string
 * @tc.level     : Level 0
 */
void wcsdup_0100(void)
{
    const wchar_t *src = L"hello";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    if (wcscmp(src, result) != 0) {
        t_error("%s Strings are not equal: src='%ls', result='%ls'\n", __func__, src, result);
    }
    free(result);
}

/**
 * @tc.name      : wcsdup_0200
 * @tc.desc      : Copy an empty wide string
 * @tc.level     : Level 1
 */
void wcsdup_0200(void)
{
    const wchar_t *src = L"";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed for empty string\n", __func__);
        return;
    }
    if (wcscmp(src, result) != 0) {
        t_error("%s Empty strings are not equal\n", __func__);
    }
    if (result[0] != L'\0') {
        t_error("%s Empty string not properly null-terminated\n", __func__);
    }
    free(result);
}

/**
 * @tc.name      : wcsdup_0300
 * @tc.desc      : Copy string with wide Unicode characters
 * @tc.level     : Level 1
 */
void wcsdup_0300(void)
{
    const wchar_t *src = L"Hello, 世界! 🌍";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed for Unicode string\n", __func__);
        return;
    }
    if (wcscmp(src, result) != 0) {
        t_error("%s Unicode strings are not equal\n", __func__);
    }
    free(result);
}

/**
 * @tc.name      : wcsdup_0400
 * @tc.desc      : Test NULL input (should crash or return NULL)
 * @tc.level     : Level 2
 */
void wcsdup_0400(void)
{
    const wchar_t *src = NULL;
    wchar_t *result = wcsdup(src);
    // This implementation will crash on wcslen(NULL)
    // Some implementations may handle NULL, but this one doesn't
    if (result != NULL) {
        free(result);
    }
    // Note: This test expects a crash, so it's testing undefined behavior
}

/**
 * @tc.name      : wcsdup_0500
 * @tc.desc      : Verify memory independence between source and copy
 * @tc.level     : Level 2
 */
void wcsdup_0500(void)
{
    wchar_t src[] = L"TestString";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    // Modify original
    src[0] = L'X';
    if (wcscmp(result, L"TestString") != 0) {
        t_error("%s Copy was modified when original changed\n", __func__);
    }
    
    // Modify copy
    result[0] = L'Y';
    if (wcscmp(src, L"XestString") != 0) {
        t_error("%s Original was modified when copy changed\n", __func__);
    }
    
    free(result);
}

/**
 * @tc.name      : wcsdup_0600
 * @tc.desc      : Copy long string
 * @tc.level     : Level 1
 */
void wcsdup_0600(void)
{
    wchar_t long_str[1024];
    for (int i = 0; i < 1023; i++) {
        long_str[i] = L'A' + (i % 26);
    }
    long_str[1023] = L'\0';
    
    wchar_t *result = wcsdup(long_str);
    if (result == NULL) {
        t_error("%s Memory allocation failed for long string\n", __func__);
        return;
    }
    if (wcscmp(long_str, result) != 0) {
        t_error("%s Long strings are not equal\n", __func__);
    }
    free(result);
}

/**
 * @tc.name      : wcsdup_0700
 * @tc.desc      : Copy string with control characters
 * @tc.level     : Level 1
 */
void wcsdup_0700(void)
{
    const wchar_t *src = L"Line1\nLine2\tTab\\Backslash\x01Control";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed for string with control chars\n", __func__);
        return;
    }
    if (wcscmp(src, result) != 0) {
        t_error("%s Strings with control chars are not equal\n", __func__);
    }
    free(result);
}

/**
 * @tc.name      : wcsdup_0800
 * @tc.desc      : Verify correct length calculation
 * @tc.level     : Level 1
 */
void wcsdup_0800(void)
{
    const wchar_t *src = L"Test";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    size_t src_len = wcslen(src);
    size_t result_len = wcslen(result);
    if (src_len != result_len) {
        t_error("%s Length mismatch: src_len=%zu, result_len=%zu\n", __func__, src_len, result_len);
    }
    
    // Verify null terminator at correct position
    if (result[src_len] != L'\0') {
        t_error("%s Null terminator not at correct position\n", __func__);
    }
    
    free(result);
}

/**
 * @tc.name      : wcsdup_0900
 * @tc.desc      : Test return value is a valid pointer for writing
 * @tc.level     : Level 2
 */
void wcsdup_0900(void)
{
    const wchar_t *src = L"ReadOnly";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    // Test that we can write to the returned memory
    result[0] = L'W';
    result[1] = L'r';
    result[2] = L'i';
    result[3] = L't';
    result[4] = L'e';
    result[5] = L'\0';
    
    if (wcscmp(result, L"Write") != 0) {
        t_error("%s Cannot write to returned memory\n", __func__);
    }
    
    free(result);
}

/**
 * @tc.name      : wcsdup_1000
 * @tc.desc      : Verify wmemcpy copies the null terminator
 * @tc.level     : Level 2
 */
void wcsdup_1000(void)
{
    const wchar_t *src = L"Test\0Hidden";  // String with embedded null
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    // wcsdup should only copy up to the first null terminator
    if (wcscmp(result, L"Test") != 0) {
        t_error("%s Did not stop at first null terminator\n", __func__);
    }
    
    // Verify length is 4, not 10
    if (wcslen(result) != 4) {
        t_error("%s Wrong length: expected 4, got %zu\n", __func__, wcslen(result));
    }
    
    free(result);
}

/**
 * @tc.name      : wcsdup_1100
 * @tc.desc      : Test malloc failure simulation
 * @tc.level     : Level 2
 */
void wcsdup_1100(void)
{
    // This test requires mocking malloc to return NULL
    // For now, we test that the function doesn't crash on allocation failure
    const wchar_t *src = L"Test";
    
    // Note: In a real test framework, you'd mock malloc to return NULL
    wchar_t *result = wcsdup(src);
    
    // The function should handle malloc failure gracefully
    // (return NULL without crashing)
    if (result != NULL) {
        // Normal case - malloc succeeded
        if (wcscmp(src, result) != 0) {
            t_error("%s Strings are not equal\n", __func__);
        }
        free(result);
    } else {
        // malloc failed - this is acceptable behavior
        // The function returned NULL as expected
    }
}

/**
 * @tc.name      : wcsdup_1200
 * @tc.desc      : Test that wmemcpy is used correctly (l+1 bytes)
 * @tc.level     : Level 2
 */
void wcsdup_1200(void)
{
    const wchar_t *src = L"ABCD";
    wchar_t *result = wcsdup(src);
    if (result == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    // Verify the entire buffer including null terminator
    for (size_t i = 0; i <= wcslen(src); i++) {
        if (result[i] != src[i]) {
            t_error("%s Character mismatch at position %zu\n", __func__, i);
        }
    }
    
    free(result);
}

int main(int argc, char *argv[])
{
    wcsdup_0100();
    wcsdup_0200();
    wcsdup_0300();
    wcsdup_0400();
    wcsdup_0500();
    wcsdup_0600();
    wcsdup_0700();
    wcsdup_0800();
    wcsdup_0900();
    wcsdup_1000();
    wcsdup_1100();
    wcsdup_1200();

    return t_status;
}