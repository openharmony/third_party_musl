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

#define EXPECT_FLOAT_EQ(a, b)                                              \
    do {                                                                   \
        if (fabsf((a) - (b)) > FLT_EPSILON) {                              \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
        }                                                                  \
    } while (0)

#ifndef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : wcstof_l_unsupported
 * @tc.desc      : Verify the wcstof_l stub returns ENOSYS without mutating endptr
 * @tc.level     : Level 1
 */
static void wcstof_l_unsupported(void)
{
    wchar_t stub[] = L"stub";
    wchar_t *end = stub;
    float num;

    errno = 0;
    num = wcstof_l(L"123.45", &end, (locale_t)0);
    EXPECT_FLOAT_EQ(num, 0.0f);
    EXPECT_EQ(wcstof_l_unsupported, errno, ENOSYS);
    if (end != stub) {
        t_error("%s end pointer changed unexpectedly\n", __func__);
    }
}
#else
static const wchar_t *g_wcstof_boundary_cases[] = {
    L"-.00000",
    L".7006492321624085354618647916449580656401309709382578858785341419448955413429303e-45",
    L".1175494420887210724209590083408724842314472120785184615334540294131831453944281e-37",
    L".1175494420887210724209590083408724842314472120785184615334540294131831453944282e-37",
    L"340282356779733661637539395458142568447.9999999999999999999",
};

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

static void expect_same_float(const char *tag, float got, float expected)
{
    if (isnan(expected)) {
        if (!isnan(got)) {
            t_error("%s failed: expected NaN, got %a\n", tag, got);
        }
        return;
    }
    if (got != expected || signbit(got) != signbit(expected)) {
        t_error("%s failed: expected %a, got %a\n", tag, expected, got);
    }
}

static void check_wcstof_l_matches(const char *tag, const wchar_t *text, locale_t loc)
{
    wchar_t *end = NULL;
    wchar_t *end_ref = NULL;
    float got;
    float ref;
    int got_errno;
    int ref_errno;

    errno = 0;
    got = wcstof_l(text, &end, loc);
    got_errno = errno;

    errno = 0;
    ref = wcstof(text, &end_ref);
    ref_errno = errno;

    expect_same_float(tag, got, ref);
    if (ref_errno != 0 && got_errno != ref_errno) {
        t_error("%s failed: errno mismatch (%d != %d), wcstof_l=%a wcstof=%a\n",
            tag, got_errno, ref_errno, got, ref);
    }
    if (!end || !end_ref) {
        t_error("%s failed: endptr not set (wcstof_l=%p, wcstof=%p)\n", tag, end, end_ref);
    } else if (end != end_ref) {
        t_error("%s failed: endptr mismatch (%ld != %ld)\n", tag,
            (long)(end - text), (long)(end_ref - text));
    }
}

static void check_wcstof_l_matches_noend(const char *tag, const wchar_t *text, locale_t loc)
{
    float got;
    float ref;
    int got_errno;
    int ref_errno;

    errno = 0;
    got = wcstof_l(text, NULL, loc);
    got_errno = errno;

    errno = 0;
    ref = wcstof(text, NULL);
    ref_errno = errno;

    expect_same_float(tag, got, ref);
    if (ref_errno != 0 && got_errno != ref_errno) {
        t_error("%s failed: errno mismatch (%d != %d), wcstof_l=%a wcstof=%a\n",
            tag, got_errno, ref_errno, got, ref);
    }
}

/**
 * @tc.name      : wcstof_l_0100
 * @tc.desc      : Verify wcstof_l matches wcstof for a decimal wide string
 * @tc.level     : Level 0
 */
static void wcstof_l_0100(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"123.45xyz", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0200
 * @tc.desc      : Verify wcstof_l matches wcstof for signed input with leading whitespace
 * @tc.level     : Level 0
 */
static void wcstof_l_0200(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"  -123.45rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0300
 * @tc.desc      : Verify wcstof_l matches wcstof when no conversion is possible
 * @tc.level     : Level 1
 */
static void wcstof_l_0300(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"xxx123.45", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0400
 * @tc.desc      : Verify wcstof_l matches wcstof for infinity input
 * @tc.level     : Level 1
 */
static void wcstof_l_0400(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"+inf", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0500
 * @tc.desc      : Verify wcstof_l matches wcstof when endptr is NULL
 * @tc.level     : Level 0
 */
static void wcstof_l_0500(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches_noend(__func__, L"456.75", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0600
 * @tc.desc      : Verify wcstof_l matches wcstof for hexadecimal floating input
 * @tc.level     : Level 1
 */
static void wcstof_l_0600(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"0X1.BC70A3D70A3D7P+6rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0700
 * @tc.desc      : Verify wcstof_l matches wcstof for NaN payload input
 * @tc.level     : Level 1
 */
static void wcstof_l_0700(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"nan(payload)rest", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0800
 * @tc.desc      : Verify wcstof_l matches wcstof for overflow handling
 * @tc.level     : Level 1
 */
static void wcstof_l_0800(void)
{
    locale_t loc = open_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"1.18973e+4932", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_0900
 * @tc.desc      : Verify wcstof_l preserves wcstof boundary rounding and signed zero results
 * @tc.level     : Level 1
 */
static void wcstof_l_0900(void)
{
    locale_t loc = open_c_locale();
    size_t i;

    if (!loc) return;
    for (i = 0; i < LENGTH(g_wcstof_boundary_cases); i++) {
        check_wcstof_l_matches(__func__, g_wcstof_boundary_cases[i], loc);
    }
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_1000
 * @tc.desc      : Verify wcstof_l matches wcstof when the locale argument is NULL
 * @tc.level     : Level 1
 */
static void wcstof_l_1000(void)
{
    check_wcstof_l_matches(__func__, L"0X1.8P+2tail", (locale_t)0);
}

/**
 * @tc.name      : wcstof_l_1100
 * @tc.desc      : Verify wcstof_l matches wcstof when the locale lacks LC_NUMERIC
 * @tc.level     : Level 1
 */
static void wcstof_l_1100(void)
{
    locale_t loc = open_ctype_only_c_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"42.5tail", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_1200
 * @tc.desc      : Verify wcstof_l ignores locale-specific decimal separators
 * @tc.level     : Level 1
 */
static void wcstof_l_1200(void)
{
    locale_t loc = open_french_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"12,5tail", loc);
    freelocale(loc);
}

/**
 * @tc.name      : wcstof_l_1300
 * @tc.desc      : Verify wcstof_l ignores locale-specific hexadecimal radix characters
 * @tc.level     : Level 1
 */
static void wcstof_l_1300(void)
{
    locale_t loc = open_french_locale();

    if (!loc) return;
    check_wcstof_l_matches(__func__, L"0X1,8P+2tail", loc);
    freelocale(loc);
}
#endif

int main(void)
{
#ifndef MUSL_EXTERNAL_FUNCTION
    wcstof_l_unsupported();
#else
    wcstof_l_0100();
    wcstof_l_0200();
    wcstof_l_0300();
    wcstof_l_0400();
    wcstof_l_0500();
    wcstof_l_0600();
    wcstof_l_0700();
    wcstof_l_0800();
    wcstof_l_0900();
    wcstof_l_1000();
    wcstof_l_1100();
    wcstof_l_1200();
    wcstof_l_1300();
#endif
    return t_status;
}
