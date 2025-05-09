/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "functionalext.h"

#define BLANK_WINT_COUNT 18
#define UNICODE_SIZE 1114111
#define MAX_VALUE 4294967295

/**
 * @tc.name      : iswblank_l_0100
 * @tc.desc      : Whether a character in a wide string is a blank character
 * @tc.level     : Level 0
 */
void iswblank_l_0100(void)
{
    const wchar_t *str = L" \t";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, NULL);
        EXPECT_EQ("iswblank_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswblank_l_0101
 * @tc.desc      : Whether a character in a wide string is a blank character
 * @tc.level     : Level 0
 */
void iswblank_l_0101(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L" \t";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, m_locale);
        EXPECT_EQ("iswblank_l_0101", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswblank_l_0102
 * @tc.desc      : Whether a character in a wide string is a blank character
 * @tc.level     : Level 0
 */
void iswblank_l_0102(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    const wchar_t *str = L" \t";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, m_locale);
        EXPECT_EQ("iswblank_l_0102", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswblank_l_0200
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 2
 */
void iswblank_l_0200(void)
{
    const wchar_t *str = L"2!~*3Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, NULL);
        EXPECT_EQ("iswblank_l_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : iswblank_l_0201
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 2
 */
void iswblank_l_0201(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"2!~*3Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, m_locale);
        EXPECT_EQ("iswblank_l_0201", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswblank_l_0202
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 2
 */
void iswblank_l_0202(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    const wchar_t *str = L"2!~*3Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank_l(*p, m_locale);
        EXPECT_EQ("iswblank_l_0202", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswblank_l_0300
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 1
 */
void iswblank_l_0300(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswblank_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswblank_l_0300", total, BLANK_WINT_COUNT);
}

/**
 * @tc.name      : iswblank_l_0400
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 1
 */
void iswblank_l_0400(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswblank_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswblank_l_0400", total, BLANK_WINT_COUNT);
}

/**
 * @tc.name      : iswblank_l_0500
 * @tc.desc      : Whether a character in a the maximum value of int is not a letter
 * @tc.level     : Level 1
 */
 void iswblank_l_0500(void)
 {
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int ret = iswblank_l(MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswblank_l_0500", ret, CMPFLAG);
 }

/**
* @tc.name      : iswblank_l_0600
* @tc.desc      : Whether a character in a the maximum value of int is not a letter
* @tc.level     : Level 1
*/
void iswblank_l_0600(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswblank_l(MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswblank_l_0600", ret, CMPFLAG);
}

/**
 * @tc.name      : iswblank_0100
 * @tc.desc      : Whether a character in a wide string is a blank character
 * @tc.level     : Level 0
 */
void iswblank_0100(void)
{
    const wchar_t *str = L" \t";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank(*p);
        EXPECT_EQ("iswblank_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswblank_0200
 * @tc.desc      : Whether the characters in a wide string is blank characters
 * @tc.level     : Level 2
 */
void iswblank_0200(void)
{
    const wchar_t *str = L"2!~*3Ad";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswblank(*p);
        EXPECT_EQ("iswblank_0200", ret, CMPFLAG);
    }
}

int main(void)
{
    set_wctype_icu_enable();
    iswblank_l_0100();
    iswblank_l_0101();
    iswblank_l_0102();
    iswblank_l_0200();
    iswblank_l_0201();
    iswblank_l_0202();
    iswblank_l_0300();
    iswblank_l_0400();
    iswblank_l_0500();
    iswblank_l_0600();

    iswblank_0100();
    iswblank_0200();
    return t_status;
}