/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <locale.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "functionalext.h"

#define TEST_BASE_MIN 1
#define TEST_BASE_MAX 37
#define LENGTH(x) (sizeof(x) / sizeof *(x))

/* r = place to store result
 * f = function call to test (or any expression)
 * x = expected result
 * m = message to print on failure (with formats for r & x)
**/

#define TEST(r, f, x, m) ( \
	errno = 0, ((r) = (f)) == (x) || \
	(t_error("%s failed (" m ")\n", #f, r, x), 0) )

#define TEST2(r, f, x, m) ( \
	((r) = (f)) == (x) || \
	(t_error("%s failed (" m ")\n", #f, r, x), 0) )

struct testNum{
    char *s;
    long long f;
};

static struct testNum g_t[] = {
    {"1000", 1000LL},
    {"-100000", -100000LL},
    {"9223372036854775807", 9223372036854775807LL},
    {"-9223372036854775807", -9223372036854775807LL},
};

static struct testNum g_h[] = {
    {"0x1A", 0x1ALL},
};

static struct testNum g_o[] = {
    {"0123", 0123LL},
};

static struct testNum g_b[] = {
    {"01010101", 01010101LL},
};

/**
 * @tc.name      : strtoll_l_0100
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoll_l, decimal number
 * @tc.level     : Level 0
 */
void strtoll_l_0100(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ll = strtoll_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ll, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0200
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoll_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoll_l_0200(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ll = strtoll_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ll, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0300
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoll_l, octal number
 * @tc.level     : Level 0
 */
void strtoll_l_0300(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ll = strtoll_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ll, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0400
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoll_l, binary number
 * @tc.level     : Level 0
 */
void strtoll_l_0400(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ll = strtoll_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ll, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0500
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoll_l, decimal number, overflow
 * @tc.level     : Level 0
 */
void strtoll_l_0500(void)
{
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ll = strtoll_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("strtoll_l", ll, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0600
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, decimal number
 * @tc.level     : Level 0
 */
void strtoll_l_0600(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ll = strtoll_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ll, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0700
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoll_l_0700(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ll = strtoll_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ll, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0800
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, octal number
 * @tc.level     : Level 0
 */
void strtoll_l_0800(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ll = strtoll_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ll, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_0900
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, binary number
 * @tc.level     : Level 0
 */
void strtoll_l_0900(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ll = strtoll_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ll, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01000
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, decimal number, ERANGE
 * @tc.level     : Level 0
 */
void strtoll_l_01000(void)
{
    int i;
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ll, strtoll_l(s="9223372036854775808", &p, 0, loc), 9223372036854775807LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
	TEST2(i, errno, ERANGE, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01100
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l, decimal number, base error
 * @tc.level     : Level 0
 */
void strtoll_l_01100(void)
{
    int i;
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ll, strtoll_l(s="1000", &p, TEST_BASE_MIN, loc), 0LL, "base error %lld != %lld");
	TEST2(i, errno, EINVAL, "errno %d != %d");
    TEST(ll, strtoll_l(s="1000", &p, TEST_BASE_MAX, loc), 0LL, "base error %lld != %lld");
	TEST2(i, errno, EINVAL, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01200
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoll_l
 * @tc.level     : Level 0
 */
void strtoll_l_01200(void)
{
    int i;
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ll, strtoll_l(s="1A", &p, 0, loc), 1LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 1, "wrong final position %d != %d");
	TEST2(i, errno, 0, "errno %d != %d");
    TEST(ll, strtoll_l(s="1,2", &p, 0, loc), 1LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 1, "wrong final position %d != %d");
	TEST2(i, errno, 0, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01300
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoll_l, decimal number
 * @tc.level     : Level 0
 */
void strtoll_l_01300(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ll = strtoll_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ll, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01400
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoll_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoll_l_01400(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ll = strtoll_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ll, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01500
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoll_l, octal number
 * @tc.level     : Level 0
 */
void strtoll_l_01500(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ll = strtoll_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ll, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01600
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoll_l, binary number
 * @tc.level     : Level 0
 */
void strtoll_l_01600(void)
{
    long long ll;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ll = strtoll_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ll, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01700
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoll_l, decimal number, overflow
 * @tc.level     : Level 0
 */
void strtoll_l_01700(void)
{
    int i;
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ll, strtoll_l(s="9223372036854775808", &p, 0, loc), 9223372036854775807LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01800
 * @tc.desc      : Thousands grouping character
 * @tc.level     : Level 0
 */
void strtoll_l_01800(void)
{
    int a;
    char *p;
    long long ll;
    char *s;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    errno = 0;
    ll = strtoll_l(s="123,809", &p, 0, loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ll, 123LL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");
    freelocale(loc);
}

int main(void)
{
    strtoll_l_0100();
    strtoll_l_0200();
    strtoll_l_0300();
    strtoll_l_0400();
    strtoll_l_0500();
    strtoll_l_0600();
    strtoll_l_0700();
    strtoll_l_0800();
    strtoll_l_0900();
    strtoll_l_01000();
    strtoll_l_01100();
    strtoll_l_01200();
    strtoll_l_01300();
    strtoll_l_01400();
    strtoll_l_01500();
    strtoll_l_01600();
    strtoll_l_01700();
    strtoll_l_01800();
    return t_status;
}