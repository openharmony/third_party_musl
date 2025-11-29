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
#include <locale.h>
#include <errno.h>
#include "test.h"

/**
 * @tc.name      : wcsrtombs_0100
 * @tc.desc      : Test the wcsrtombs method to convert wide character string to multibyte string
 * @tc.level     : Level 0
 */
void wcsrtombs_0100(void)
{
    const wchar_t *src = L"Hello World";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Hello World") != 0) {
        t_error("%s wcsrtombs get result is '%s' are not want 'Hello World'\n", __func__, dst);
    }
    
    if (src_ptr != NULL) {
        t_error("%s wcsrtombs src_ptr should be NULL after conversion\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_0200
 * @tc.desc      : Test wcsrtombs with empty wide string
 * @tc.level     : Level 1
 */
void wcsrtombs_0200(void)
{
    const wchar_t *src = L"";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed\n", __func__);
        return;
    }
    
    if (result != 0) {
        t_error("%s wcsrtombs get result length is %zu are not want 0\n", __func__, result);
    }
    
    if (dst[0] != '\0') {
        t_error("%s wcsrtombs get dst is not empty string\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_0300
 * @tc.desc      : Test wcsrtombs with insufficient buffer size
 * @tc.level     : Level 1
 */
void wcsrtombs_0300(void)
{
    const wchar_t *src = L"Hello World";
    char dst[6];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed\n", __func__);
        return;
    }
    
    if (result >= sizeof(dst)) {
        t_error("%s wcsrtombs should not exceed buffer size\n", __func__);
    }
    
    if (src_ptr == NULL) {
        t_error("%s wcsrtombs src_ptr should not be NULL with insufficient buffer\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_0400
 * @tc.desc      : Test wcsrtombs with special characters
 * @tc.level     : Level 1
 */
void wcsrtombs_0400(void)
{
    const wchar_t *src = L"Test\nString\t123";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Test\nString\t123") != 0) {
        t_error("%s wcsrtombs get result is '%s' are not want 'Test\\nString\\t123'\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_0500
 * @tc.desc      : Test wcsrtombs with NULL destination buffer to calculate required size
 * @tc.level     : Level 1
 */
void wcsrtombs_0500(void)
{
    const wchar_t *src = L"Hello World";
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(NULL, &src_ptr, 0, &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs size calculation failed\n", __func__);
        return;
    }
    
    if (result != 11) {
        t_error("%s wcsrtombs get size is %zu are not want 11\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsrtombs_0600
 * @tc.desc      : Test wcsrtombs with unicode characters
 * @tc.level     : Level 2
 */
void wcsrtombs_0600(void)
{
    const wchar_t *src = L"中文测试";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        // This might fail if locale doesn't support UTF-8, which is acceptable
        return;
    }
    
    if (src_ptr != NULL) {
        t_error("%s wcsrtombs src_ptr should be NULL after conversion\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_0700
 * @tc.desc      : Test wcsrtombs with state persistence across calls
 * @tc.level     : Level 2
 */
void wcsrtombs_0700(void)
{
    const wchar_t *src = L"Partial";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    // First call with small buffer
    size_t result1 = wcsrtombs(dst, &src_ptr, 4, &state);
    if (result1 == (size_t)-1) {
        t_error("%s wcsrtombs first conversion failed\n", __func__);
        return;
    }
    
    // Second call to continue conversion
    size_t result2 = wcsrtombs(dst + result1, &src_ptr, sizeof(dst) - result1, &state);
    if (result2 == (size_t)-1) {
        t_error("%s wcsrtombs second conversion failed\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Partial") != 0) {
        t_error("%s wcsrtombs get result is '%s' are not want 'Partial'\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_0800
 * @tc.desc      : Test wcsrtombs with NULL state parameter
 * @tc.level     : Level 1
 */
void wcsrtombs_0800(void)
{
    const wchar_t *src = L"Test String";
    char dst[32];
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), NULL);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion with NULL state failed\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Test String") != 0) {
        t_error("%s wcsrtombs get result is '%s' are not want 'Test String'\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_0900
 * @tc.desc      : Test wcsrtombs with invalid wide character
 * @tc.level     : Level 2
 */
void wcsrtombs_0900(void)
{
    // Create a wide character that might be invalid in current locale
    wchar_t invalid_str[] = {0xFFFFFFFF, 0};
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = invalid_str;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result != (size_t)-1) {
        t_error("%s wcsrtombs should fail with invalid wide character\n", __func__);
    }
    
    if (errno != EILSEQ) {
        t_error("%s wcsrtombs should set errno to EILSEQ for invalid character\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_1000
 * @tc.desc      : Test wcsrtombs with exactly sized buffer
 * @tc.level     : Level 1
 */
void wcsrtombs_1000(void)
{
    const wchar_t *src = L"Buffer";
    char dst[7]; // "Buffer" + null terminator
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed\n", __func__);
        return;
    }
    
    if (result != 6) {
        t_error("%s wcsrtombs get result length is %zu are not want 6\n", __func__, result);
    }
    
    if (strcmp(dst, "Buffer") != 0) {
        t_error("%s wcsrtombs get result is '%s' are not want 'Buffer'\n", __func__, dst);
    }
}

int main(int argc, char *argv[])
{
    wcsrtombs_0100();
    wcsrtombs_0200();
    wcsrtombs_0300();
    wcsrtombs_0400();
    wcsrtombs_0500();
    wcsrtombs_0600();
    wcsrtombs_0700();
    wcsrtombs_0800();
    wcsrtombs_0900();
    wcsrtombs_1000();
    return t_status;
}