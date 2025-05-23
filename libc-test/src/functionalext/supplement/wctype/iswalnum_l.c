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

#include <locale.h>
#include <stdlib.h>
#include <wctype.h>
#include "functionalext.h"

const int COUNT = 62;
const int SIZE = 128;

#define ALNUM_WINT_COUNT 136784
#define UNICODE_SIZE 1114111
#define MAX_VALUE 4294967295

/**
 * @tc.name      : iswalnum_l_0100
 * @tc.desc      : Verify iswalnum_l process success. The parameter c is an English letter,
 *                 and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
void iswalnum_l_0100(void)
{
    wchar_t c = 'a';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswalnum_l(c, m_locale);
    EXPECT_NE("iswalnum_l_0100", ret, 0);
}

/**
 * @tc.name      : iswalnum_l_0200
 * @tc.desc      : Verify iswalnum_l process success. The parameter c is an English number,
 *                 and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
void iswalnum_l_0200(void)
{
    wchar_t c = '1';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswalnum_l('1', m_locale);
    EXPECT_NE("iswalnum_l_0200", ret, 0);
}

/**
 * @tc.name      : iswalnum_l_0300
 * @tc.desc      : Verify iswalnum_l process success. The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void iswalnum_l_0300(void)
{
    wchar_t c = '*';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswalnum_l('*', m_locale);
    EXPECT_EQ("iswalnum_l_0300", ret, 0);
}

/**
 * @tc.name      : iswalnum_l_0400
 * @tc.desc      : Verify iswalnum_l process success. Determine the number of letters and numbers in the ascii
 *                 code table.
 * @tc.level     : Level 1
 */
void iswalnum_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iswalnum_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswalnum_l_0400", total, COUNT);
}

/**
 * @tc.name      : iswalnum_l_0500
 * @tc.desc      : Verify iswalnum_l process success. Determine the number of letters and numbers in the ascii
 *                 code table.
 * @tc.level     : Level 1
 */
void iswalnum_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswalnum_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswalnum_l_0400", total, ALNUM_WINT_COUNT);
}

/**
 * @tc.name      : iswalnum_l_0600
 * @tc.desc      : Verify iswalnum_l process success. Determine the number of letters and numbers in the ascii
 *                 code table.
 * @tc.level     : Level 1
 */
void iswalnum_l_0600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswalnum_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswalnum_l_0400", total, ALNUM_WINT_COUNT);
}

/**
 * @tc.name      : iswalnum_l_0700
 * @tc.desc      : Verify iswalnum_l process success. Input parameter is the maximum value of int, please verify that the return is CMPFLAG
 * @tc.level     : Level 1
 */
 void iswalnum_l_0700(void)
 {
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswalnum_l((wchar_t)MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswalnum_l_0700", ret, CMPFLAG);
 }

 /**
 * @tc.name      : iswalnum_l_0800
 * @tc.desc      : Verify iswalnum_l process success. Input parameter is the maximum value of int, please verify that the return is CMPFLAG
 *                 code table.
 * @tc.level     : Level 1
 */
 void iswalnum_l_0800(void)
 {
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswalnum_l((wchar_t)MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswalnum_l_0800", ret, CMPFLAG);
 }

int main(void)
{
    set_wctype_icu_enable();
    iswalnum_l_0100();
    iswalnum_l_0200();
    iswalnum_l_0300();
    iswalnum_l_0400();
    iswalnum_l_0500();
    iswalnum_l_0600();
    iswalnum_l_0700();
    iswalnum_l_0800();
    return t_status;
}