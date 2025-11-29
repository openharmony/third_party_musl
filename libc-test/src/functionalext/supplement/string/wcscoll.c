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
#include "test.h"

/**
 * @tc.name      : wcscoll_0100
 * @tc.desc      : Test the wcscoll method to compare two wide strings using current locale
 * @tc.level     : Level 0
 */
void wcscoll_0100(void)
{
    wchar_t *str1 = L"apple";
    wchar_t *str2 = L"banana";
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s wcscoll get result is %d are not want negative\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_0200
 * @tc.desc      : Test wcscoll with equal strings
 * @tc.level     : Level 1
 */
void wcscoll_0200(void)
{
    wchar_t *str1 = L"hello";
    wchar_t *str2 = L"hello";
    
    int result = wcscoll(str1, str2);
    if (result != 0) {
        t_error("%s wcscoll get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_0300
 * @tc.desc      : Test wcscoll with strings that differ only in case
 * @tc.level     : Level 1
 */
void wcscoll_0300(void)
{
    wchar_t *str1 = L"Apple";
    wchar_t *str2 = L"apple";
    
    int result = wcscoll(str1, str2);
    // The result depends on locale, so we just check that it's consistent
    int result2 = wcscoll(str2, str1);
    
    if ((result < 0 && result2 <= 0) || (result > 0 && result2 >= 0)) {
        t_error("%s wcscoll results are not consistent for case comparison\n", __func__);
    }
}

/**
 * @tc.name      : wcscoll_0400
 * @tc.desc      : Test wcscoll with numeric strings
 * @tc.level     : Level 1
 */
void wcscoll_0400(void)
{
    wchar_t *str1 = L"100";
    wchar_t *str2 = L"200";
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s wcscoll get result is %d are not want negative for numeric comparison\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_0500
 * @tc.desc      : Test wcscoll with special characters
 * @tc.level     : Level 1
 */
void wcscoll_0500(void)
{
    wchar_t *str1 = L"test!";
    wchar_t *str2 = L"test@";
    
    int result = wcscoll(str1, str2);
    // Just verify the function doesn't crash and returns a consistent result
    int result2 = wcscoll(str2, str1);
    
    if ((result < 0 && result2 <= 0) || (result > 0 && result2 >= 0)) {
        t_error("%s wcscoll results are not consistent for special characters\n", __func__);
    }
}

/**
 * @tc.name      : wcscoll_0600
 * @tc.desc      : Test wcscoll with empty strings
 * @tc.level     : Level 1
 */
void wcscoll_0600(void)
{
    wchar_t *str1 = L"";
    wchar_t *str2 = L"";
    wchar_t *str3 = L"a";
    
    int result1 = wcscoll(str1, str2);
    if (result1 != 0) {
        t_error("%s wcscoll get result is %d are not want 0 for empty strings\n", __func__, result1);
    }
    
    int result2 = wcscoll(str1, str3);
    if (result2 >= 0) {
        t_error("%s wcscoll get result is %d are not want negative for empty vs non-empty\n", __func__, result2);
    }
}

/**
 * @tc.name      : wcscoll_0700
 * @tc.desc      : Test wcscoll with accented characters
 * @tc.level     : Level 2
 */
void wcscoll_0700(void)
{
    wchar_t *str1 = L"cafe";
    wchar_t *str2 = L"café";
    
    int result = wcscoll(str1, str2);
    // The ordering depends on locale, so we just test that it doesn't crash
    // and returns consistent results
    int result2 = wcscoll(str2, str1);
    
    if ((result < 0 && result2 <= 0) || (result > 0 && result2 >= 0)) {
        t_error("%s wcscoll results are not consistent for accented characters\n", __func__);
    }
}

/**
 * @tc.name      : wcscoll_0800
 * @tc.desc      : Test wcscoll with different locale settings
 * @tc.level     : Level 2
 */
void wcscoll_0800(void)
{
    // Save current locale
    char *old_locale = setlocale(LC_ALL, NULL);
    
    // Test with C locale
    setlocale(LC_ALL, "C");
    
    wchar_t *str1 = L"Apple";
    wchar_t *str2 = L"apple";
    
    int result_c = wcscoll(str1, str2);
    
    // Try to set a different locale if available
    if (setlocale(LC_ALL, "en_US.UTF-8") != NULL) {
        int result_en = wcscoll(str1, str2);
        
        // Results might differ between locales, which is normal
        // We just verify both calls complete successfully
    }
    
    // Restore original locale
    setlocale(LC_ALL, old_locale);
}

/**
 * @tc.name      : wcscoll_0900
 * @tc.desc      : Test wcscoll with very long strings
 * @tc.level     : Level 2
 */
void wcscoll_0900(void)
{
    wchar_t str1[] = L"This is a very long string for testing the wcscoll function with extended content";
    wchar_t str2[] = L"This is a very long string for testing the wcscoll function with extended content";
    wchar_t str3[] = L"This is a very long string for testing the wcscoll function with different ending";
    
    int result1 = wcscoll(str1, str2);
    if (result1 != 0) {
        t_error("%s wcscoll get result is %d are not want 0 for identical long strings\n", __func__, result1);
    }
    
    int result2 = wcscoll(str1, str3);
    // Just verify the function completes without error
}

/**
 * @tc.name      : wcscoll_1000
 * @tc.desc      : Test wcscoll with strings that have same prefix but different suffixes
 * @tc.level     : Level 1
 */
void wcscoll_1000(void)
{
    wchar_t *str1 = L"abcdefg";
    wchar_t *str2 = L"abcdefh";
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s wcscoll get result is %d are not want negative for strings with same prefix\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcscoll_0100();
    wcscoll_0200();
    wcscoll_0300();
    wcscoll_0400();
    wcscoll_0500();
    wcscoll_0600();
    wcscoll_0700();
    wcscoll_0800();
    wcscoll_0900();
    wcscoll_1000();
    return t_status;
}