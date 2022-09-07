/*
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

#include <uchar.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include "functionalext.h"

mbstate_t state;

/**
 * @tc.name      : c16rtomb_0100
 * @tc.desc      : Verify that the 16-bit wide character representation is converted to a narrow multibyte character
 *                 representation.
 * @tc.level     : Level 0
 */
void c16rtomb_0100(void)
{
    wchar_t str[] = L"test";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; ++n) {
        rc = c16rtomb(p, str[n], &state);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0100", rc, 1);
}

/**
 * @tc.name      : c16rtomb_0200
 * @tc.desc      : Verify that the 16-bit wide character representation is converted to a narrow multibyte character
 *                 representation(s parameter NULL). representation.
 * @tc.level     : Level 1
 */
void c16rtomb_0200(void)
{
    wchar_t str[] = L"";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; ++n) {
        rc = c16rtomb(p, str[n], &state);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0200", rc, 1);
}

/**
 * @tc.name      : c16rtomb_0300
 * @tc.desc      : Verify that the 16-bit wide character representation cannot be converted to a narrow multibyte
 *                 character representation (s parameter NULL).
 * @tc.level     : Level 2
 */
void c16rtomb_0300(void)
{
    wchar_t str[] = L"";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    size_t n = 0;
    rc = c16rtomb(p, 0xE000u, NULL);
    EXPECT_EQ("c16rtomb_0300", rc, -1);
}

/**
 * @tc.name      : c16rtomb_0400
 * @tc.desc      : Verify that the 16-bit wide character representation is converted to a narrow multibyte character
 *                 representation(c16 is 0xD800u,ps parameter NULL).
 * @tc.leve      : Level 1
 */
void c16rtomb_0400(void)
{
    wchar_t str[] = L"test";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; ++n) {
        rc = c16rtomb(p, 0xD800u, NULL);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0400", rc, 0);
}

/**
 * @tc.name      : c16rtomb_0500
 * @tc.desc      : Verify that the 16-bit wide character representation is converted to a narrow multibyte character
 *                 representation (c16 is 0xDC00u,s parameter NULL).
 * @tc.leve      : Level 1
 */
void c16rtomb_0500(void)
{
    wchar_t str[] = L"test";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; n++) {
        rc = c16rtomb(p, 0xdc00u, NULL);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0500", rc, -1);
}

/**
 * @tc.name      : c16rtomb_0600
 * @tc.desc      : Verify that the 16-bit wide character representation cannot be converted to a narrow multibyte
 *                 character representation (c16 is 0xE000u).
 * @tc.leve      : Level 2
 */
void c16rtomb_0600(void)
{
    wchar_t str[] = L"test";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; ++n) {
        rc = c16rtomb(p, 0xE000u, &state);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0600", rc, -1);
}

/**
 * @tc.name      : c16rtomb_0700
 * @tc.desc      : Verify that the 16-bit wide character representation is converted to a narrow multibyte character
 *                 representation (c16 is 0xDFFFu).
 * @tc.leve      : Level 1
 */
void c16rtomb_0700(void)
{
    wchar_t str[] = L"test";
    size_t in_sz = sizeof str / sizeof *str;
    int rc = 0;
    char out[MB_CUR_MAX * in_sz];
    char *p = out;
    for (size_t n = 0; n < in_sz; ++n) {
        rc = c16rtomb(p, 0xDFFFu, &state);
        p += rc;
    }
    EXPECT_EQ("c16rtomb_0700", rc, 1);
}

int main()
{
    c16rtomb_0100();
    c16rtomb_0200();
    c16rtomb_0300();
    c16rtomb_0400();
    c16rtomb_0500();
    c16rtomb_0600();
    c16rtomb_0700();
    return t_status;
}