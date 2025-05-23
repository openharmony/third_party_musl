
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
#include <wchar.h>
#include <wctype.h>
#include "functionalext.h"

const int COUNT = 94;
const int SIZE = 128;
#define ALNUM_WINT_COUNT 286465
#define UNICODE_SIZE 1114111
#define MAX_VALUE 4294967295

/*
 * @tc.name      : iswgraph_l_0100
 * @tc.desc      : Verify iswgraph_l process success. When that the entered character is a letter (parameter is 'a'
 *                 has a graphical representation).
 * @tc.level     : Level 0
 */
void iswgraph_l_0100(void)
{
    wchar_t c = 'a';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswgraph_l(c, m_locale);
    EXPECT_NE("iswgraph_l_0100", ret, 0);
}

/*
 * @tc.name      : iswgraph_l_0200
 * @tc.desc      : Verify iswgraph_l process success. When that the entered character is not a letter (parameter is '!'
 *                 has a graphical representation).
 * @tc.level     : Level 2
 */
void iswgraph_l_0200(void)
{
    wchar_t c = '!';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswgraph_l(c, m_locale);
    EXPECT_NE("iswgraph_l_0200", ret, 0);
}

/*
 * @tc.name      : iswgraph_l_0300
 * @tc.desc      : Verify iswgraph_l process success. When that the entered character is not has a graphical representation.
 * @tc.level     : Level 2
 */
void iswgraph_l_0300(void)
{
    wchar_t c = ' ';
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iswgraph_l(c, m_locale);
    EXPECT_EQ("iswgraph_l_0300", ret, 0);
}

/**
 * @tc.name      : iswgraph_l_0400
 * @tc.desc      : Verify iswgraph_l process success. Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void iswgraph_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iswgraph_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswgraph_l_0400", total, COUNT);
}

/**
 * @tc.name      : iswgraph_l_0500
 * @tc.desc      : Verify iswgraph_l process success. Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void iswgraph_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswgraph_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswgraph_l_0500", total, ALNUM_WINT_COUNT);
}

/**
 * @tc.name      : iswgraph_l_0600
 * @tc.desc      : Verify iswgraph_l process success. Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void iswgraph_l_0600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswgraph_l((wchar_t)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswgraph_l_0600", total, ALNUM_WINT_COUNT);
}

/**
 * @tc.name      : iswgraph_l_0700
 * @tc.desc      : Verify iswgraph_l process success. Input parameter is the maximum value of int, please verify that the return is CMPFLAG
 *                 code table.
 * @tc.level     : Level 1
 */
 void iswgraph_l_0700(void)
 {
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswgraph_l((wchar_t)MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswgraph_l_0700", ret, CMPFLAG);
 }

 /**
 * @tc.name      : iswgraph_l_0800
 * @tc.desc      : Verify iswgraph_l process success. Input parameter is the maximum value of int, please verify that the return is CMPFLAG
 *                 code table.
 * @tc.level     : Level 1
 */
 void iswgraph_l_0800(void)
 {
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int ret = iswgraph_l((wchar_t)MAX_VALUE, m_locale);
    freelocale(m_locale);
    EXPECT_EQ("iswgraph_l_0800", ret, CMPFLAG);
 }

int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    iswgraph_l_0100();
    iswgraph_l_0200();
    iswgraph_l_0300();
    iswgraph_l_0400();
    iswgraph_l_0500();
    iswgraph_l_0600();
    iswgraph_l_0700();
    iswgraph_l_0800();
    return t_status;
}