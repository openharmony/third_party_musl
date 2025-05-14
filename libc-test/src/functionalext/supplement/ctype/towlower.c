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
#include <wctype.h>
#include <locale.h>
#include "test.h"

#define MAX_VALUE 4294967295
/**
 * @tc.name      : towlower_0100
 * @tc.desc      : Convert wide characters to lowercase via towlower
 * @tc.level     : Level 0
 */
void towlower_0100(void)
{
    wint_t wt = L'\u0190';
    wint_t result = towlower(wt);
    unsigned int want = 603;
    if (result != want) {
        t_error("%s towlower get result is %d are not 603\n", __func__, result);
    }
}

/**
 * @tc.name      : towlower_0200
 * @tc.desc      : Test that the incoming character exceeds the function limit
 * @tc.level     : Level 1
 */
void towlower_0200(void)
{
    wint_t wt = 0x20000;
    wint_t result = towlower(wt);
    if (result != wt) {
        t_error("%s towlower get result is %d are not 0x20000\n", __func__, result);
    }
}

/**
 * @tc.name      : towlower_l_0100
 * @tc.desc      : Verify towlower_l process success when using the en_US.UTF-8 character set.
 *                 Input parameters are Uppercase letters, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towlower_l_0100(void) 
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *upstr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *lowstr = "abcdefghijklmnopqrstuvwxyz";
    const char *up = upstr;
    const char *lp = lowstr;
    wint_t res = 0;

    if (!m_locale) {
        t_error("Test case %s failed to call function newlocale\n", __FUNCTION__);
        return;
    }

    while (up && *up != '\0' && lp && *lp != '\0') {
        res = towlower_l(*up, m_locale);
        if (res != *lp) {
            t_error("Test case %s failed, get result is %c but want get value %c\n", __FUNCTION__, (char)res, *lp);
        }
        up++;
        lp++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towlower_l_0200
 * @tc.desc      : Verify towlower_l process fail when using the en_US.UTF-8 character set.
 *                 Input parameters as numbers, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towlower_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    const char *str = "0123456789";
    const char *p = str;
    wint_t res = 0;

    if (!m_locale) {
        t_error("Test case %s failed to call function newlocale\n", __FUNCTION__);
        return;
    }

    while (p && *p != '\0') {
        res = towlower_l(*p, m_locale);
        if (res != *p) {
            t_error("Test case %s failed, result is %c but want get value %c \n", __FUNCTION__, (char)res, *p);
        }
        p++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towlower_l_0300
 * @tc.desc      : Verify towlower_l process fail when using the zh_CN character set.
 *                 Input parameters as numbers, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towlower_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const char *str = "0123456789";
    const char *p = str;
    wint_t res = 0;

    if (!m_locale) {
        t_error("Test case %s failed to call function newlocale\n", __FUNCTION__);
        return;
    }

    while (p && *p != '\0') {
        res = towlower_l(*p, m_locale);
        if (res != *p) {
            t_error("Test case %s failed, result is %c but want get value %c \n", __FUNCTION__, (char)res, *p);
        }
        p++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towlower_l_0400
 * @tc.desc      : Verify towlower_l process success when using the zh_CN character set.
 *                 Input parameters are Uppercase letters, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towlower_l_0400(void) 
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    const char *upstr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *lowstr = "abcdefghijklmnopqrstuvwxyz";
    const char *up = upstr;
    const char *lp = lowstr;
    wint_t res = 0;

    if (!m_locale) {
        t_error("Test case %s failed to call function newlocale\n", __FUNCTION__);
        return;
    }

    while (up && *up != '\0' && lp && *lp != '\0') {
        res = towlower_l(*up, m_locale);
        if (res != *lp) {
            t_error("Test case %s failed, get result is %c but want get value %c\n", __FUNCTION__, (char)res, *lp);
        }
        up++;
        lp++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towlower_l_0500
 * @tc.desc      : Verify towlower_l process success when using the zh_CN character set.
 *                 Input parameter is the maximum value of int, please verify that the return is -1
 * @tc.level     : Level 1
 */
 void towlower_l_0500(void) 
 {
    locale_t m_locale = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    wint_t res = towlower_l(MAX_VALUE, m_locale);
    if (res != -1) {
        t_error("Test case %s failed, get result is -1 but want get value %d\n", __FUNCTION__, res);
    } 
    freelocale(m_locale);
 }

 /**
 * @tc.name      : towlower_l_0600
 * @tc.desc      : Verify towlower_l process success when using the en_US.UTF-8 character set.
 *                 Input parameter is the maximum value of int, please verify that the return is -1
 * @tc.level     : Level 1
 */
 void towlower_l_0600(void) 
 {
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    wint_t res = towlower_l(MAX_VALUE, m_locale);
    if (res != -1) {
        t_error("Test case %s failed, get result is -1 but want get value %d\n", __FUNCTION__, res);
    } 
    freelocale(m_locale);
 }


int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    towlower_0100();
    towlower_0200();
    towlower_l_0100();
    towlower_l_0200();
    towlower_l_0300();
    towlower_l_0400();
    towlower_l_0500();
    towlower_l_0600();
    return t_status;
}