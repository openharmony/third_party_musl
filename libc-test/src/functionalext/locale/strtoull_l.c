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
#include <pthread.h>

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
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL,
        "uncaught overflow %llu != %llu");
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
    locale_t loc = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL,
        "uncaught overflow %llu != %llu");
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
    locale_t loc = newlocale(LC_ALL_MASK, "zh_CN", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    TEST(ull, strtoull_l(s="9223372036854775808", &p, 0, loc), 9223372036854775808ULL,
        "uncaught overflow %llu != %llu");
    TEST(ull, strtoull_l(s="18446744073709551615", &p, 0, loc), 18446744073709551615ULL,
        "uncaught overflow %llu != %llu");
    TEST2(i, errno, 0, "errno %d != %d");
    TEST(ull, strtoull_l(s="18446744073709551616", &p, 0, loc), 18446744073709551615ULL,
        "uncaught overflow %llu != %llu");
    TEST2(i, errno, ERANGE, "errno %d != %d");
    TEST(ull, strtoull_l(s="-18446744073709551616", &p, 0, loc), 18446744073709551615ULL,
        "uncaught overflow %llu != %llu");
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

/**
 * @tc.name      : strtoull_l_01900
 * @tc.desc      : basic
 * @tc.level     : Level 0
 */
void strtoull_l_01900(void)
{
    int a;
    locale_t c_locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    if (c_locale == NULL) {
        perror("newlocale failed");
        return;
    }
    struct {
        const char *str;
        int base;
        const char *desc;
        unsigned long long expect;
    } test_cases[] = {
        {"1234567890123456789", 10, "decimal", 1234567890123456789ULL},
        {"0xFFFFFFFFFFFFFFFF", 16, "hexadecimal", 18446744073709551615ULL},
        {"0777777777777777777777", 8, "octal", 9223372036854775807ULL},
        {"1111111111111111111111111111111111111111111111111111111111111111",
            2, "binary", 18446744073709551615ULL},
        {"ABCDEF1234567890", 16, "hexadecimal capital", 12379813812177893520ULL},
        {"abcdef0123456789", 16, "hexadecimal lowercase", 12379813738877118345ULL},
        {"ZYXWVU", 36, "hexatrigesimal capital", 2175005370ULL},
        {"zyxwvu", 36, "hexatrigesimal lowercase", 2175005370ULL},
    };

    for (unsigned int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        char *endptr = NULL;
        errno = 0;
        unsigned long long val = strtoull_l(test_cases[i].str, &endptr, test_cases[i].base, c_locale);

        EXPECT_LONGLONGEQ(test_cases[i].desc, val, test_cases[i].expect);
	    TEST2(a, errno, 0, "errno %d != %d");
    }
    freelocale(c_locale);
}

/**
 * @tc.name      : strtoull_l_02000
 * @tc.desc      : Thousands grouping character
 * @tc.level     : Level 0
 */
void strtoull_l_02000(void)
{
    int a;
    char *p;
    unsigned long long ull;
    char *s;
    locale_t c_loc = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    if (c_loc == NULL) {
        perror("newlocale failed");
        return;
    }
    locale_t en_loc = newlocale(LC_NUMERIC_MASK, "en_US.UTF-8", (locale_t)0);
    if (en_loc == NULL) {
        perror("newlocale failed");
        freelocale(c_loc);
        return;
    }
    errno = 0;
    ull = strtoull_l(s="9,876,543,210,987,654", &p, 0, c_loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ull, 9ULL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");

    ull = strtoull_l(s="9,876,543,210,987,654", &p, 0, en_loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ull, 9ULL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");
    freelocale(c_loc);
    freelocale(en_loc);
}

struct test_edge{
    char *s;
    int base;
    unsigned long long f;
    int err;
};

static struct test_edge g_edge[] = {
    {"18446744073709551616", 10, 18446744073709551615ULL, ERANGE},
    {"-123456789", 10, 18446744073586094827ULL, ERANGE},
    {"xyz987654", 10, 0ULL, EINVAL},
    {"98765abc12345", 10, 98765ULL, EINVAL},
    {"0x1a2b3c", 0, 1715004ULL, EINVAL},
    {"", 10, 0ULL, EINVAL},
};

/**
 * @tc.name      : strtoull_l_02100
 * @tc.desc      : set locale as C, edge
 * @tc.level     : Level 0
 */
void strtoull_l_02100(void)
{
    unsigned long long ull;
    char *p;
    int a;
    locale_t loc = newlocale(LC_ALL_MASK, "C", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_edge); i++) {
        ull = strtoull_l(g_edge[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("number", ull, g_edge[i].f);
        TEST2(a, errno, g_edge[i].err, "errno %d != %d");
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoull_l_02200
 * @tc.desc      : set locale as C and en_US.UTF-8, reentrant
 * @tc.level     : Level 0
 */
void strtoull_l_02200(void)
{
    locale_t c_loc = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    if (c_loc == NULL) {
        perror("newlocale failed");
        return;
    }
    locale_t en_loc = newlocale(LC_NUMERIC_MASK, "en_US.UTF-8", (locale_t)0);
    if (en_loc == NULL) {
        perror("newlocale failed");
        freelocale(c_loc);
        return;
    }

    const char *str1 = "4294967296123456";
    char *endptr1 = NULL;
    const char *str2 = "1,234,567,890,123,456";
    char *endptr2 = NULL;

    int round = 1;
    long long val1 = 0, val2 = 0;
    while (round <= 3) {
        errno = 0;
        val1 = strtoull_l(str1, &endptr1, 10, c_loc);
        EXPECT_LONGLONGEQ("c_loc reentrant", val1, 4294967296123456ULL);
        errno = 0;
        val2 = strtoull_l(str2, &endptr2, 10, en_loc);
        EXPECT_LONGLONGEQ("en_loc reentrant", val2, 1ULL);
        round++;
    }
    freelocale(c_loc);
    freelocale(en_loc);
}

void *thread_func(void *arg)
{
    unsigned long long ull;
    char *p;
    int a;
    locale_t loc = (locale_t)arg;
    for (unsigned int i = 0; i < LENGTH(g_edge); i++) {
        ull = strtoull_l(g_edge[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("number", ull, g_edge[i].f);
        TEST2(a, errno, g_edge[i].err, "errno %d != %d");
    }
 	return NULL;
}

#define NUM_THREADS 5
void test_work(locale_t loc)
{
 	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * NUM_THREADS);
 	if (threads == NULL) {
 	    t_error("Failed to allocate memory: %s\n", strerror(errno));
 	    return;
 	}
 	 
 	size_t last = 0;
 	while (last < NUM_THREADS) {
 	    if (pthread_create(&(threads[last]), NULL, thread_func, loc)) {
 	        t_error("Failed to create thread: %s\n", strerror(errno));
 	        break;
 	    }
 	    last++;
 	}
 	 
 	for (size_t i = 0; i < last; i++) {
 	    if (pthread_join(threads[i], NULL)) {
 	        t_error("Failed to join thread: %s\n", strerror(errno));
 	    }
 	} 	 
 	free(threads);
}

/**
 * @tc.name      : strtoull_l_02300
 * @tc.desc      : set locale as C, multithreading
 * @tc.level     : Level 0
 */
void strtoull_l_02300(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    test_work(loc);
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
    strtoull_l_02000();
    strtoull_l_02100();
    strtoull_l_02200();
    strtoull_l_02300();
    return t_status;
}