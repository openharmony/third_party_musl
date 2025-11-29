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
#include "functionalext.h"
#include <errno.h>

#define TEST_BASE_MIN 1
#define TEST_BASE_MAX 37
#define LENGTH(x) (sizeof(x) / sizeof *(x))

/* r = place to store result
 * f = function call to test (or any expression)
 * x = expected result
 * m = message to print on failure (with formats for r & x)
**/

#define TEST(r, f, x, m) ( \
	errno = 0, msg = #f, ((r) = (f)) == (x) || \
	(t_error("%s failed (" m ")\n", #f, r, x), 0) )

#define TEST2(r, f, x, m) ( \
	((r) = (f)) == (x) || \
	(t_error("%s failed (" m ")\n", msg, r, x), 0) )

struct testNum{
    char *s;
    long long f;
};

static struct testNum g_t[] = {
    {"1000", 1000LL},
    {"9223372036854775807", 9223372036854775807ULL},
};

static struct testNum g_h[] = {
    {"0x1A", 0x1AULL},
};

static struct testNum g_o[] = {
    {"0123", 0123ULL},
};

static struct testNum g_b[] = {
    {"01010101", 01010101ULL},
};

/**
 * @tc.name      : strtoull_l_0100
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoull_l, decimal number
 * @tc.level     : Level 0
 */
void strtoull_l_0100(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ull = strtoull_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ull, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0200
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoull_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoull_l_0200(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ull = strtoull_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ull, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0300
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoull_l, octal number
 * @tc.level     : Level 0
 */
void strtoull_l_0300(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ull = strtoull_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ull, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0400
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoull_l, binary number
 * @tc.level     : Level 0
 */
void strtoull_l_0400(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ull = strtoull_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ull, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0500
 * @tc.desc      : set locale as zh_CN.UTF-8, do basic tests for strtoull_l, decimal number, overflow
 * @tc.level     : Level 0
 */
void strtoull_l_0500(void)
{
    int i;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL, "uncaught overflow %llu != %llu");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0600
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, decimal number
 * @tc.level     : Level 0
 */
void strtoull_l_0600(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ull = strtoull_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ull, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0700
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoull_l_0700(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ull = strtoull_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ull, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0800
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, octal number
 * @tc.level     : Level 0
 */
void strtoull_l_0800(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ull = strtoull_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ull, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_0900
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, binary number
 * @tc.level     : Level 0
 */
void strtoull_l_0900(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ull = strtoull_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ull, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01000
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, decimal number, ERANGE
 * @tc.level     : Level 0
 */
void strtoull_l_01000(void)
{
    int i;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL, "uncaught overflow %llu != %llu");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
	TEST2(i, errno, 0, "errno %d != %d");

    TEST(ull, strtoull_l(s="-9223372036854775807", &p, 0, loc), -9223372036854775807ULL, "fail %llu != %llu");
	TEST2(i, errno, 0, "errno %d != %d");

    TEST(ull, strtoull_l(s="-123", &p, 0, loc), -123ULL, "fail %llu != %llu");
	TEST2(i, errno, 0, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01100
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l, decimal number, base error
 * @tc.level     : Level 0
 */
void strtoull_l_01100(void)
{
    int i;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="1000", &p, TEST_BASE_MIN, loc), 0ULL, "base error %llu != %llu");
	TEST2(i, errno, EINVAL, "errno %d != %d");
    TEST(ull, strtoull_l(s="1000", &p, TEST_BASE_MAX, loc), 0ULL, "base error %llu != %llu");
	TEST2(i, errno, EINVAL, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01200
 * @tc.desc      : set locale as en_US.UTF-8, do basic tests for strtoull_l
 * @tc.level     : Level 0
 */
void strtoull_l_01200(void)
{
    int i;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="1A", &p, 0, loc), 1ULL, "uncaught overflow %llu != %llu");
    TEST2(i, p-s, 1, "wrong final position %d != %d");
	TEST2(i, errno, 0, "errno %d != %d");
    TEST(ull, strtoull_l(s="1,2", &p, 0, loc), 1ULL, "uncaught overflow %llu != %llu");
    TEST2(i, p-s, 1, "wrong final position %d != %d");
	TEST2(i, errno, 0, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01300
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoull_l, decimal number
 * @tc.level     : Level 0
 */
void strtoull_l_01300(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_t); i++) {
        ull = strtoull_l(g_t[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("decimal number", ull, g_t[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01400
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoull_l, hexadecimal number
 * @tc.level     : Level 0
 */
void strtoull_l_01400(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_h); i++) {
        ull = strtoull_l(g_h[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("hexadecimal number", ull, g_h[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01500
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoull_l, octal number
 * @tc.level     : Level 0
 */
void strtoull_l_01500(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_o); i++) {
        ull = strtoull_l(g_o[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("octal number", ull, g_o[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01600
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoull_l, binary number
 * @tc.level     : Level 0
 */
void strtoull_l_01600(void)
{
    unsigned long long ull;
    char *p;
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ull = strtoull_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("binary number", ull, g_b[i].f);
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01700
 * @tc.desc      : set locale as zh_CN, do basic tests for strtoull_l, decimal number, overflow
 * @tc.level     : Level 0
 */
void strtoull_l_01700(void)
{
    int i;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL, "uncaught overflow %llu != %llu");
    TEST(ull, strtoull_l(s="18446744073709551615", &p, 0, loc), 18446744073709551615ULL, "uncaught overflow %llu != %llu");
    TEST2(i, errno, 0, "errno %d != %d");
    TEST(ull, strtoull_l(s="18446744073709551616", &p, 0, loc), 18446744073709551615ULL, "uncaught overflow %llu != %llu");
    TEST2(i, errno, ERANGE, "errno %d != %d");
    TEST(ull, strtoull_l(s="-18446744073709551616", &p, 0, loc), 18446744073709551615ULL, "uncaught overflow %llu != %llu");
    TEST2(i, errno, ERANGE, "errno %d != %d");
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_01800
 * @tc.desc      : Thousands grouping character
 * @tc.level     : Level 0
 */
void strtoull_l_01800(void)
{
    int a;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    errno = 0;
    ull = strtoull_l(s="123,809", &p, 0, loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ull, 123ULL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");
    freelocale(loc);
}

struct ltest {
    const char *str;		        /* Convert this.  */
    unsigned long long int expect;	/* To get this.  */
    int base;			            /* Use this base.  */
    int left;			            /* With this left over.  */
    const unsigned int err;			/* And this in errno.  */
};

static const struct ltest tests[] = {
    /* Then unsigned.  */
    {"  0", 0, 0, 0, 0},
    {"0xffffffffg", 0xffffffff, 0, 'g', 0},
    {"0xffffffffffffffffg", 0xffffffffffffffffull, 0, 'g', 0},
    {"-0xfedcba987654321", 0xf0123456789abcdfull, 0, 0, 0},
    {"0xf1f2f3f4f5f6f7f8f9", 0xffffffffffffffffull, 0, 0, ERANGE},
    {"-0x123456789abcdef01", 0xffffffffffffffffull, 0, 0, ERANGE},
    {"1111111111111111111111111111111111111111111111111111111111111111", 0xffffffffffffffff, 2, 0, 0},
    {"10000000000000000000000000000000000000000000000000000000000000000", 0xffffffffffffffff, 2, 0, ERANGE},
    {"11112220022122120101211020120210210211220", 0xffffffffffffffff, 3, 0, 0},
    {"11112220022122120101211020120210210211221", 0xffffffffffffffff, 3, 0, ERANGE},
    {"33333333333333333333333333333333", 0xffffffffffffffff, 4, 0, 0},
    {"100000000000000000000000000000000", 0xffffffffffffffff, 4, 0, ERANGE},
    {"2214220303114400424121122430", 0xffffffffffffffff, 5, 0, 0},
    {"2214220303114400424121122431", 0xffffffffffffffff, 5, 0, ERANGE},
    {"3520522010102100444244423", 0xffffffffffffffff, 6, 0, 0},
    {"3520522010102100444244424", 0xffffffffffffffff, 6, 0, ERANGE},
    {"45012021522523134134601", 0xffffffffffffffff, 7, 0, 0},
    {"45012021522523134134602", 0xffffffffffffffff, 7, 0, ERANGE},
    {"1777777777777777777777", 0xffffffffffffffff, 8, 0, 0},
    {"2000000000000000000000", 0xffffffffffffffff, 8, 0, ERANGE},
    {"145808576354216723756", 0xffffffffffffffff, 9, 0, 0},
    {"145808576354216723757", 0xffffffffffffffff, 9, 0, ERANGE},
    {"18446744073709551615", 0xffffffffffffffff, 10, 0, 0},
    {"18446744073709551616", 0xffffffffffffffff, 10, 0, ERANGE},
    {"335500516a429071284", 0xffffffffffffffff, 11, 0, 0},
    {"335500516a429071285", 0xffffffffffffffff, 11, 0, ERANGE},
    {"839365134a2a240713", 0xffffffffffffffff, 12, 0, 0},
    {"839365134a2a240714", 0xffffffffffffffff, 12, 0, ERANGE},
    {"219505a9511a867b72", 0xffffffffffffffff, 13, 0, 0},
    {"219505a9511a867b73", 0xffffffffffffffff, 13, 0, ERANGE},
    {"8681049adb03db171", 0xffffffffffffffff, 14, 0, 0},
    {"8681049adb03db172", 0xffffffffffffffff, 14, 0, ERANGE},
    {"2c1d56b648c6cd110", 0xffffffffffffffff, 15, 0, 0},
    {"2c1d56b648c6cd111", 0xffffffffffffffff, 15, 0, ERANGE},
    {"ffffffffffffffff", 0xffffffffffffffff, 16, 0, 0},
    {"10000000000000000", 0xffffffffffffffff, 16, 0, ERANGE},
    {"67979g60f5428010", 0xffffffffffffffff, 17, 0, 0},
    {"67979g60f5428011", 0xffffffffffffffff, 17, 0, ERANGE},
    {"2d3fgb0b9cg4bd2f", 0xffffffffffffffff, 18, 0, 0},
    {"2d3fgb0b9cg4bd2g", 0xffffffffffffffff, 18, 0, ERANGE},
    {"141c8786h1ccaagg", 0xffffffffffffffff, 19, 0, 0},
    {"141c8786h1ccaagh", 0xffffffffffffffff, 19, 0, ERANGE},
    {"b53bjh07be4dj0f", 0xffffffffffffffff, 20, 0, 0},
    {"b53bjh07be4dj0g", 0xffffffffffffffff, 20, 0, ERANGE},
    {"5e8g4ggg7g56dif", 0xffffffffffffffff, 21, 0, 0},
    {"5e8g4ggg7g56dig", 0xffffffffffffffff, 21, 0, ERANGE},
    {"2l4lf104353j8kf", 0xffffffffffffffff, 22, 0, 0},
    {"2l4lf104353j8kg", 0xffffffffffffffff, 22, 0, ERANGE},
    {"1ddh88h2782i515", 0xffffffffffffffff, 23, 0, 0},
    {"1ddh88h2782i516", 0xffffffffffffffff, 23, 0, ERANGE},
    {"l12ee5fn0ji1if", 0xffffffffffffffff, 24, 0, 0},
    {"l12ee5fn0ji1ig", 0xffffffffffffffff, 24, 0, ERANGE},
    {"c9c336o0mlb7ef", 0xffffffffffffffff, 25, 0, 0},
    {"c9c336o0mlb7eg", 0xffffffffffffffff, 25, 0, ERANGE},
    {"7b7n2pcniokcgf", 0xffffffffffffffff, 26, 0, 0},
    {"7b7n2pcniokcgg", 0xffffffffffffffff, 26, 0, ERANGE},
    {"4eo8hfam6fllmo", 0xffffffffffffffff, 27, 0, 0},
    {"4eo8hfam6fllmp", 0xffffffffffffffff, 27, 0, ERANGE},
    {"2nc6j26l66rhof", 0xffffffffffffffff, 28, 0, 0},
    {"2nc6j26l66rhog", 0xffffffffffffffff, 28, 0, ERANGE},
    {"1n3rsh11f098rn", 0xffffffffffffffff, 29, 0, 0},
    {"1n3rsh11f098ro", 0xffffffffffffffff, 29, 0, ERANGE},
    {"14l9lkmo30o40f", 0xffffffffffffffff, 30, 0, 0},
    {"14l9lkmo30o40g", 0xffffffffffffffff, 30, 0, ERANGE},
    {"nd075ib45k86f", 0xffffffffffffffff, 31, 0, 0},
    {"nd075ib45k86g", 0xffffffffffffffff, 31, 0, ERANGE},
    {"fvvvvvvvvvvvv", 0xffffffffffffffff, 32, 0, 0},
    {"g000000000000", 0xffffffffffffffff, 32, 0, ERANGE},
    {"b1w8p7j5q9r6f", 0xffffffffffffffff, 33, 0, 0},
    {"b1w8p7j5q9r6g", 0xffffffffffffffff, 33, 0, ERANGE},
    {"7orp63sh4dphh", 0xffffffffffffffff, 34, 0, 0},
    {"7orp63sh4dphi", 0xffffffffffffffff, 34, 0, ERANGE},
    {"5g24a25twkwff", 0xffffffffffffffff, 35, 0, 0},
    {"5g24a25twkwfg", 0xffffffffffffffff, 35, 0, ERANGE},
    {"3w5e11264sgsf", 0xffffffffffffffff, 36, 0, 0},
    {"3w5e11264sgsg", 0xffffffffffffffff, 36, 0, ERANGE},
};

/**
 * @tc.name      : strtoull_l_01900
 * @tc.desc      : glibc case
 * @tc.level     : Level 0
 */
void strtoull_l_01900(void)
{
    int a;
    char *p;
    unsigned long long ull;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(tests); i++) {
        errno = 0;
        ull = strtoull_l(tests[i].str, &p, tests[i].base, loc);
        EXPECT_LONGLONGEQ("glibc case", ull, tests[i].expect);
        if (ull != tests[i].expect || errno != tests[i].err) {
            printf("glibc case number %s ll=%lld \n", tests[i].str, ull);
            printf("glibc case error %d \n", errno);   
        }
	    TEST2(a, errno, tests[i].err, "glibc case errno %d != %d");
        TEST2(a, *p, tests[i].left, "glibc case wrong final position %d != %d");
    }
    freelocale(loc);
}

int main(void)
{
    strtoull_l_0100();
    strtoull_l_0200();
    strtoull_l_0300();
    strtoull_l_0400();
    strtoull_l_0500();
    strtoull_l_0600();
    strtoull_l_0700();
    strtoull_l_0800();
    strtoull_l_0900();
    strtoull_l_01000();
    strtoull_l_01100();
    strtoull_l_01200();
    strtoull_l_01300();
    strtoull_l_01400();
    strtoull_l_01500();
    strtoull_l_01600();
    strtoull_l_01700();
    strtoull_l_01800();
    strtoull_l_01900();
    return t_status;
}