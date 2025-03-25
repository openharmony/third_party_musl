/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>
#include <ctype.h>
#include <wctype.h>
#include <locale.h>
#include "test.h"

/**
 * @tc.name      : towupper_l_0100
 * @tc.desc      : Verify towupper_l process success when using the en_US.UTF-8 character set.
 *                 Input parameters are Uppercase letters, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towupper_l_0100(void) 
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
        res = towupper_l(*lp, m_locale);
        if (res != *up) {
            t_error("Test case %s failed, get result is %c but want get value %c\n", __FUNCTION__, (char)res, *lp);
        }
        up++;
        lp++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towupper_l_0200
 * @tc.desc      : Verify towupper_l process fail when using the en_US.UTF-8 character set.
 *                 Input parameters as numbers, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towupper_l_0200(void)
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
        res = towupper_l(*p, m_locale);
        if (res != *p) {
            t_error("Test case %s failed, result is %c but want get value %c \n", __FUNCTION__, (char)res, *p);
        }
        p++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towupper_l_0300
 * @tc.desc      : Verify towupper_l process fail when using the zh_CN character set.
 *                 Input parameters as numbers, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towupper_l_0300(void)
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
        res = towupper_l(*p, m_locale);
        if (res != *p) {
            t_error("Test case %s failed, result is %c but want get value %c \n", __FUNCTION__, (char)res, *p);
        }
        p++;
    }
    freelocale(m_locale);
}

/**
 * @tc.name      : towupper_l_0400
 * @tc.desc      : Verify towlower_l process success when using the zh_CN character set.
 *                 Input parameters are Uppercase letters, verify conversion to Lowercase letters.
 * @tc.level     : Level 1
 */
void towupper_l_0400(void) 
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
        res = towupper_l(*lp, m_locale);
        if (res != *up) {
            t_error("Test case %s failed, get result is %c but want get value %c\n", __FUNCTION__, (char)res, *lp);
        }
        up++;
        lp++;
    }
    freelocale(m_locale);
}

int main(int argc, char *argv[])
{
    set_wctype_icu_enable();
    towupper_l_0100();
    towupper_l_0200();
    towupper_l_0300();
    towupper_l_0400();
    return t_status;
}