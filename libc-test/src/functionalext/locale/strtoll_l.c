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
 * ll = expected result
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
    int i;
    char *p;
    long long ll;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_b); i++) {
        ll = strtoll_l(g_b[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("strtoll_l", ll, g_b[i].f);
    }
    TEST(ll, strtoll_l(s="9223372036854775808", &p, 0, loc), 9223372036854775807LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
	//TEST2(i, errno, ERANGE, "missing errno %d != %d");
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
    char *msg="";
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
    char *msg="";
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
    char *msg="";
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
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ll, strtoll_l(s="9223372036854775808", &p, 0, loc), 9223372036854775807LL, "uncaught overflow %lld != %lld");
    TEST2(i, p-s, 19, "wrong final position %d != %d");
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
    /* First, signed numbers:  */
    /* simple... */
    {"123", 123, 0, 0, 0},
    {"+123", 123, 0, 0, 0},
    {"  123", 123, 0, 0, 0},
    {" 123 ", 123, 0, ' ', 0},
    {"   -17", -17, 0, 0, 0},

    /* implicit base... */
    {"0123", 0123, 0, 0, 0},
    {"0123a", 0123, 0, 'a', 0},
    {"01239", 0123, 0, '9', 0},
    {"0x123", 0x123, 0, 0, 0},
    {"-0x123", -0x123, 0, 0, 0},
    {"0x0xc", 0, 0, 'x', 0},
    {" +0x123fg", 0x123f, 0, 'g', 0},

    /* explicit base... */
    {"123", 0x123, 16, 0, 0},
    {"0x123", 0x123, 16, 0, 0},
    {"123", 0123, 8, 0, 0},
    {"0123", 0123, 8, 0, 0},
    {"0123", 123, 10, 0, 0},
    {"0x123", 0, 10, 'x', 0},

    /* case insensitivity... */
    {"abcd", 0xabcd, 16, 0, 0},
    {"AbCd", 0xabcd, 16, 0, 0},
    {"0xABCD", 0xabcd, 16, 0, 0},
    {"0Xabcd", 0xabcd, 16, 0, 0},

    /* odd bases... */
    {"0xyz", 33 * 35 + 34, 35, 'z', 0},
    {"yz!", 34 * 36 + 35, 36, '!', 0},
    {"-yz", -(34*36 + 35), 36, 0, 0},
    {"GhI4", ((16*20 + 17)*20 + 18)*20 + 4, 20, 0, 0},

    /* special case for the 32-bit version of strtoll,
     from a ncftp configure test */
    {"99000000001", 1000000000ll * 99ll + 1ll, 0, 0},

    /* extremes... */
    {"9223372036854775807LL", 9223372036854775807ll, 0, 'L', 0},
    {"9223372036854775808", 9223372036854775807ll, 0, 0, ERANGE},
    {"922337203685477580777", 9223372036854775807ll, 0, 0, ERANGE},
    {"9223372036854775810", 9223372036854775807ll, 0, 0, ERANGE},
    {"-2147483648", -2147483648ll, 0, 0, 0},
    {"-9223372036854775808", -9223372036854775807ll - 1ll, 0, 0, 0},
    {"-9223372036854775809", -9223372036854775807ll - 1ll, 0, 0, ERANGE},
    {"0x112233445566778899z", 9223372036854775807ll, 16, 'z', ERANGE},
    {"0xFFFFFFFFFFFF00FF" , 9223372036854775807ll, 0, 0, ERANGE},
    {"111111111111111111111111111111111111111111111111111111111111111", 9223372036854775807ll, 2, 0, 0},
    {"1000000000000000000000000000000000000000000000000000000000000000", 9223372036854775807ll, 2, 0, ERANGE},
    {"2021110011022210012102010021220101220221", 9223372036854775807ll, 3, 0, 0},
    {"2021110011022210012102010021220101220222", 9223372036854775807ll, 3, 0, ERANGE},
    {"13333333333333333333333333333333", 9223372036854775807ll, 4, 0, 0},
    {"20000000000000000000000000000000", 9223372036854775807ll, 4, 0, ERANGE},
    {"1104332401304422434310311212", 9223372036854775807ll, 5, 0, 0},
    {"1104332401304422434310311213", 9223372036854775807ll, 5, 0, ERANGE},
    {"1540241003031030222122211", 9223372036854775807ll, 6, 0, 0},
    {"1540241003031030222122212", 9223372036854775807ll, 6, 0, ERANGE},
    {"22341010611245052052300", 9223372036854775807ll, 7, 0, 0},
    {"22341010611245052052301", 9223372036854775807ll, 7, 0, ERANGE},
    {"777777777777777777777", 9223372036854775807ll, 8, 0, 0},
    {"1000000000000000000000", 9223372036854775807ll, 8, 0, ERANGE},
    {"67404283172107811827", 9223372036854775807ll, 9, 0, 0},
    {"67404283172107811828", 9223372036854775807ll, 9, 0, ERANGE},
    {"9223372036854775807LL", 9223372036854775807ll, 10, 'L', 0},
    {"9223372036854775808", 9223372036854775807ll, 10, 0, ERANGE},
    {"1728002635214590697", 9223372036854775807ll, 11, 0, 0},
    {"1728002635214590698", 9223372036854775807ll, 11, 0, ERANGE},
    {"41a792678515120367", 9223372036854775807ll, 12, 0, 0},
    {"41a792678515120368", 9223372036854775807ll, 12, 0, ERANGE},
    {"10b269549075433c37", 9223372036854775807ll, 13, 0, 0},
    {"10b269549075433c38", 9223372036854775807ll, 13, 0, ERANGE},
    {"4340724c6c71dc7a7", 9223372036854775807ll, 14, 0, 0},
    {"4340724c6c71dc7a8", 9223372036854775807ll, 14, 0, ERANGE},
    {"160e2ad3246366807", 9223372036854775807ll, 15, 0, 0},
    {"160e2ad3246366808", 9223372036854775807ll, 15, 0, ERANGE},
    {"7fffffffffffffff", 9223372036854775807ll, 16, 0, 0},
    {"8000000000000000", 9223372036854775807ll, 16, 0, ERANGE},
    {"33d3d8307b214008", 9223372036854775807ll, 17, 0, 0},
    {"33d3d8307b214009", 9223372036854775807ll, 17, 0, ERANGE},
    {"16agh595df825fa7", 9223372036854775807ll, 18, 0, 0},
    {"16agh595df825fa8", 9223372036854775807ll, 18, 0, ERANGE},
    {"ba643dci0ffeehh", 9223372036854775807ll, 19, 0, 0},
    {"ba643dci0ffeehi", 9223372036854775807ll, 19, 0, ERANGE},
    {"5cbfjia3fh26ja7", 9223372036854775807ll, 20, 0, 0},
    {"5cbfjia3fh26ja8", 9223372036854775807ll, 20, 0, ERANGE},
    {"2heiciiie82dh97", 9223372036854775807ll, 21, 0, 0},
    {"2heiciiie82dh98", 9223372036854775807ll, 21, 0, ERANGE},
    {"1adaibb21dckfa7", 9223372036854775807ll, 22, 0, 0},
    {"1adaibb21dckfa8", 9223372036854775807ll, 22, 0, ERANGE},
    {"i6k448cf4192c2", 9223372036854775807ll, 23, 0, 0},
    {"i6k448cf4192c3", 9223372036854775807ll, 23, 0, ERANGE},
    {"acd772jnc9l0l7", 9223372036854775807ll, 24, 0, 0},
    {"acd772jnc9l0l8", 9223372036854775807ll, 24, 0, ERANGE},
    {"64ie1focnn5g77", 9223372036854775807ll, 25, 0, 0},
    {"64ie1focnn5g78", 9223372036854775807ll, 25, 0, ERANGE},
    {"3igoecjbmca687", 9223372036854775807ll, 26, 0, 0},
    {"3igoecjbmca688", 9223372036854775807ll, 26, 0, ERANGE},
    {"27c48l5b37oaop", 9223372036854775807ll, 27, 0, 0},
    {"27c48l5b37oaoq", 9223372036854775807ll, 27, 0, ERANGE},
    {"1bk39f3ah3dmq7", 9223372036854775807ll, 28, 0, 0},
    {"1bk39f3ah3dmq8", 9223372036854775807ll, 28, 0, ERANGE},
    {"q1se8f0m04isb", 9223372036854775807ll, 29, 0, 0},
    {"q1se8f0m04isc", 9223372036854775807ll, 29, 0, ERANGE},
    {"hajppbc1fc207", 9223372036854775807ll, 30, 0, 0},
    {"hajppbc1fc208", 9223372036854775807ll, 30, 0, ERANGE},
    {"bm03i95hia437", 9223372036854775807ll, 31, 0, 0},
    {"bm03i95hia438", 9223372036854775807ll, 31, 0, ERANGE},
    {"7vvvvvvvvvvvv", 9223372036854775807ll, 32, 0, 0},
    {"8000000000000", 9223372036854775807ll, 32, 0, ERANGE},
    {"5hg4ck9jd4u37", 9223372036854775807ll, 33, 0, 0},
    {"5hg4ck9jd4u38", 9223372036854775807ll, 33, 0, ERANGE},
    {"3tdtk1v8j6tpp", 9223372036854775807ll, 34, 0, 0},
    {"3tdtk1v8j6tpq", 9223372036854775807ll, 34, 0, ERANGE},
    {"2pijmikexrxp7", 9223372036854775807ll, 35, 0, 0},
    {"2pijmikexrxp8", 9223372036854775807ll, 35, 0, ERANGE},
    {"1y2p0ij32e8e7", 9223372036854775807ll, 36, 0, 0},
    {"1y2p0ij32e8e8", 9223372036854775807ll, 36, 0, ERANGE},
    {"-1000000000000000000000000000000000000000000000000000000000000000", -9223372036854775808ull, 2, 0, 0},
    {"-1000000000000000000000000000000000000000000000000000000000000001", -9223372036854775808ull, 2, 0, ERANGE},
    {"-2021110011022210012102010021220101220222", -9223372036854775808ull, 3, 0, 0},
    {"-2021110011022210012102010021220101221000", -9223372036854775808ull, 3, 0, ERANGE},
    {"-20000000000000000000000000000000", -9223372036854775808ull, 4, 0, 0},
    {"-20000000000000000000000000000001", -9223372036854775808ull, 4, 0, ERANGE},
    {"-1104332401304422434310311213", -9223372036854775808ull, 5, 0, 0},
    {"-1104332401304422434310311214", -9223372036854775808ull, 5, 0, ERANGE},
    {"-1540241003031030222122212", -9223372036854775808ull, 6, 0, 0},
    {"-1540241003031030222122213", -9223372036854775808ull, 6, 0, ERANGE},
    {"-22341010611245052052301", -9223372036854775808ull, 7, 0, 0},
    {"-22341010611245052052302", -9223372036854775808ull, 7, 0, ERANGE},
    {"-1000000000000000000000", -9223372036854775808ull, 8, 0, 0},
    {"-1000000000000000000001", -9223372036854775808ull, 8, 0, ERANGE},
    {"-67404283172107811828", -9223372036854775808ull, 9, 0, 0},
    {"-67404283172107811830", -9223372036854775808ull, 9, 0, ERANGE},
    {"-9223372036854775808", -9223372036854775807ll - 1ll, 10, 0, 0},
    {"-9223372036854775809", -9223372036854775807ll - 1ll, 10, 0, ERANGE},
    {"-1728002635214590698", -9223372036854775808ull, 11, 0, 0},
    {"-1728002635214590699", -9223372036854775808ull, 11, 0, ERANGE},
    {"-41a792678515120368", -9223372036854775808ull, 12, 0, 0},
    {"-41a792678515120369", -9223372036854775808ull, 12, 0, ERANGE},
    {"-10b269549075433c38", -9223372036854775808ull, 13, 0, 0},
    {"-10b269549075433c39", -9223372036854775808ull, 13, 0, ERANGE},
    {"-4340724c6c71dc7a8", -9223372036854775808ull, 14, 0, 0},
    {"-4340724c6c71dc7a9", -9223372036854775808ull, 14, 0, ERANGE},
    {"-160e2ad3246366808", -9223372036854775808ull, 15, 0, 0},
    {"-160e2ad3246366809", -9223372036854775808ull, 15, 0, ERANGE},
    {"-8000000000000000", -9223372036854775808ull, 16, 0, 0},
    {"-8000000000000001", -9223372036854775808ull, 16, 0, ERANGE},
    {"-33d3d8307b214009", -9223372036854775808ull, 17, 0, 0},
    {"-33d3d8307b21400a", -9223372036854775808ull, 17, 0, ERANGE},
    {"-16agh595df825fa8", -9223372036854775808ull, 18, 0, 0},
    {"-16agh595df825fa9", -9223372036854775808ull, 18, 0, ERANGE},
    {"-ba643dci0ffeehi", -9223372036854775808ull, 19, 0, 0},
    {"-ba643dci0ffeei0", -9223372036854775808ull, 19, 0, ERANGE},
    {"-5cbfjia3fh26ja8", -9223372036854775808ull, 20, 0, 0},
    {"-5cbfjia3fh26ja9", -9223372036854775808ull, 20, 0, ERANGE},
    {"-2heiciiie82dh98", -9223372036854775808ull, 21, 0, 0},
    {"-2heiciiie82dh99", -9223372036854775808ull, 21, 0, ERANGE},
    {"-1adaibb21dckfa8", -9223372036854775808ull, 22, 0, 0},
    {"-1adaibb21dckfa9", -9223372036854775808ull, 22, 0, ERANGE},
    {"-i6k448cf4192c3", -9223372036854775808ull, 23, 0, 0},
    {"-i6k448cf4192c4", -9223372036854775808ull, 23, 0, ERANGE},
    {"-acd772jnc9l0l8", -9223372036854775808ull, 24, 0, 0},
    {"-acd772jnc9l0l9", -9223372036854775808ull, 24, 0, ERANGE},
    {"-64ie1focnn5g78", -9223372036854775808ull, 25, 0, 0},
    {"-64ie1focnn5g79", -9223372036854775808ull, 25, 0, ERANGE},
    {"-3igoecjbmca688", -9223372036854775808ull, 26, 0, 0},
    {"-3igoecjbmca689", -9223372036854775808ull, 26, 0, ERANGE},
    {"-27c48l5b37oaoq", -9223372036854775808ull, 27, 0, 0},
    {"-27c48l5b37oap0", -9223372036854775808ull, 27, 0, ERANGE},
    {"-1bk39f3ah3dmq8", -9223372036854775808ull, 28, 0, 0},
    {"-1bk39f3ah3dmq9", -9223372036854775808ull, 28, 0, ERANGE},
    {"-q1se8f0m04isc", -9223372036854775808ull, 29, 0, 0},
    {"-q1se8f0m04isd", -9223372036854775808ull, 29, 0, ERANGE},
    {"-hajppbc1fc208", -9223372036854775808ull, 30, 0, 0},
    {"-hajppbc1fc209", -9223372036854775808ull, 30, 0, ERANGE},
    {"-bm03i95hia438", -9223372036854775808ull, 31, 0, 0},
    {"-bm03i95hia439", -9223372036854775808ull, 31, 0, ERANGE},
    {"-8000000000000", -9223372036854775808ull, 32, 0, 0},
    {"-8000000000001", -9223372036854775808ull, 32, 0, ERANGE},
    {"-5hg4ck9jd4u38", -9223372036854775808ull, 33, 0, 0},
    {"-5hg4ck9jd4u39", -9223372036854775808ull, 33, 0, ERANGE},
    {"-3tdtk1v8j6tpq", -9223372036854775808ull, 34, 0, 0},
    {"-3tdtk1v8j6tpr", -9223372036854775808ull, 34, 0, ERANGE},
    {"-2pijmikexrxp8", -9223372036854775808ull, 35, 0, 0},
    {"-2pijmikexrxp9", -9223372036854775808ull, 35, 0, ERANGE},
    {"-1y2p0ij32e8e8", -9223372036854775808ull, 36, 0, 0},
    {"-1y2p0ij32e8e9", -9223372036854775808ull, 36, 0, ERANGE},
};

/**
 * @tc.name      : strtoll_l_01800
 * @tc.desc      : glibc case
 * @tc.level     : Level 0
 */
void strtoll_l_01800(void)
{
    int a;
    char *p;
    long long ll;
    char *s;
    char *msg="";
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(tests); i++) {
        errno = 0;
        ll = strtoll_l(tests[i].str, &p, tests[i].base, loc);
        EXPECT_LONGLONGEQ("glibc case", ll, tests[i].expect);
        if (ll != tests[i].expect || errno != tests[i].err) {
            printf("glibc case number %s ll=%lld \n", tests[i].str, ll);
            printf("glibc case error %d \n", errno);   
        }
	    TEST2(a, errno, tests[i].err, "glibc case errno %d != %d");
        TEST2(a, *p, tests[i].left, "glibc case wrong final position %d != %d");
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_01900
 * @tc.desc      : Thousands grouping character
 * @tc.level     : Level 0
 */
void strtoll_l_01900(void)
{
    int a;
    char *p;
    long long ll;
    char *s;
    char *msg="";
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
    strtoll_l_01900();
    return t_status;
}