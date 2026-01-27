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
#include <stdlib.h>
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
    if (old_locale == NULL) {
        t_error("%s setlocale failed to get current locale\n", __func__);
        return;
    }
    
    // Test with C locale
    if (setlocale(LC_ALL, "C") == NULL) {
        t_error("%s setlocale failed to set C locale\n", __func__);
        if (setlocale(LC_ALL, old_locale) == NULL) {
            t_error("%s setlocale failed to restore original locale\n", __func__);
        }
        return;
    }
    
    wchar_t *str1 = L"Apple";
    wchar_t *str2 = L"apple";
    
    int result_c = wcscoll(str1, str2);
    
    // Try to set a different locale if available
    if (setlocale(LC_ALL, "en_US.UTF-8") != NULL) {
        int result_en = wcscoll(str1, str2);
        
        // Results might differ between locales, which is normal
        // We just verify both calls complete successfully
    } else {
        t_error("%s setlocale failed to set en_US.UTF-8 locale\n", __func__);
    }
    
    // Restore original locale
    if (setlocale(LC_ALL, old_locale) == NULL) {
        t_error("%s setlocale failed to restore original locale\n", __func__);
    }
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

/**
 * @tc.name      : wcscoll_1100
 * @tc.desc      : Test wcscoll with NULL pointer 
 * @tc.level     : Level 3
 */
void wcscoll_1100(void)
{
    wchar_t *str1 = NULL;
    wchar_t *str2 = L"test";
    
    int result = wcscoll(str1, str2);
    (void)result;  
    printf("%s: called wcscoll with NULL pointer, no crash (system-dependent)\n", __func__);
}

/**
 * @tc.name      : wcscoll_1200
 * @tc.desc      : Test wcscoll with one empty string and one long string 
 * @tc.level     : Level 2
 */
void wcscoll_1200(void)
{
    wchar_t *str1 = L"";
    wchar_t str2[101];
    
    if (memset_s(str2, sizeof(str2), (int)L'a', 100 * sizeof(wchar_t)) != 0) {
        t_error("%s: memset_s failed to fill wide string", __func__);
        return;  
    }
    
    str2[100] = L'\0';
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s: wcscoll returns %d (expected negative for empty vs long string)", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_1300
 * @tc.desc      : Test wcscoll with Chinese characters 
 * @tc.level     : Level 2
 */
void wcscoll_1300(void)
{
    wchar_t *str1 = L"苹果";  
    wchar_t *str2 = L"香蕉";  
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for Chinese characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_1400
 * @tc.desc      : Test wcscoll with strings of different lengths 
 * @tc.level     : Level 1
 */
void wcscoll_1400(void)
{
    wchar_t *str1 = L"abc";
    wchar_t *str2 = L"abcd";  
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s: wcscoll returns %d (expected negative for 'abc' < 'abcd')", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_1500
 * @tc.desc      : Test wcscoll with control characters 
 * @tc.level     : Level 3
 */
void wcscoll_1500(void)
{
    wchar_t *str1 = L"\ttest";  
    wchar_t *str2 = L"\ntest";  
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for control characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_1600
 * @tc.desc      : Test wcscoll with mixed case and numbers 
 * @tc.level     : Level 1
 */
void wcscoll_1600(void)
{
    wchar_t *str1 = L"Test123";
    wchar_t *str2 = L"test456";
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for mixed case+numbers", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_1700
 * @tc.desc      : Test wcscoll with right-to-left (RTL) characters 
 * @tc.level     : Level 3
 */
void wcscoll_1700(void)
{
    wchar_t *str1 = L"السلام";  
    wchar_t *str2 = L"العمر";  
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0) || (result == 0 && result2 == 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for RTL characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_1800
 * @tc.desc      : Test wcscoll with strings containing spaces 
 * @tc.level     : Level 1
 */
void wcscoll_1800(void)
{
    wchar_t *str1 = L"hello world";  
    wchar_t *str2 = L"helloworld";   
    
    int result = wcscoll(str1, str2);

    if (result >= 0) {
        t_error("%s: wcscoll returns %d (expected negative for 'hello world' < 'helloworld')", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_1900
 * @tc.desc      : Test wcscoll with maximum wchar_t value 
 * @tc.level     : Level 3
 */
void wcscoll_1900(void)
{
    wchar_t str1[] = {0xFFFF, L'\0'};  
    wchar_t str2[] = {0xFFFE, L'\0'};  
    
    int result = wcscoll(str1, str2);
    if (result <= 0) {
        t_error("%s: wcscoll returns %d (expected positive for 0xFFFF > 0xFFFE)", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_2000
 * @tc.desc      : Test wcscoll with strings that differ in leading spaces 
 * @tc.level     : Level 1
 */
void wcscoll_2000(void)
{
    wchar_t *str1 = L"  test";  
    wchar_t *str2 = L" test";  
    
    int result = wcscoll(str1, str2);
    
    if (result <= 0) {
        t_error("%s: wcscoll returns %d (expected positive for '  test' > ' test')", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_2100
 * @tc.desc      : Test wcscoll with Japanese characters 
 * @tc.level     : Level 2
 */
void wcscoll_2100(void)
{
    wchar_t *str1 = L"りんご";  
    wchar_t *str2 = L"バナナ";  
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for Japanese characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2200
 * @tc.desc      : Test wcscoll with strings of all identical characters 
 * @tc.level     : Level 1
 */
void wcscoll_2200(void)
{
    wchar_t *str1 = L"aaaaa";  
    wchar_t *str2 = L"aaaaaa"; 
    
    int result = wcscoll(str1, str2);
    if (result >= 0) {
        t_error("%s: wcscoll returns %d (expected negative for shorter identical string)", __func__, result);
    }
}

/**
 * @tc.name      : wcscoll_2300
 * @tc.desc      : Test wcscoll with symbols and punctuation 
 * @tc.level     : Level 2
 */
void wcscoll_2300(void)
{
    wchar_t *str1 = L"!@#$";
    wchar_t *str2 = L"$#@!";
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for symbols+punctuation", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2400
 * @tc.desc      : Test wcscoll with non-BMP characters 
 * @tc.level     : Level 3
 */
void wcscoll_2400(void)
{
 
    #ifdef _WIN32
    wchar_t str1[] = {0xD83D, 0xDE00, L'\0'};  
    wchar_t str2[] = {0xD83D, 0xDE01, L'\0'}; 
    #else
    wchar_t *str1 = L"😀";  
    wchar_t *str2 = L"😁"; 
    #endif
    
    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);
    
    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for non-BMP characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2500
 * @tc.desc      : Test wcscoll with case-insensitive locale
 * @tc.level     : Level 2
 */
void wcscoll_2500(void)
{

    char *old_locale = setlocale(LC_ALL, NULL);
    if (old_locale == NULL) {
        t_error("%s: failed to get current locale (setlocale return NULL)", __func__);
        return;
    }

    char *dup_locale = strdup(old_locale);
    if (dup_locale == NULL) {
        t_error("%s: strdup failed to allocate memory for old locale", __func__);
        return;
    }

    int success = 0;
    char *locale1 = setlocale(LC_ALL, "en_US.UTF-8@ignorecase");
    char *locale2 = NULL;
    if (locale1 == NULL) {
        locale2 = setlocale(LC_ALL, "C.UTF-8@ignorecase");
    }

    if (locale1 != NULL || locale2 != NULL) {
        success = 1;
        wchar_t *str1 = L"Apple";
        wchar_t *str2 = L"apple";

        int result = wcscoll(str1, str2);
        if (result != 0) {
            t_error("%s: wcscoll returns %d (expected 0 for case-insensitive locale)", __func__, result);
        }
    }

    if (!success) {
        printf("%s: case-insensitive locale not available, skip test\n", __func__);
    }

    if (setlocale(LC_ALL, dup_locale) == NULL) {
        t_error("%s: failed to restore original locale (%s)", __func__, dup_locale);
    }

    free(dup_locale);
}

/**
 * @tc.name      : wcscoll_2600
 * @tc.desc      : Test wcscoll with Korean characters
 * @tc.level     : Level 2
 */
void wcscoll_2600(void)
{
    wchar_t *str1 = L"사과";
    wchar_t *str2 = L"바나나";

    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);

    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for Korean characters", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2700
 * @tc.desc      : Test wcscoll with strings containing tab characters
 * @tc.level     : Level 2
 */
void wcscoll_2700(void)
{
    wchar_t *str1 = L"test\tvalue";
    wchar_t *str2 = L"test value";

    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);

    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0) || (result == 0 && result2 == 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for tab vs space", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2800
 * @tc.desc      : Test wcscoll with numeric ordering vs lexicographic
 * @tc.level     : Level 1
 */
void wcscoll_2800(void)
{
    wchar_t *str1 = L"item9";
    wchar_t *str2 = L"item10";

    int result = wcscoll(str1, str2);
    int result2 = wcscoll(str2, str1);

    if (!((result < 0 && result2 > 0) || (result > 0 && result2 < 0))) {
        t_error("%s: inconsistent results (result1=%d, result2=%d) for numeric strings", __func__, result, result2);
    }
}

/**
 * @tc.name      : wcscoll_2900
 * @tc.desc      : Test wcscoll with strings containing only digits
 * @tc.level     : Level 1
 */
void wcscoll_2900(void)
{
    wchar_t *str1 = L"123";
    wchar_t *str2 = L"456";
    wchar_t *str3 = L"123";

    int result1 = wcscoll(str1, str2);
    if (result1 >= 0) {
        t_error("%s: wcscoll returns %d (expected negative for '123' < '456')", __func__, result1);
    }

    int result2 = wcscoll(str1, str3);
    if (result2 != 0) {
        t_error("%s: wcscoll returns %d (expected 0 for equal digit strings)", __func__, result2);
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
    wcscoll_1100();
    wcscoll_1200();
    wcscoll_1300();
    wcscoll_1400();
    wcscoll_1500();
    wcscoll_1600();
    wcscoll_1700();
    wcscoll_1800();
    wcscoll_1900();
    wcscoll_2000();
    wcscoll_2100();
    wcscoll_2200();
    wcscoll_2300();
    wcscoll_2400();
    wcscoll_2500();
    wcscoll_2600();
    wcscoll_2700();
    wcscoll_2800();
    wcscoll_2900();
    return t_status;
}

