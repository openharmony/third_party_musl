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

const int COUNT = 149016;
const int SIZE = 1114111;  // unicode max: 10FFFF
/**
 * @tc.name      : iswprint_0100
 * @tc.desc      : Determine whether the incoming wide character can be printed by the iswprint method
 * @tc.level     : Level 1
 */
void iswprint_0100(void)
{
    wchar_t ch = L'9';
    int result = iswprint(ch);
    if (result == 0) {
        t_error("%s iswprint get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswprint_0200
 * @tc.desc      : Test the iswprint method by passing in a non-printable wide character
 * @tc.level     : Level 1
 */
void iswprint_0200(void)
{
    wchar_t ch = L'\n';
    int result = iswprint(ch);
    if (result != 0) {
        t_error("%s iswprint get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswprint_l_0100
 * @tc.desc      : Whether the characters in a wide string are printable characters
 * @tc.level     : Level 0
 */
void iswprint_l_0100(void)
{
    const wchar_t *str = L"1Aa®Â Æ";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswprint_l(*p, NULL);
        EXPECT_EQ("iswprint_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswprint_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-printable characters
 * @tc.level     : Level 2
 */
void iswprint_l_0200(void)
{
    const wchar_t *str = L"\a\b";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswprint_l(*p, NULL);
        EXPECT_EQ("iswprint_l_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : iswprint_l_0300
 * @tc.desc      : Whether the characters in a wide string are non-printable characters
 * @tc.level     : Level 2
 */
void iswprint_l_0300(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iswprint_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswprint_l_0300", total, COUNT);
}

/**
 * @tc.name      : iswprint_l_0400
 * @tc.desc      : Whether the characters in a wide string are non-printable characters
 * @tc.level     : Level 2
 */
void iswprint_l_0400(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iswprint_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswprint_l_0400", total, COUNT);
}

int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    iswprint_0100();
    iswprint_0200();
    iswprint_l_0100();
    iswprint_l_0200();
    iswprint_l_0300();
    iswprint_l_0400();
    return t_status;
}