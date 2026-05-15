/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <errno.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <wchar.h>
#include "functionalext.h"

#define LENGTH(x) (sizeof(x) / sizeof *(x))

#define EXPECT_LDOUBLE_EQ(a, b)                                              \
    do {                                                                     \
        if (fabsl((a) - (b)) > LDBL_EPSILON) {                               \
            t_error("%s failed: %Lf is not equal to %Lf\n", __func__, a, b); \
        }                                                                    \
    } while (0)

#ifndef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : wcstold_l_unsupported
 * @tc.desc      : Verify the wcstold_l stub returns ENOSYS without mutating endptr
 * @tc.level     : Level 1
 */
static void wcstold_l_unsupported(void)
{
    wchar_t stub[] = L"stub";
    wchar_t *end = stub;
    long double num;

    errno = 0;
    num = wcstold_l(L"123.45", &end, (locale_t)0);
    EXPECT_LDOUBLE_EQ(num, 0.0L);
    EXPECT_EQ(wcstold_l_unsupported, errno, ENOSYS);
    if (end != stub) {
        t_error("%s end pointer changed unexpectedly\n", __func__);
    }
}
#else
static locale_t open_locale_aliases(const char *const *names, size_t count)
{
    locale_t loc = NULL;
    size_t i;

    for (i = 0; i < count && !loc; i++) {
        loc = newlocale(LC_ALL_MASK, names[i], NULL);
    }
    return loc;
}

static locale_t open_c_locale(void)
{
    return newlocale(LC_ALL_MASK, "C", NULL);
}

static locale_t open_ctype_only_c_locale(void)
{
    return newlocale(LC_CTYPE_MASK, "C", NULL);
}

static locale_t open_french_locale(void)
{
    static const char *const names[] = {"fr_FR.utf8", "fr_FR.UTF-8", "fr_FR"};

    return open_locale_aliases(names, LENGTH(names));
}

static void expect_same_ldouble(const char *tag, long double got, long double expected)
{
    if (isnan(expected)) {
        if (!isnan(got)) {
            t_error("%s failed: expected NaN, got %La\n", tag, got);
        }
        return;
    }
    if (got != expected || signbit(got) != signbit(expected)) {
        t_error("%s failed: expected %La, got %La\n", tag, expected, got);
    }
}

static void check_wcstold_l_matches(const char *tag, const wchar_t *text, locale_t loc)
{
    wchar_t *end = NULL;
    wchar_t *end_ref = NULL;
    long double got;
    long double ref;
    int got_errno;
    int ref_errno;

    errno = 0;
    got = wcstold_l(text, &end, loc);
    got_errno = errno;

    errno = 0;
    ref = wcstold(text, &end_ref);
    ref_errno = errno;

    expect_same_ldouble(tag, got, ref);
    if (ref_errno != 0 && got_errno != ref_errno) {
        t_error("%s failed: errno mismatch (%d != %d), wcstold_l=%La wcstold=%La\n",
            tag, got_errno, ref_errno, got, ref);
    }
    if (!end || !end_ref) {
        t_error("%s failed: endptr not set (wcstold_l=%p, wcstold=%p)\n", tag, end, end_ref);
    } else if (end != end_ref) {
        t_error("%s failed: endptr mismatch (%ld != %ld)\n", tag,
            (long)(end - text), (long)(end_ref - text));
    }
}

static void check_wcstold_l_matches_noend(const char *tag, const wchar_t *text, locale_t loc)
{
    long double got;
    long double ref;
    int got_errno;
    int ref_errno;

    errno = 0;
    got = wcstold_l(text, NULL, loc);
    got_errno = errno;

    errno = 0;
    ref = wcstold(text, NULL);
    ref_errno = errno;

    expect_same_ldouble(tag, got, ref);
    if (ref_errno != 0 && got_errno != ref_errno) {
        t_error("%s failed: errno mismatch (%d != %d), wcstold_l=%La wcstold=%La\n",
            tag, got_errno, ref_errno, got, ref);
    }
}

#if LDBL_MANT_DIG > DBL_MANT_DIG
static void check_precision_roundtrip(const char *tag, long double input, locale_t loc)
{
    wchar_t text[128];

    if (swprintf(text, sizeof(text) / sizeof(text[0]), L"%.*Lg", LDBL_DECIMAL_DIG, input) < 0) {
        t_error("%s failed: swprintf error\n", tag);
        return;
    }

    check_wcstold_l_matches(tag, text, loc);
}
#endif

/**
 * @tc.name      : wcstold_l_0100
 * @tc.desc      : Verify wcstold_l matches wcstold for a decimal wide string
 * @tc.level     : Level 0
 */
static void wcstold_l_0100(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"123.45xyz", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0200
 * @tc.desc      : Verify wcstold_l matches wcstold for signed input with leading whitespace
 * @tc.level     : Level 0
 */
static void wcstold_l_0200(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"  -123.45rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0300
 * @tc.desc      : Verify wcstold_l matches wcstold when no conversion is possible
 * @tc.level     : Level 1
 */
static void wcstold_l_0300(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"xxx123.45", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0400
 * @tc.desc      : Verify wcstold_l matches wcstold for infinity input
 * @tc.level     : Level 1
 */
static void wcstold_l_0400(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"+inf", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0500
 * @tc.desc      : Verify wcstold_l matches wcstold when endptr is NULL
 * @tc.level     : Level 0
 */
static void wcstold_l_0500(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches_noend(__func__, L"456.75", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0600
 * @tc.desc      : Verify wcstold_l matches wcstold for hexadecimal floating input
 * @tc.level     : Level 1
 */
static void wcstold_l_0600(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"0X1.BC70A3D70A3D7P+6rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0700
 * @tc.desc      : Verify wcstold_l matches wcstold for NaN payload input
 * @tc.level     : Level 1
 */
static void wcstold_l_0700(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"nan(payload)rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0800
 * @tc.desc      : Verify wcstold_l preserves wcstold long double precision
 * @tc.level     : Level 1
 */
static void wcstold_l_0800(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
#if LDBL_MANT_DIG > DBL_MANT_DIG
    check_precision_roundtrip(__func__, nextafterl(1.0L, 2.0L), loc);
    check_precision_roundtrip(__func__, nextafterl(1.0L, 0.0L), loc);
    check_precision_roundtrip(__func__, nextafterl(0.0L, 1.0L), loc);
    check_precision_roundtrip(__func__, 1.0L + ldexpl(1.0L, -(DBL_MANT_DIG + 2)), loc);
#endif
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_0900
 * @tc.desc      : Verify wcstold_l preserves wcstold range beyond the double exponent width
 * @tc.level     : Level 1
 */
static void wcstold_l_0900(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"1e400", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_1000
 * @tc.desc      : Verify wcstold_l matches wcstold for overflow handling
 * @tc.level     : Level 1
 */
static void wcstold_l_1000(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"1.18973e+49322", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_1100
 * @tc.desc      : Verify wcstold_l matches wcstold when the locale argument is NULL
 * @tc.level     : Level 1
 */
static void wcstold_l_1100(void)
{
    check_wcstold_l_matches(__func__, L"0X1.8P+2tail", (locale_t)0);
}

/**
 * @tc.name      : wcstold_l_1200
 * @tc.desc      : Verify wcstold_l matches wcstold when the locale lacks LC_NUMERIC
 * @tc.level     : Level 1
 */
static void wcstold_l_1200(void)
{
    locale_t loc = open_ctype_only_c_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"42.5tail", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_1300
 * @tc.desc      : Verify wcstold_l ignores locale-specific decimal separators
 * @tc.level     : Level 1
 */
static void wcstold_l_1300(void)
{
    locale_t loc = open_french_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"12,5tail", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstold_l_1400
 * @tc.desc      : Verify wcstold_l ignores locale-specific hexadecimal radix characters
 * @tc.level     : Level 1
 */
static void wcstold_l_1400(void)
{
    locale_t loc = open_french_locale();

    if (!loc) return;
    check_wcstold_l_matches(__func__, L"0X1,8P+2tail", loc);
    freelocale(loc);
}
#endif

int main(void)
{
#ifndef MUSL_EXTERNAL_FUNCTION
    wcstold_l_unsupported();
#else
    wcstold_l_0100();
    wcstold_l_0200();
    wcstold_l_0300();
    wcstold_l_0400();
    wcstold_l_0500();
    wcstold_l_0600();
    wcstold_l_0700();
    wcstold_l_0800();
    wcstold_l_0900();
    wcstold_l_1000();
    wcstold_l_1100();
    wcstold_l_1200();
    wcstold_l_1300();
    wcstold_l_1400();
#endif
    return t_status;
}
