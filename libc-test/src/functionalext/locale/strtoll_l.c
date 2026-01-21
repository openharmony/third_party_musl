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

/**
 * @tc.name      : strtoll_l_01900
 * @tc.desc      : basic
 * @tc.level     : Level 0
 */
void strtoll_l_01900(void)
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
        long long expect;
    } test_cases[] = {
        {"123456789012345", 10, "decimal", 123456789012345LL},
        {"-987654321098765", 10, "decimal", -987654321098765LL},
        {"0x1A2B3C4D5E6F", 16, "hexadecimal", 28772997619311LL},
        {"0755", 8, "octal", 493LL},
        {"111001101011001", 2, "binary", 29529LL},
        {"9ABCDEF", 16, "hexadecimal capital", 162254319LL},
        {"abcdef9", 16, "hexadecimal lowercase", 180150009LL}
    };

    for (unsigned int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        char *endptr = NULL;
        errno = 0;
        long long val = strtoll_l(test_cases[i].str, &endptr, test_cases[i].base, c_locale);

        EXPECT_LONGLONGEQ(test_cases[i].desc, val, test_cases[i].expect);
	    TEST2(a, errno, 0, "errno %d != %d");
    }
    freelocale(c_locale);
}

/**
 * @tc.name      : strtoll_l_02000
 * @tc.desc      : Thousands grouping character
 * @tc.level     : Level 0
 */
void strtoll_l_02000(void)
{
    int a;
    char *p;
    long long ll;
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
    ll = strtoll_l(s="1,234,567,890", &p, 0, c_loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ll, 1LL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");

    ll = strtoll_l(s="1,234,567,890", &p, 0, en_loc);
    EXPECT_LONGLONGEQ("thousands grouping character number", ll, 1LL);
	TEST2(a, errno, 0, "errno %d != %d");
    TEST2(a, *p, ',', "wrong final position %d != %d");
    freelocale(c_loc);
    freelocale(en_loc);
}

struct test_edge{
    char *s;
    int base;
    long long f;
    int err;
};

static struct test_edge g_edge[] = {
    {"9223372036854775808", 10, 9223372036854775807LL, ERANGE},
    {"-9223372036854775809", 10, -9223372036854775807LL - 1LL, ERANGE},
    {"abc12345", 10, 0LL, EINVAL},
    {"12345abc67890", 10, 12345LL, EINVAL},
    {"zYXWVU", 36, 0LL, EINVAL},
    {"", 10, 0LL, EINVAL},
};

/**
 * @tc.name      : strtoll_l_02100
 * @tc.desc      : set locale as C, edge
 * @tc.level     : Level 0
 */
void strtoll_l_02100(void)
{
    long long ll;
    char *p;
    int a;
    locale_t loc = newlocale(LC_ALL_MASK, "C", NULL);
    if (loc == NULL) {
        perror("newlocale failed");
        return;
    }
    for (unsigned int i = 0; i < LENGTH(g_edge); i++) {
        ll = strtoll_l(g_edge[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("number", ll, g_edge[i].f);
        TEST2(a, errno, g_edge[i].err, "errno %d != %d");
    }
    freelocale(loc);
}

/**
 * @tc.name      : strtoll_l_02200
 * @tc.desc      : set locale as C and en_US.UTF-8, reentrant
 * @tc.level     : Level 0
 */
void strtoll_l_02200(void)
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

    const char *str1 = "102420484096";
    char *endptr1 = NULL;
    const char *str2 = "1,000,000,000";
    char *endptr2 = NULL;

    int round = 1;
    long long val1 = 0, val2 = 0;
    while (round <= 3) {
        errno = 0;
        val1 = strtoll_l(str1, &endptr1, 10, c_loc);
        EXPECT_LONGLONGEQ("c_loc reentrant", val1, 102420484096LL);
        errno = 0;
        val2 = strtoll_l(str2, &endptr2, 10, en_loc);
        EXPECT_LONGLONGEQ("en_loc reentrant", val2, 1LL);
        round++;
    }
    freelocale(c_loc);
    freelocale(en_loc);
}

void *thread_func(void *arg)
{
    long long ll;
    char *p;
    int a;
    locale_t loc = (locale_t)arg;
    for (unsigned int i = 0; i < LENGTH(g_edge); i++) {
        ll = strtoll_l(g_edge[i].s, &p, 0, loc);
        EXPECT_LONGLONGEQ("number", ll, g_edge[i].f);
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
 * @tc.name      : strtoll_l_02300
 * @tc.desc      : set locale as C, multithreading
 * @tc.level     : Level 0
 */
void strtoll_l_02300(void)
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
    strtoll_l_02000();
    strtoll_l_02100();
    strtoll_l_02200();
    strtoll_l_02300();
    return t_status;
}