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
#include <locale.h>
#include <wctype.h>

#include "functionalext.h"
#include "test.h"

#define DIGIT_WINT_COUNT 680
#define UNICODE_SIZE 1114111
#define MAX_VALUE 4294967295
/**
 * @tc.name      : iswdigit_0100
 * @tc.desc      : Use the iswdigit function to determine whether the incoming wide character is a decimal number
 * @tc.level     : Level 0
 */
void iswdigit_0100(void)
{
    wchar_t ch = L'9';
    int result = iswdigit(ch);
    if (result == 0) {
        t_error("%s iswdigit get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswdigit_0200
 * @tc.desc      : Test iswdigit when a non-decimal digit is passed in
 * @tc.level     : Level 1
 */
void iswdigit_0200(void)
{
    wchar_t ch = L'A';
    int result = iswdigit(ch);
    if (result != 0) {
        t_error("%s iswdigit get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswdigit_l_0100
 * @tc.desc      : Whether a character in a wide string is a number character
 * @tc.level     : Level 0
 */
void iswdigit_l_0100(void)
{
    const wchar_t *str = L"1234567890";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, NULL);
        EXPECT_EQ("iswdigit_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswdigit_l_0101
 * @tc.desc      : Whether a character in a wide string is a number character
 * @tc.level     : Level 0
 */
void iswdigit_l_0101(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"1234567890";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, m_locale);
        EXPECT_EQ("iswdigit_l_0101", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswdigit_l_0102
 * @tc.desc      : Whether a character in a wide string is a number character
 * @tc.level     : Level 0
 */
void iswdigit_l_0102(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    const wchar_t *str = L"1234567890";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, m_locale);
        EXPECT_EQ("iswdigit_l_0102", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswdigit_l_0200
 * @tc.desc      : Whether a character in a wide string is a non-number character
 * @tc.level     : Level 2
 */
void iswdigit_l_0200(void)
{
    const wchar_t *str = L"ewqWRE!~*Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, NULL);
        EXPECT_EQ("iswdigit_l_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : iswdigit_l_0201
 * @tc.desc      : Whether a character in a wide string is a non-number character
 * @tc.level     : Level 2
 */
void iswdigit_l_0201(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"ewqWRE!~*Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, m_locale);
        EXPECT_EQ("iswdigit_l_0201", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswdigit_l_0202
 * @tc.desc      : Whether a character in a wide string is a non-number character
 * @tc.level     : Level 2
 */
void iswdigit_l_0202(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    const wchar_t *str = L"ewqWRE!~*Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswdigit_l(*p, m_locale);
        EXPECT_EQ("iswdigit_l_0202", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswdigit_l_0300
 * @tc.desc      : Whether a character in a wide string is a non-number character
 * @tc.level     : Level 2
 */
void iswdigit_l_0300(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswdigit_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswdigit_l_0300", total, DIGIT_WINT_COUNT);
}

/**
 * @tc.name      : iswdigit_l_0400
 * @tc.desc      : Whether a character in a wide string is a non-number character
 * @tc.level     : Level 2
 */
void iswdigit_l_0400(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswdigit_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswdigit_l_0400", total, DIGIT_WINT_COUNT);
}

/**
 * @tc.name      : iswdigit_l_0500
 * @tc.desc      : Whether a character in a the maximum value of int is not a letter
 * @tc.level     : Level 1
 */
void iswdigit_l_0500(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int ret = iswdigit_l(MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswdigit_l_0500", ret, CMPFLAG);
}

/**
* @tc.name      : iswdigit_l_0600
* @tc.desc      : Whether a character in a the maximum value of int is not a letter
* @tc.level     : Level 1
*/
void iswdigit_l_0600(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswdigit_l(MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswdigit_l_0600", ret, CMPFLAG);
}

int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    iswdigit_0100();
    iswdigit_0200();
    iswdigit_l_0100();
    iswdigit_l_0101();
    iswdigit_l_0102();
    iswdigit_l_0200();
    iswdigit_l_0201();
    iswdigit_l_0202();
    iswdigit_l_0300();
    iswdigit_l_0400();
    iswdigit_l_0500();
    iswdigit_l_0600();
    return t_status;
}