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
#include <locale.h>

#include "functionalext.h"
#include "test.h"

#define LOWER_WINT_COUNT 2233
#define UNICODE_SIZE 1114111
/**
 * @tc.name      : iswlower_0100
 * @tc.desc      : Use the iswlower method to determine whether the incoming wide character is a lowercase letter
 * @tc.level     : Level 0
 */
void iswlower_0100(void)
{
    wchar_t ch = L'a';
    int result = iswlower(ch);
    if (result == 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_0200
 * @tc.desc      : Test the return value of iswlower when uppercase letters are passed in
 * @tc.level     : Level 1
 */
void iswlower_0200(void)
{
    wchar_t ch = L'A';
    int result = iswlower(ch);
    if (result != 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_0300
 * @tc.desc      : Test the return value of iswlower when passing in a number
 * @tc.level     : Level 1
 */
void iswlower_0300(void)
{
    wchar_t ch = L'1';
    int result = iswlower(ch);
    if (result != 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_l_0100
 * @tc.desc      : Whether the characters in a wide string is lower character
 * @tc.level     : Level 0
 */
void iswlower_l_0100(void)
{
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyz";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, NULL);
        EXPECT_EQ("iswlower_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswlower_l_0101
 * @tc.desc      : Whether the characters in a wide string is lower character
 * @tc.level     : Level 0
 */
void iswlower_l_0101(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyz";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, m_locale);
        EXPECT_EQ("iswlower_l_0101", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswlower_l_0102
 * @tc.desc      : Whether the characters in a wide string is lower character
 * @tc.level     : Level 0
 */
void iswlower_l_0102(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyz";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, m_locale);
        EXPECT_EQ("iswlower_l_0102", ret, ONREXPECT);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswlower_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0200(void)
{
    const wchar_t *str = L"12ABC!@#";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, NULL);
        EXPECT_EQ("iswlower_l_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : iswlower_l_0201
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0201(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"12ABC!@#";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, m_locale);
        EXPECT_EQ("iswlower_l_0201", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswlower_l_0202
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0202(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    const wchar_t *str = L"12ABC!@#";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, m_locale);
        EXPECT_EQ("iswlower_l_0202", ret, CMPFLAG);
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : iswlower_l_0300
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0300(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswlower_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswlower_l_0300", total, LOWER_WINT_COUNT);
}

/**
 * @tc.name      : iswlower_l_0400
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0400(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswlower_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswlower_l_0400", total, LOWER_WINT_COUNT);
}

int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    iswlower_0100();
    iswlower_0200();
    iswlower_0300();
    iswlower_l_0100();
    iswlower_l_0101();
    iswlower_l_0102();
    iswlower_l_0200();
    iswlower_l_0201();
    iswlower_l_0202();
    iswlower_l_0300();
    iswlower_l_0400();
    return t_status;
}