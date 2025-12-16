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

#include "mtest.h"
#include "functionalext.h"

/**
 * @tc.name      : signbitl_0100
 * @tc.desc      : The sign bit of the returned argument is 0 (The value of the argument of type long is 0)
 * @tc.level     : Level 0
 */
void signbitl_0100(void)
{
    long double x = 0.0L;
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0100", result, 0);
}

/**
 * @tc.name      : signbitl_0200
 * @tc.desc      : The sign bit of the returned argument is 0 (The value of the argument of type long is positive)
 * @tc.level     : Level 0
 */
void signbitl_0200(void)
{
    long double x = 3.14;
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0200", result, 0);
}

/**
 * @tc.name      : signbitl_0300
 * @tc.desc      : The sign bit of the returned parameter is 1 (The parameter value of type long is negative)
 * @tc.level     : Level 0
 */
void signbitl_0300(void)
{
    long double x = -3.14;
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0300", result, 1);
}

/**
 * @tc.name      : signbitl_0400
 * @tc.desc      : The sign bit of positive infinity (+INFINITY) is 0
 * @tc.level     : Level 1
 */
void signbitl_0400(void)
{
    long double x = INFINITY;  // Positive infinity (long double type)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0400", result, 0);
}

/**
 * @tc.name      : signbitl_0500
 * @tc.desc      : The sign bit of negative infinity (-INFINITY) is 1
 * @tc.level     : Level 1
 */
void signbitl_0500(void)
{
    long double x = -INFINITY; // Negative infinity (long double type)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0500", result, 1);
}

/**
 * @tc.name      : signbitl_0600
 * @tc.desc      : The sign bit of positive zero (+0.0L) is 0
 * @tc.level     : Level 1
 * @note         : Distinguish between ordinary zero and explicit positive zero, verify sign bit consistency
 */
void signbitl_0600(void)
{
    long double x = +0.0L;     // Explicit positive zero
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0600", result, 0);
}

/**
 * @tc.name      : signbitl_0700
 * @tc.desc      : The sign bit of negative zero (-0.0L) is 1
 * @tc.level     : Level 1
 * @note         : Negative zero is a floating-point specific scenario, numerically equal to 0 but with sign bit 1
 */
void signbitl_0700(void)
{
    long double x = -0.0L;     // Negative zero (long double type)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0700", result, 1);
}

/**
 * @tc.name      : signbitl_0800
 * @tc.desc      : The sign bit of NaN (Not a Number, positive NaN) is 0
 * @tc.level     : Level 2
 * @note         : NaN has no positive/negative semantics itself, but stores a sign bit; positive NaN has sign bit 0
 */
void signbitl_0800(void)
{
    long double x = NAN;       // Positive NaN (default NaN is positive)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0800", result, 0);
}

/**
 * @tc.name      : signbitl_0900
 * @tc.desc      : The sign bit of the maximum value of long double (positive) is 0
 * @tc.level     : Level 1
 * @note         : Boundary value test to verify sign bit recognition in extreme value scenarios
 */
void signbitl_0900(void)
{
    long double x = LDBL_MAX;  // Maximum value of long double type (positive)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_0900", result, 0);
}


/**
 * @tc.name      : signbitl_1000
 * @tc.desc      : The sign bit of the minimum value of long double (negative extreme) is 1
 * @tc.level     : Level 1
 * @note         : LDBL_MIN is the smallest positive value; -LDBL_MAX is used here to represent negative extreme value
 */
void signbitl_1000(void)
{
    long double x = -LDBL_MAX; // Maximum negative value of long double type
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_1000", result, 1);
}

/**
 * @tc.name      : signbitl_1100
 * @tc.desc      : The sign bit of an extremely small positive number (close to 0) is 0
 * @tc.level     : Level 1
 * @note         : Verify sign bit recognition of small values close to the underflow threshold
 */
void signbitl_1100(void)
{
    long double x = LDBL_MIN;  // Smallest positive value of long double type
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_1100", result, 0);
}

/**
 * @tc.name      : signbitl_1200
 * @tc.desc      : The sign bit of an extremely small negative number (close to 0) is 1
 * @tc.level     : Level 1
 */
void signbitl_1200(void)
{
    long double x = -LDBL_MIN; // Smallest negative value of long double type
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_1200", result, 1);
}

/**
 * @tc.name      : signbitl_1300
 * @tc.desc      : The sign bit of a positive number in scientific notation is 0
 * @tc.level     : Level 1
 * @note         : Verify consistency of sign bit recognition across different numeric formats
 */
void signbitl_1300(void)
{
    long double x = 1.2345e10L; // Positive number in scientific notation (long double)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_1300", result, 0);
}

/**
 * @tc.name      : signbitl_1400
 * @tc.desc      : The sign bit of a negative number in scientific notation is 1
 * @tc.level     : Level 1
 */
void signbitl_1400(void)
{
    long double x = -9.8765e-5L; // Negative number in scientific notation (long double)
    int result = __signbitl(x);
    EXPECT_EQ("signbitl_1400", result, 1);
}

int main(int argc, char *argv[])
{
    signbitl_0100();
    signbitl_0200();
    signbitl_0300();
    signbitl_0400();
    signbitl_0500();
    signbitl_0600();
    signbitl_0700();
    signbitl_0800();
    signbitl_0900();
    signbitl_1000();
    signbitl_1100();
    signbitl_1200();
    signbitl_1300();
    signbitl_1400();
    return t_status;
}