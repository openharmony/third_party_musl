/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include "test.h"

#define EXPECT_DOUBLE_EQ(a, b) (fabs((a) - (b)) <= DBL_EPSILON * fmax(fabs(a), fabs(b)))
#define EXPECT_DOUBLE_CLOSE(a, b, epsilon) (fabs((a) - (b)) <= (epsilon))

/**
 * @tc.name      : strtod_0100
 * @tc.desc      : Test strtod with basic integer conversion
 * @tc.level     : Level 0
 */
void strtod_0100(void)
{
    char *endptr;
    double result;
    result = strtod("123", &endptr);
    if (result == 123.0 && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0100 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0200
 * @tc.desc      : Test strtod with conversion of negative integers
 * @tc.level     : Level 0
 */
void strtod_0200(void)
{
    char *endptr;
    double result;;
    result = strtod("-456", &endptr);
    if (result == -456.0 && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0200 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0300
 * @tc.desc      : Test strtod with floating-point number conversion
 * @tc.level     : Level 0
 */
void strtod_0300(void)
{
    char *endptr;
    double result;
    result = strtod("3.14159", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 3.14159) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0300 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0400
 * @tc.desc      : Test strtod with positive exponent in scientific notation
 * @tc.level     : Level 0
 */
void strtod_0400(void)
{
    char *endptr;
    double result;
    result = strtod("1.23e4", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 12300.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0400 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0500
 * @tc.desc      : Test strtod with negative exponent in scientific notation
 * @tc.level     : Level 0
 */
void strtod_0500(void)
{
    char *endptr;
    double result;
    result = strtod("1.23e-2", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 0.0123) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0500 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0600
 * @tc.desc      : Test strtod with positive exponent is capitalized as E
 * @tc.level     : Level 0
 */
void strtod_0600(void)
{
    char *endptr;
    double result;
    result = strtod("2.5E3", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 2500.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0600 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0700
 * @tc.desc      : Test strtod with zero-value test
 * @tc.level     : Level 0
 */
void strtod_0700(void)
{
    char *endptr;
    double result;
    result = strtod("0", &endptr);
    if (result == 0.0 && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0700 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0800
 * @tc.desc      : Test strtod with negative zero value test
 * @tc.level     : Level 0
 */
void strtod_0800(void)
{
    char *endptr;
    double result;
    result = strtod("-0", &endptr);
    if (result == -0.0 && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0800 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_0900
 * @tc.desc      : Test strtod with maximum integer value
 * @tc.level     : Level 0
 */
void strtod_0900(void)
{
    char *endptr;
    double result;
    result = strtod("2147483647", &endptr); // 2^31-1
    if (EXPECT_DOUBLE_EQ(result, 2147483647.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_0900 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1000
 * @tc.desc      : Test strtod with minimum negative integer value
 * @tc.level     : Level 0
 */
void strtod_1000(void)
{
    char *endptr;
    double result;
    result = strtod("-2147483648", &endptr);
    if (EXPECT_DOUBLE_EQ(result, -2147483648.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_1000 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1100
 * @tc.desc      : Test strtod with positive overflow test
 * @tc.level     : Level 0
 */
void strtod_1100(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("1e9999", &endptr);
    if ((result == HUGE_VAL || isinf(result)) && errno == ERANGE) {
        return;
    } else {
        t_error("strtod_1100 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1200
 * @tc.desc      : Test strtod with negative overflow test
 * @tc.level     : Level 0
 */
void strtod_1200(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("-1e9999", &endptr);
    if ((result == -HUGE_VAL || (isinf(result) && signbit(result))) && errno == ERANGE) {
        return;
    } else {
        t_error("strtod_1200 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1300
 * @tc.desc      : Test strtod with positive downflow test
 * @tc.level     : Level 0
 */
void strtod_1300(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("1e-9999", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 0.0) && errno == ERANGE) {
        return;
    } else {
        t_error("strtod_1300 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1400
 * @tc.desc      : Test strtod with negative underflow test 
 * @tc.level     : Level 0
 */
void strtod_1400(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("-1e-9999", &endptr);
    if (EXPECT_DOUBLE_EQ(result, -0.0) && errno == ERANGE) {
        return;
    } else {
        t_error("strtod_1400 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1500
 * @tc.desc      : Test strtod with infinity test
 * @tc.level     : Level 0
 */
void strtod_1500(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("inf", &endptr);
    if (isinf(result) && !signbit(result) && *endptr == '\0' && errno == 0) {
        return;
    } else {
        t_error("strtod_1500 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1600
 * @tc.desc      : Test strtod with negative infinity test
 * @tc.level     : Level 0
 */
void strtod_1600(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("-INFINITY", &endptr);
    if (isinf(result) && signbit(result) && *endptr == '\0' && errno == 0) {
        return;
    } else {
        t_error("strtod_1600 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1700
 * @tc.desc      : Test strtod with NaN test
 * @tc.level     : Level 0
 */
void strtod_1700(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("NaN", &endptr);
    if (isnan(result) && *endptr == '\0' && errno == 0) {
        return;
    } else {
        t_error("strtod_1700 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1800
 * @tc.desc      : Test strtod with NaN testing with load
 * @tc.level     : Level 0
 */
void strtod_1800(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("NaN(123)", &endptr);
    if (isnan(result) && errno == 0) {
        return;
    } else {
        t_error("strtod_1800 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_1900
 * @tc.desc      : Test strtod with signed NaN test
 * @tc.level     : Level 0
 */
void strtod_1900(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("-NAN", &endptr);
    if (isnan(result) && errno == 0) {
        return;
    } else {
        t_error("strtod_1900 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2000
 * @tc.desc      : Test strtod with hexadecimal positive floating-point number
 * @tc.level     : Level 0
 */
void strtod_2000(void)
{
    char *endptr;
    double result;
    result = strtod("0x1.2p3", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 9.0) && *endptr == '\0') { // 0x1.2 * 2^3 = 1.125 * 8 = 9.0
        return;
    } else {
        t_error("strtod_2000 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2100
 * @tc.desc      : Test strtod with hexadecimal negative exponent
 * @tc.level     : Level 0
 */
void strtod_2100(void)
{
    char *endptr;
    double result;
    result = strtod("0x1.Fp-1", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 0.96875) && *endptr == '\0') { // 0x1.F * 2^-1 = 1.9375 * 0.5 = 0.96875
        return;
    } else {
        t_error("strtod_2100 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2200
 * @tc.desc      : Test strtod with hexadecimal uppercase
 * @tc.level     : Level 0
 */
void strtod_2200(void)
{
    char *endptr;
    double result;
    result = strtod("0XABC.DEFP+10", &endptr);
    double expected = (0xABC + 0xDEF/4096.0) * pow(2, 10);
    if (EXPECT_DOUBLE_CLOSE(result, expected, 1e-6) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2200 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2300
 * @tc.desc      : Test strtod with hexadecimal integer
 * @tc.level     : Level 0
 */
void strtod_2300(void)
{
    char *endptr;
    double result;
    result = strtod("0x1A3", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 419.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2300 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2400
 * @tc.desc      : Test strtod with hexadecimal overflow
 * @tc.level     : Level 0
 */
void strtod_2400(void)
{
    char *endptr;
    double result;
    errno = 0;
    result = strtod("0x1p10000", &endptr);
    if ((result == HUGE_VAL || isinf(result)) && errno == ERANGE) {
        return;
    } else {
        t_error("strtod_2400 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2500
 * @tc.desc      : Test strtod with leading space
 * @tc.level     : Level 0
 */
void strtod_2500(void)
{
    char *endptr;
    double result;
    result = strtod("   123.45", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 123.45) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2500 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2600
 * @tc.desc      : Test strtod with leading TAB character
 * @tc.level     : Level 0
 */
void strtod_2600(void)
{
    char *endptr;
    double result;
    result = strtod("\t\n\r  -67.8", &endptr);
    if (EXPECT_DOUBLE_EQ(result, -67.8) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2600 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2700
 * @tc.desc      : Test strtod with followed by non-numeric characters
 * @tc.level     : Level 0
 */
void strtod_2700(void)
{
    char *endptr;
    double result;
    result = strtod("123.45abc", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 123.45) && strcmp(endptr, "abc") == 0) {
        return;
    } else {
        t_error("strtod_2700 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2800
 * @tc.desc      : Test strtod with decimal starting with a dot
 * @tc.level     : Level 0
 */
void strtod_2800(void)
{
    char *endptr;
    double result;
    result = strtod(".5", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 0.5) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2800 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_2900
 * @tc.desc      : Test strtod with ending with a decimal point
 * @tc.level     : Level 0
 */
void strtod_2900(void)
{
    char *endptr;
    double result;
    result = strtod("123.", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 123.0) && *endptr == '\0') {
        return;
    } else {
        t_error("strtod_2900 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3000
 * @tc.desc      : Test strtod with only decimal points
 * @tc.level     : Level 0
 */
void strtod_3000(void)
{
    char *endptr;
    double result;
    result = strtod(".", &endptr);
    if (result == 0.0) {
        return;
    } else {
        t_error("strtod_3000 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3100
 * @tc.desc      : Test strtod with index symbol but no number
 * @tc.level     : Level 0
 */
void strtod_3100(void)
{
    char *endptr;
    double result;
    result = strtod("1.23e", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 1.23) && *endptr == 'e') {
        return;
    } else {
        t_error("strtod_3100 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3200
 * @tc.desc      : Test strtod with double-symbol test
 * @tc.level     : Level 0
 */
void strtod_3200(void)
{
    char *endptr;
    double result;
    result = strtod("+-123", &endptr);
    if (result == 0.0) {
        return;
    } else {
        t_error("strtod_3200 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3300
 * @tc.desc      : Test strtod with empty string
 * @tc.level     : Level 0
 */
void strtod_3300(void)
{
    char *endptr;
    double result;
    result = strtod("", &endptr);
    if (result == 0.0) {
        return;
    } else {
        t_error("strtod_3300 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3400
 * @tc.desc      : Test strtod with long decimal part
 * @tc.level     : Level 0
 */
void strtod_3400(void)
{
    char *endptr;
    double result;
    result = strtod("0.12345678901234567890", &endptr);
    if (*endptr == '\0') {
        return;
    } else {
        t_error("strtod_3400 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3500
 * @tc.desc      : Test strtod with large integer precision
 * @tc.level     : Level 0
 */
void strtod_3500(void)
{
    char *endptr;
    double result;
    result = strtod("12345678901234567890", &endptr);
    if (*endptr == '\0') {
        return;
    } else {
        t_error("strtod_3500 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3600
 * @tc.desc      : Test strtod with endptr is NULL
 * @tc.level     : Level 0
 */
void strtod_3600(void)
{
    char *endptr;
    double result;
    result = strtod("123.45", NULL);
    if (EXPECT_DOUBLE_EQ(result, 123.45)) {
        return;
    } else {
        t_error("strtod_3600 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3700
 * @tc.desc      : Test strtod with endptr is NULL when encountering a non-numeric value
 * @tc.level     : Level 0
 */
void strtod_3700(void)
{
    char *endptr;
    double result;
    result = strtod("123.45abc", NULL);
    if (EXPECT_DOUBLE_EQ(result, 123.45)) {
        return;
    } else {
        t_error("strtod_3700 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3800
 * @tc.desc      : Test strtod with a comma as a decimal point (should fail)
 * @tc.level     : Level 0
 */
void strtod_3800(void)
{
    char *endptr;
    double result;
    result = strtod("123,45", &endptr);
    if (result == 123.0 && *endptr == ',') {
        return;
    } else {
        t_error("strtod_3800 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_3900
 * @tc.desc      : Test strtod with multiple decimal points
 * @tc.level     : Level 0
 */
void strtod_3900(void)
{
    char *endptr;
    double result;
    result = strtod("12.34.56", &endptr);
    if (EXPECT_DOUBLE_EQ(result, 12.34) && *endptr == '.') {
        return;
    } else {
        t_error("strtod_3900 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_4000
 * @tc.desc      : Test strtod with DBL_MAX string
 * @tc.level     : Level 0
 */
void strtod_4000(void)
{
    char *endptr;
    double result;
    char dbl_max_str[100];
    sprintf(dbl_max_str, "%.17e", DBL_MAX);
    errno = 0;
    result = strtod(dbl_max_str, &endptr);
    if (EXPECT_DOUBLE_CLOSE(result, DBL_MAX, DBL_MAX * 1e-15) && errno == 0) {
        return;
    } else {
        t_error("strtod_4000 failed, result: %f\n", result);
    }
}

/**
 * @tc.name      : strtod_4100
 * @tc.desc      : Test strtod with DBL_MIN string
 * @tc.level     : Level 0
 */
void strtod_4100(void)
{
    char *endptr;
    double result;
    char dbl_min_str[100];
    sprintf(dbl_min_str, "%.17e", DBL_MIN);
    errno = 0;
    result = strtod(dbl_min_str, &endptr);
    if (EXPECT_DOUBLE_CLOSE(result, DBL_MIN, DBL_MIN * 1e-15) && errno == 0) {
        return;
    } else {
        t_error("strtod_4100 failed, result: %f\n", result);
    }
}

int main(int argc, char *argv[])
{
    strtod_0100();
    strtod_0200();
    strtod_0300();
    strtod_0400();
    strtod_0500();
    strtod_0600();
    strtod_0700();
    strtod_0800();
    strtod_0900();
    strtod_1000();
    strtod_1100();
    strtod_1200();
    strtod_1300();
    strtod_1400();
    strtod_1500();
    strtod_1600();
    strtod_1700();
    strtod_1800();
    strtod_1900();
    strtod_2000();
    strtod_2100();
    strtod_2200();
    strtod_2300();
    strtod_2400();
    strtod_2500();
    strtod_2600();
    strtod_2700();
    strtod_2800();
    strtod_2900();
    strtod_3000();
    strtod_3100();
    strtod_3200();
    strtod_3300();
    strtod_3400();
    strtod_3500();
    strtod_3600();
    strtod_3700();
    strtod_3800();
    strtod_3900();
    strtod_4000();
    strtod_4100();
    return t_status;
}

