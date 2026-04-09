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
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include "test.h"

/**
 * @tc.name      : towctrans_0100
 * @tc.desc      : Convert character of current type to lowercases
 * @tc.level     : Level 0
 */
void towctrans_0100(void)
{
    wchar_t str[] = L"ABCD";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"abcd")) {
        t_error("%s towctrans lower failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0200
 * @tc.desc      : Convert character of current type to uppercase
 * @tc.level     : Level 1
 */
void towctrans_0200(void)
{
    wchar_t str[] = L"abcd";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"ABCD")) {
        t_error("%s towctrans upper failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0300
 * @tc.desc      : Convert character of current type to uppercase
 * @tc.level     : Level 1
 */
void towctrans_0300(void)
{
    wchar_t str[] = L"Abcd";
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], 0);
    }
    if (wcscmp(str, L"Abcd")) {
        t_error("%s towctrans failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0400
 * @tc.desc      : Test towctrans with digits using tolower
 * @tc.level     : Level 1
 */
void towctrans_0400(void)
{
    wchar_t str[] = L"12345";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"12345")) {
        t_error("%s towctrans digits failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0500
 * @tc.desc      : Test towctrans with special characters using tolower
 * @tc.level     : Level 1
 */
void towctrans_0500(void)
{
    wchar_t str[] = L"@#$%^&*";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"@#$%^&*")) {
        t_error("%s towctrans special chars failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0600
 * @tc.desc      : Test towctrans with mixed string using tolower
 * @tc.level     : Level 1
 */
void towctrans_0600(void)
{
    wchar_t str[] = L"Hello World! 123";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"hello world! 123")) {
        t_error("%s towctrans mixed string failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0700
 * @tc.desc      : Test towctrans with mixed string using toupper
 * @tc.level     : Level 1
 */
void towctrans_0700(void)
{
    wchar_t str[] = L"Hello World! 123";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"HELLO WORLD! 123")) {
        t_error("%s towctrans mixed string failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0800
 * @tc.desc      : Test towctrans with already uppercase characters using toupper
 * @tc.level     : Level 1
 */
void towctrans_0800(void)
{
    wchar_t str[] = L"ABCDEF";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"ABCDEF")) {
        t_error("%s towctrans already uppercase failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0900
 * @tc.desc      : Test towctrans with already lowercase characters using tolower
 * @tc.level     : Level 1
 */
void towctrans_0900(void)
{
    wchar_t str[] = L"abcdef";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"abcdef")) {
        t_error("%s towctrans already lowercase failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1000
 * @tc.desc      : Test towctrans with single character
 * @tc.level     : Level 2
 */
void towctrans_1000(void)
{
    wchar_t c = L'A';
    wctrans_t trans = wctrans("tolower");
    wchar_t result = towctrans(c, trans);
    if (result != L'a') {
        t_error("%s towctrans single char failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1100
 * @tc.desc      : Test towctrans with empty string
 * @tc.level     : Level 2
 */
void towctrans_1100(void)
{
    wchar_t str[] = L"";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"")) {
        t_error("%s towctrans empty string failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1200
 * @tc.desc      : Test towctrans with whitespace characters
 * @tc.level     : Level 2
 */
void towctrans_1200(void)
{
    wchar_t str[] = L" \t\n\r";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L" \t\n\r")) {
        t_error("%s towctrans whitespace failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1300
 * @tc.desc      : Test towctrans with digits using toupper
 * @tc.level     : Level 1
 */
void towctrans_1300(void)
{
    wchar_t str[] = L"12345";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"12345")) {
        t_error("%s towctrans digits failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1400
 * @tc.desc      : Test towctrans with special characters using toupper
 * @tc.level     : Level 1
 */
void towctrans_1400(void)
{
    wchar_t str[] = L"@#$%^&*";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"@#$%^&*")) {
        t_error("%s towctrans special chars failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1500
 * @tc.desc      : Test towctrans with mixed case letters
 * @tc.level     : Level 1
 */
void towctrans_1500(void)
{
    wchar_t str[] = L"aBcDeFgHiJkLmNoPqRsTuVwXyZ";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"abcdefghijklmnopqrstuvwxyz")) {
        t_error("%s towctrans mixed case failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_1600
 * @tc.desc      : Test towctrans with mixed case letters to uppercase
 * @tc.level     : Level 1
 */
void towctrans_1600(void)
{
    wchar_t str[] = L"aBcDeFgHiJkLmNoPqRsTuVwXyZ";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
        t_error("%s towctrans mixed case failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    towctrans_0100();
    towctrans_0200();
    towctrans_0300();
    towctrans_0400();
    towctrans_0500();
    towctrans_0600();
    towctrans_0700();
    towctrans_0800();
    towctrans_0900();
    towctrans_1000();
    towctrans_1100();
    towctrans_1200();
    towctrans_1300();
    towctrans_1400();
    towctrans_1500();
    towctrans_1600();
    return t_status;
}