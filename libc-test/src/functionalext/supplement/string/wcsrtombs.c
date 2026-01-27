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

/**
 * @tc.name      : wcsrtombs_1100
 * @tc.desc      : Test wcsrtombs with accented characters (UTF-8 locale required)
 * @tc.level     : Level 2
 */
void wcsrtombs_1100(void)
{
    const wchar_t *src = L"café résumé";  
    char dst[32] = {0};  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }
    
    char *new_locale = setlocale(LC_ALL, "UTF-8");
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 locale (setlocale return NULL)", __func__);
        return;  
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for accented characters (errno=%d)", __func__, errno);
        setlocale(LC_ALL, old_locale);
        return;
    }
    
    if (strcmp(dst, "café résumé") != 0) {
        t_error("%s: wcsrtombs get result is '%s' (want 'café résumé')", __func__, dst);
    }
    
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_1200
 * @tc.desc      : Test wcsrtombs with buffer size exactly equal to required length (no extra null terminator)
 * @tc.level     : Level 2
 */
void wcsrtombs_1200(void)
{
    const wchar_t *src = L"ExactSize";  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;  
    
    size_t required_len = wcsrtombs(NULL, &src_ptr, 0, &state);
    if (required_len == (size_t)-1) {
        t_error("%s: wcsrtombs failed to calculate required length (errno=%d)", __func__, errno);
        return;
    }
    
    if (required_len == 0 || required_len > 1024) {  
        t_error("%s: invalid required length %zu (out of valid range)", __func__, required_len);
        return;
    }
    
    char dst[required_len];
    src_ptr = src;  
    
    if (memset_s(&state, sizeof(mbstate_t), 0, sizeof(mbstate_t)) != 0) {
        t_error("%s: memset_s failed to reset mbstate_t", __func__);
        return;  
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed with exact buffer size (errno=%d)", __func__, errno);
        return;
    }
    
    if (result != sizeof(dst)) {
        t_error("%s: wcsrtombs get result length %zu (want %zu)", __func__, result, sizeof(dst));
    }
    
    if (src_ptr == NULL || src_ptr != src + wcslen(src)) {
        t_error("%s: src_ptr should point to end of source (src+%zu), but got %p", 
                __func__, wcslen(src), (void*)src_ptr);
    }
}

/**
 * @tc.name      : wcsrtombs_1300
 * @tc.desc      : Test wcsrtombs with Japanese characters (UTF-8 locale required)
 * @tc.level     : Level 2
 */
void wcsrtombs_1300(void)
{
    const wchar_t *src = L"りんご バナナ";  
    char dst[64] = {0};  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }
    
    char *utf8_locales[] = {"UTF-8", "C.UTF-8", "en_US.UTF-8", "ja_JP.UTF-8", NULL};
    char *new_locale = NULL;
    for (int i = 0; utf8_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, utf8_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 compatible locale (required for Japanese conversion)", __func__);
        return;
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for Japanese characters (errno=%d)", __func__, errno);
        setlocale(LC_ALL, old_locale);
        return;
    }
    
    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion for Japanese characters (src_ptr=%p)", 
                __func__, (void*)src_ptr);
    }
    
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_1400
 * @tc.desc      : Test wcsrtombs with NULL source string
 * @tc.level     : Level 3
 */
void wcsrtombs_1400(void)
{
    const wchar_t *src = NULL;
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result != 0) {
        t_error("%s wcsrtombs get result length %zu (want 0) for NULL source\n", __func__, result);
    }
    
    if (src_ptr != NULL) {
        t_error("%s wcsrtombs src_ptr should be NULL for NULL source\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_1500
 * @tc.desc      : Test wcsrtombs with large buffer (exceeding required size)
 * @tc.level     : Level 1
 */
void wcsrtombs_1500(void)
{
    const wchar_t *src = L"Large buffer test: 1234567890";
    char dst[1024];  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed with large buffer\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Large buffer test: 1234567890") != 0) {
        t_error("%s wcsrtombs get result '%s' (want 'Large buffer test: 1234567890')\n", __func__, dst);
    }
    
    if (src_ptr != NULL) {
        t_error("%s wcsrtombs src_ptr should be NULL with large buffer\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_1600
 * @tc.desc      : Test wcsrtombs with non-UTF-8 locale (e.g., GBK for Chinese)
 * @tc.level     : Level 3
 */
void wcsrtombs_1600(void)
{
    const wchar_t *src = L"中文测试";
    char dst[32] = {0};  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }
    
    char *gbk_locales[] = {"zh_CN.GBK", "GBK", "zh_CN", "zh_CN.gbk", NULL};
    char *new_locale = NULL;
    for (int i = 0; gbk_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, gbk_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    
    if (new_locale == NULL) {
        printf("%s: GBK compatible locale not available, skip test\n", __func__);
        return;  
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed with GBK locale (errno=%d)", __func__, errno);
        if (setlocale(LC_ALL, old_locale) == NULL) {
            t_error("%s: failed to restore original locale after conversion failure", __func__);
        }
        return;
    }
    
    if (result != 8) {
        t_error("%s: wcsrtombs get length %zu (want 8) with GBK locale", __func__, result);
    }
    
    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion with GBK locale (src_ptr=%p)", 
                __func__, (void*)src_ptr);
    }
    
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_1700
 * @tc.desc      : Test wcsrtombs with state reset between calls
 * @tc.level     : Level 2
 */
void wcsrtombs_1700(void)
{
    const wchar_t *src1 = L"Partial1";
    const wchar_t *src2 = L"FullString";
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src1;

    size_t result1 = wcsrtombs(dst, &src_ptr, 5, &state);
    if (result1 == (size_t)-1) {
        t_error("%s wcsrtombs first conversion failed\n", __func__);
        return;
    }

    if (memset_s(&state, sizeof(mbstate_t), 0, sizeof(mbstate_t)) != 0) {
        t_error("%s: memset_s failed to reset mbstate_t", __func__);
        return;  
    }

    src_ptr = src2;
    size_t result2 = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result2 == (size_t)-1) {
        t_error("%s wcsrtombs second conversion failed after state reset\n", __func__);
        return;
    }

    if (strcmp(dst, "FullString") != 0) {
        t_error("%s wcsrtombs get result '%s' (want 'FullString') after state reset\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_1800
 * @tc.desc      : Test wcsrtombs with symbols and punctuation (mixed with letters)
 * @tc.level     : Level 1
 */
void wcsrtombs_1800(void)
{
    const wchar_t *src = L"Hello!@#$%^&*()_+-=[]{}|;':\",./<>?";
    char dst[128];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed for symbols\n", __func__);
        return;
    }
    
    if (strcmp(dst, "Hello!@#$%^&*()_+-=[]{}|;':\",./<>?") != 0) {
        t_error("%s wcsrtombs get result '%s' (want mixed symbols)\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_1900
 * @tc.desc      : Test wcsrtombs with non-BMP characters (emoji, UTF-8 locale required)
 * @tc.level     : Level 3
 */
void wcsrtombs_1900(void)
{
    const wchar_t *src = NULL;
    #ifdef _WIN32
    static wchar_t emoji_src[] = {0xD83D, 0xDE00, 0xD83D, 0xDE01, L'\0'};  
    src = emoji_src;
    #else
    src = L"😀😁";  
    #endif
    
    char dst[32] = {0};  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }
    
    char *utf8_locales[] = {"UTF-8", "C.UTF-8", "en_US.UTF-8", NULL};
    char *new_locale = NULL;
    for (int i = 0; utf8_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, utf8_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 compatible locale (required for emoji conversion)", __func__);
        return;
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for emoji (errno=%d)", __func__, errno);
        if (setlocale(LC_ALL, old_locale) == NULL) {
            t_error("%s: failed to restore original locale after conversion failure", __func__);
        }
        return;
    }
    
    if (result != 8) {
        t_error("%s: wcsrtombs get length %zu (want 8) for emoji", __func__, result);
    }
    
    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion for emoji (src_ptr=%p)", 
                __func__, (void*)src_ptr);
    }
    
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_2000
 * @tc.desc      : Test wcsrtombs with zero buffer size (only calculate length)
 * @tc.level     : Level 1
 */
void wcsrtombs_2000(void)
{
    const wchar_t *src = L"ZeroBufferSizeTest";
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(NULL, &src_ptr, 0, &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs size calculation failed with zero buffer\n", __func__);
        return;
    }
    
    if (result != 18) {
        t_error("%s wcsrtombs get length %zu (want 18) with zero buffer\n", __func__, result);
    }
    
    if (src_ptr != src) {
        t_error("%s wcsrtombs src_ptr should not advance with zero buffer\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_2100
 * @tc.desc      : Test wcsrtombs with trailing null characters in source
 * @tc.level     : Level 1
 */
void wcsrtombs_2100(void)
{
    const wchar_t src[] = {L'a', L'b', L'c', L'\0', L'\0', L'd', L'\0'};
    char dst[32];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed with trailing nulls\n", __func__);
        return;
    }
    
    if (strcmp(dst, "abc") != 0) {
        t_error("%s wcsrtombs get result '%s' (want 'abc') with trailing nulls\n", __func__, dst);
    }
    
    if (src_ptr != &src[3]) {  
        t_error("%s wcsrtombs src_ptr not pointing to first null terminator\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_2200
 * @tc.desc      : Test wcsrtombs with invalid mbstate_t (corrupted state)
 * @tc.level     : Level 3
 */
void wcsrtombs_2200(void)
{
    const wchar_t *src = L"ValidString";
    char dst[32] = {0};  
    mbstate_t state;
    const wchar_t *src_ptr = src;
    
    if (memset_s(&state, sizeof(mbstate_t), 0xAA, sizeof(mbstate_t)) != 0) {
        t_error("%s: memset_s failed to corrupt mbstate_t", __func__);
        return;  
    }  
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    
    if (result == (size_t)-1) {
        printf("%s: test completed without crash (invalid state, conversion failed, errno=%d)\n", 
                __func__, errno);
    } else {
        printf("%s: test completed without crash (invalid state, conversion succeeded, result length=%zu)\n", 
                __func__, result);
    }
    
    (void)dst;
    (void)src_ptr;
}
/**
 * @tc.name      : wcsrtombs_2300
 * @tc.desc      : Test wcsrtombs with mixed ASCII and non-ASCII characters
 * @tc.level     : Level 2
 */
void wcsrtombs_2300(void)
{
    const wchar_t *src = L"Mix: 中文 123 café 😀";  
    char dst[128] = {0};  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }
    
    char *utf8_locales[] = {"UTF-8", "C.UTF-8", "en_US.UTF-8", NULL};
    char *new_locale = NULL;
    for (int i = 0; utf8_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, utf8_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 compatible locale (required for mixed characters conversion)", __func__);
        return;
    }
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for mixed characters (errno=%d)", __func__, errno);
        if (setlocale(LC_ALL, old_locale) == NULL) {
            t_error("%s: failed to restore original locale after conversion failure", __func__);
        }
        return;
    }
    
    if (strcmp(dst, "Mix: 中文 123 café 😀") != 0) {
        t_error("%s: wcsrtombs get result '%s' (want 'Mix: 中文 123 café 😀')", __func__, dst);
    }
    
    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion for mixed characters (src_ptr=%p)", 
                __func__, (void*)src_ptr);
    }
    
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}
/**
 * @tc.name      : wcsrtombs_2400
 * @tc.desc      : Test wcsrtombs with buffer size 1 (only null terminator)
 * @tc.level     : Level 2
 */
void wcsrtombs_2400(void)
{
    const wchar_t *src = L"LongString";
    char dst[1];  
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;
    
    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed with buffer size 1\n", __func__);
        return;
    }
    
    if (result != 0) {
        t_error("%s wcsrtombs get length %zu (want 0) with buffer size 1\n", __func__, result);
    }
    
    if (dst[0] != '\0') {
        t_error("%s wcsrtombs dst should be null terminator with buffer size 1\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_2500
 * @tc.desc      : Test wcsrtombs with repeated conversions using same state
 * @tc.level     : Level 2
 */
void wcsrtombs_2500(void)
{
    const wchar_t *src_part1 = L"FirstPart";
    const wchar_t *src_part2 = L"SecondPart";
    char dst[64] = {0};
    mbstate_t state = {0};
    const wchar_t *src_ptr = src_part1;

    size_t result1 = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result1 == (size_t)-1 || src_ptr != NULL) {
        t_error("%s wcsrtombs first part conversion failed\n", __func__);
        return;
    }

    src_ptr = src_part2;
    size_t result2 = wcsrtombs(dst + result1, &src_ptr, sizeof(dst) - result1, &state);
    if (result2 == (size_t)-1 || src_ptr != NULL) {
        t_error("%s wcsrtombs second part conversion failed\n", __func__);
        return;
    }

    if (strcmp(dst, "FirstPartSecondPart") != 0) {
        t_error("%s wcsrtombs get result '%s' (want 'FirstPartSecondPart')\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsrtombs_2600
 * @tc.desc      : Test wcsrtombs with Greek characters (UTF-8 locale required)
 * @tc.level     : Level 2
 */
void wcsrtombs_2600(void)
{
    const wchar_t *src = L"Γεια σας";
    char dst[64] = {0};
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;

    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }

    char *utf8_locales[] = {"UTF-8", "C.UTF-8", "en_US.UTF-8", "el_GR.UTF-8", NULL};
    char *new_locale = NULL;
    for (int i = 0; utf8_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, utf8_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 compatible locale (required for Greek conversion)", __func__);
        return;
    }

    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for Greek characters (errno=%d)", __func__, errno);
        setlocale(LC_ALL, old_locale);
        return;
    }

    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion for Greek characters (src_ptr=%p)",
                __func__, (void*)src_ptr);
    }

    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_2700
 * @tc.desc      : Test wcsrtombs with Cyrillic characters (UTF-8 locale required)
 * @tc.level     : Level 2
 */
void wcsrtombs_2700(void)
{
    const wchar_t *src = L"Привет";
    char dst[64] = {0};
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;

    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }

    char *utf8_locales[] = {"UTF-8", "C.UTF-8", "en_US.UTF-8", "ru_RU.UTF-8", NULL};
    char *new_locale = NULL;
    for (int i = 0; utf8_locales[i] != NULL; i++) {
        new_locale = setlocale(LC_ALL, utf8_locales[i]);
        if (new_locale != NULL) {
            break;
        }
    }
    if (new_locale == NULL) {
        t_error("%s: failed to set UTF-8 compatible locale (required for Cyrillic conversion)", __func__);
        return;
    }

    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s: wcsrtombs conversion failed for Cyrillic characters (errno=%d)", __func__, errno);
        setlocale(LC_ALL, old_locale);
        return;
    }

    if (src_ptr != NULL) {
        t_error("%s: wcsrtombs did not complete conversion for Cyrillic characters (src_ptr=%p)",
                __func__, (void*)src_ptr);
    }

    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, old_locale);
    }
}

/**
 * @tc.name      : wcsrtombs_2800
 * @tc.desc      : Test wcsrtombs with buffer size exactly 1 byte
 * @tc.level     : Level 2
 */
void wcsrtombs_2800(void)
{
    const wchar_t *src = L"Test";
    char dst[1];
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;

    size_t result = wcsrtombs(dst, &src_ptr, sizeof(dst), &state);
    if (result == (size_t)-1) {
        t_error("%s wcsrtombs conversion failed with buffer size 1\n", __func__);
        return;
    }

    if (result > 0) {
        t_error("%s wcsrtombs should return 0 when buffer only fits null terminator\n", __func__);
    }

    if (dst[0] != '\0') {
        t_error("%s wcsrtombs dst should contain null terminator\n", __func__);
    }
}

/**
 * @tc.name      : wcsrtombs_2900
 * @tc.desc      : Test wcsrtombs with NULL destination and non-zero buffer size
 * @tc.level     : Level 3
 */
void wcsrtombs_2900(void)
{
    const wchar_t *src = L"TestString";
    mbstate_t state = {0};
    const wchar_t *src_ptr = src;

    size_t result = wcsrtombs(NULL, &src_ptr, 10, &state);

    if (result != 0 && result != (size_t)-1) {
        t_error("%s wcsrtombs with NULL dst should return 0 or -1, got %zu\n", __func__, result);
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
    wcsrtombs_1100();
    wcsrtombs_1200();
    wcsrtombs_1300();
    wcsrtombs_1400();
    wcsrtombs_1500();
    wcsrtombs_1600();
    wcsrtombs_1700();
    wcsrtombs_1800();
    wcsrtombs_1900();
    wcsrtombs_2000();
    wcsrtombs_2100();
    wcsrtombs_2200();
    wcsrtombs_2300();
    wcsrtombs_2400();
    wcsrtombs_2500();
    wcsrtombs_2600();
    wcsrtombs_2700();
    wcsrtombs_2800();
    wcsrtombs_2900();
    return t_status;
}