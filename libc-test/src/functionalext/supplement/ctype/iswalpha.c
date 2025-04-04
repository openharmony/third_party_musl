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

#define ALPHA_WINT_COUNT 136104
#define UNICODE_SIZE 1114111
/**
 * @tc.name      : iswalpha_l_0100
 * @tc.desc      : Whether the characters in a wide string are letters
 * @tc.level     : Level 0
 */
void iswalpha_l_0100(void)
{
    const wchar_t *str = L"aBcrWER";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswalpha_l(*p, NULL);
        EXPECT_EQ("iswalpha_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswalpha_l_0200
 * @tc.desc      : Whether a character in a wide string is not a letter
 * @tc.level     : Level 2
 */
void iswalpha_l_0200(void)
{
    const wchar_t *str = L"2!~*";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswalpha_l(*p, NULL);
        EXPECT_EQ("iswalpha_l_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : iswalpha_l_0300
 * @tc.desc      : Whether a character in a wide string is not a letter
 * @tc.level     : Level 1
 */
void iswalpha_l_0300(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswalpha_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswalpha_l_0300", total, ALPHA_WINT_COUNT);
}

/**
 * @tc.name      : iswalpha_l_0400
 * @tc.desc      : Whether a character in a wide string is not a letter
 * @tc.level     : Level 1
 */
void iswalpha_l_0400(void)
{
    locale_t m_locale = newlocale(LC_CTYPE_MASK, "zh_CN", NULL);
    int total = 0;
    for (int i = 0; i < UNICODE_SIZE; i++) {
        int ret = iswalpha_l(i, m_locale);
        if (ret) {
            total++;
        }
    }
    freelocale(m_locale);
    EXPECT_EQ("iswalpha_l_0300", total, ALPHA_WINT_COUNT);
}

int main(void)
{
    set_wctype_icu_enable();
    iswalpha_l_0100();
    iswalpha_l_0200();
    iswalpha_l_0300();
    iswalpha_l_0400();
    return t_status;
}