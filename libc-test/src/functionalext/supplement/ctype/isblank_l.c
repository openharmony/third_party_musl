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

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

const int COUNT = 2;
const int SIZE = 128;
const int RET_TRUE = 1;
const int RET_FALSE = 0;

/**
 * @tc.name      : isblank_l_0100
 * @tc.desc      : Verify isblank_l process success. when using the en_US.UTF-8 character set.
 *                 the parameter c is one of a space in {'\t', ' '}),
 *                 and the input character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_l_0100(void)
{
    int ret = RET_FALSE;
    const int arr[] = {'\t', ' '};
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (unsigned int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        ret = isblank_l(arr[i], m_locale);
        if (!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0200
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is control character,
 *                  which determines that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0200(void)
{
    int ret = RET_FALSE;
    const char *str = "\r\n\b\0\f";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0300
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is an English letter,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0300(void)
{
    int ret = RET_FALSE;
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0400
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is numeric,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0400(void)
{
    int ret = RET_FALSE;
    const char *str = "1234567890";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0500
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is a special character,
 *           and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0500(void)
{
    int ret = RET_FALSE;
    const char *str = "!@#$%^()+{}[]";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0600
 * @tc.desc      : Verify isblank_l process when using the en_US.UTF-8 character set
 *                 Determine the number of space characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isblank_l_0600(void)
{
    int ret = RET_FALSE;
    int total = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        ret = isblank_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(__FUNCTION__, total, COUNT);

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0700
 * @tc.desc      : Verify isblank_l process success. when using the en_US.UTF-8 character set.
 *                 the parameter c is one of a space in {L'\t', L' '}),
 *                 and the input wide character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_l_0700(void)
{
    int ret = RET_FALSE;
    const wchar_t arr[] = {L'\t', L' '};
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (unsigned int i = 0; i < sizeof(arr) / sizeof(wchar_t); i++) {
        ret = isblank_l(arr[i], m_locale);
        if (!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0800
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a wide char control character,
 *                  which determines that the input wide character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0800(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"\r\n\b\0\f";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != L'\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_0900
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is a wide char English letter,
 *                 and it is judged that the input wide character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0900(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != L'\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1000
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is a wide char alphanumeric,
 *                 and it is judged that the input wide character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1000(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"1234567890";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != L'\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1100
 * @tc.desc      : Verify isblank_l process fail when using the en_US.UTF-8 character set
 *                 When the parameter c is a wide char special character,
 *           and it is judged that the input wide character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1100(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"!@#$%^()+{}[]";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != L'\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1200
 * @tc.desc      : Verify isblank_l process success. when using the zh_CN character set.
 *                 the parameter c is one of a space in {'\t', ' '}),
 *                 and the input character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_l_1200(void)
{
    int ret = RET_FALSE;
    const int arr[] = {'\t', ' '};
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (unsigned int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        ret = isblank_l(arr[i], m_locale);
        if (!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1300
 * @tc.desc      : Verify isblank_l process fail when using the zh_CN character set.
 *                 the parameter c is control character,
 *                  which determines that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1300(void)
{
    int ret = RET_FALSE;
    const char *str = "\r\n\b\0\f";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1400
 * @tc.desc      : Verify isblank_l process fail when using the zh_CN character set
 *                 When the parameter c is an English letter,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1400(void)
{
    int ret = RET_FALSE;
    const char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1500
 * @tc.desc      : Verify isblank_l process fail when using the zh_CN character set
 *                 When the parameter c is alphanumeric,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1500(void)
{
    int ret = RET_FALSE;
    const char *str = "1234567890";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1600
 * @tc.desc      : Verify isblank_l process fail when using the zh_CN character set
 *                 When the parameter c is a special character,
 *           and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_1600(void)
{
    int ret = RET_FALSE;
    const char *str = "!@#$%^()+{}[]";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while (*p && *p != '\0') {
        ret = isblank_l(*p, m_locale);
        if (ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : isblank_l_1700
 * @tc.desc      : Verify isblank_l process when using the zh_CN character set
 *                 Determine the number of space characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isblank_l_1700(void)
{
    int ret = RET_FALSE;
    int total = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < SIZE; i++) {
        ret = isblank_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(__FUNCTION__, total, COUNT);

    freelocale(m_locale);
    m_locale = NULL;
}

int main(void)
{
    isblank_l_0100();
    isblank_l_0200();
    isblank_l_0300();
    isblank_l_0400();
    isblank_l_0500();
    isblank_l_0600();
    isblank_l_0700();
    isblank_l_0800();
    isblank_l_0900();
    isblank_l_1000();
    isblank_l_1100();
    isblank_l_1200();
    isblank_l_1300();
    isblank_l_1400();
    isblank_l_1500();
    isblank_l_1600();
    isblank_l_1700();
    return t_status;
}