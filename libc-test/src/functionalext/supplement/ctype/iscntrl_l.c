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

const int COUNT = 33;
const int SIZE = 128;
const int CN_COUNT = 66;
const int CN_SIZE = 256;
const int RET_TRUE = 1;
const int RET_FALSE = 0;

/**
 * @tc.name      : iscntrl_l_0100
 * @tc.desc      : Verify iscntrl_l process success. when using the en_US.UTF-8 character set.
 *                 the parameter c is a control character,
 *                 and the input character is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_0100(void)
{
    int ret = RET_FALSE;
    // arr : control character array
    const char arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10,
                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x7f};
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for(unsigned int i = 0; i < sizeof(arr)/sizeof(char); i++) {
        ret = iscntrl_l(arr[i], m_locale);
        if(!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0200
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a special character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0200(void)
{
    int ret = RET_FALSE;
    const char *str = "!@#$%^(){}[]";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0300
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a number character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0300(void)
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

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0400
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is an English letter,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0400(void)
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

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0500
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 Determine the number in the ascii code table.
 * @tc.level     : Level 1
 */
void iscntrl_l_0500(void)
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
        ret = iscntrl_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(__FUNCTION__, total, COUNT);
    freelocale(m_locale);
    m_locale = NULL;
}


/**
 * @tc.name      : iscntrl_l_0600
 * @tc.desc      : Verify iscntrl_l process success. when using the en_US.UTF-8 character set.
 *                 the parameter c is a wide char control character,
 *                 and the input character is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_0600(void)
{
    int ret = RET_FALSE;
    // arr : control character array
    const wchar_t arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10,
                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x7f};

    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for(unsigned int i = 0; i < sizeof(arr)/sizeof(wchar_t); i++) {
        ret = iscntrl_l(arr[i], m_locale);
        if(!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0700
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a wide char special character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0700(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"!@#$%^(){}[]";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while(*p && *p != L'\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0800
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a wide char number character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0800(void)
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

    while(*p && *p != L'\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_0900
 * @tc.desc      : Verify iscntrl_l process fail when using the en_US.UTF-8 character set.
 *                 the parameter c is a wide char English letter,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0900(void)
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

    while(*p && *p != L'\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1000
 * @tc.desc      : Verify iscntrl_l process success. when using the zh_CN character set.
 *                 the parameter c is a control character,
 *                 and the input character is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_1000(void)
{
    int ret = RET_FALSE;
    // arr : control character array
    const char arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10,
                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x7f};

    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for(unsigned int i = 0; i < sizeof(arr)/sizeof(char); i++) {
        ret = iscntrl_l(arr[i], m_locale);
        if(!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1100
 * @tc.desc      : Verify iscntrl_l process fail when using the zh_CN character set.
 *                 the parameter c is a special character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_1100(void)
{
    int ret = RET_FALSE;
    const char *str = "!@#$%^(){}[]";
    const char *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1200
 * @tc.desc      : Verify iscntrl_l process fail when using the zh_CN character set.
 *                 the parameter c is a number character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_1200(void)
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

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1300
 * @tc.desc      : Verify iscntrl_l process fail when using the zh_CN character set.
 *                 the parameter c is a English letter,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_1300(void)
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

    while(*p && *p != '\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1400
 * @tc.desc      : Verify iscntrl_l process fail when using the zh_CN character set.
 *                 Determine the number in unicode table 0 ~ 256.
 * @tc.level     : Level 1
 */
void iscntrl_l_1400(void)
{
    int ret = RET_FALSE;
    int total = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
 
    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for (int i = 0; i < CN_SIZE; i++) {
        ret = iscntrl_l(i, m_locale);
        if (ret) {
            total++;
        }
    }

    EXPECT_EQ(__FUNCTION__, total, CN_COUNT);

    freelocale(m_locale);
    m_locale = NULL;
}


/**
 * @tc.name      : iscntrl_l_1500
 * @tc.desc      : Verify iscntrl_l process success. when using the zh_CN character set.
 *                 the parameter c is a wide char control character,
 *                 and the input character is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_1500(void)
{
    int ret = RET_FALSE;
    const wchar_t arr[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10,
                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x7f};

    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    for(int i = 0; i < sizeof(arr)/sizeof(wchar_t); i++) {
        ret = iscntrl_l(arr[i], m_locale);
        if(!ret) {
            t_printf("Test case %s failed while testing character, index:%d\n", __FUNCTION__, i);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_TRUE);
    }
    
    freelocale(m_locale);
    m_locale = NULL;
}

/**
 * @tc.name      : iscntrl_l_1600
 * @tc.desc      : Verify iscntrl_l process fail when using the zh_CN character set.
 *                 the parameter c is a wide char special character,
 *                 and the input character is judged to be not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_1600(void)
{
    int ret = RET_FALSE;
    const wchar_t *str = L"!@#$%^(){}[]";
    const wchar_t *p = str;
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);

    if (!m_locale) {
        t_printf("Test case %s failed to call function newlocale\n", __FUNCTION__);
        EXPECT_NE(__FUNCTION__, ret, RET_FALSE);
        return ;
    }

    while(*p && *p != L'\0') {
        ret = iscntrl_l(*p, m_locale);
        if(ret) {
            t_printf("Test case %s failed while testing character %c\n", __FUNCTION__, *p);
        }
        EXPECT_EQ(__FUNCTION__, ret, RET_FALSE);
        *p++;
    }

    freelocale(m_locale);
    m_locale = NULL;
}

int main(void)
{
    iscntrl_l_0100();
    iscntrl_l_0200();
    iscntrl_l_0300();
    iscntrl_l_0400();
    iscntrl_l_0500();
    iscntrl_l_0600();
    iscntrl_l_0700();
    iscntrl_l_0800();
    iscntrl_l_0900();
    iscntrl_l_1000();
    iscntrl_l_1100();
    iscntrl_l_1200();
    iscntrl_l_1300();
    iscntrl_l_1400();
    iscntrl_l_1500();
    iscntrl_l_1600();
    return t_status;
}