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

#include <float.h>
#include <math.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;
const int32_t COUNT_ONE = 1;

/**
 * @tc.name      : __signbit_0100
 * @tc.desc      : When the float value is 0, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0100()
{
    double x = 0;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0200
 * @tc.desc      : When the float value is 3.14, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0200()
{
    double x = 3.14;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0200", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0300
 * @tc.desc      : When the float value is -3.14, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0300()
{
    double x = -3.14;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0300", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_0400
 * @tc.desc      : When the double value is negative zero (-0.0),
                   test the return value of the function
 * @tc.level     : Level 1
 */
void __signbit_0400()
{
    double x = -0.0;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0400", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_0500
 * @tc.desc      : When the double value is positive infinity (INFINITY),
                   test the return value of the function
 * @tc.level     : Level 1
 */
void __signbit_0500()
{
    double x = INFINITY;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0500", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0600
 * @tc.desc      : When the double value is negative infinity (-INFINITY),
                   test the return value of the function
 * @tc.level     : Level 1
 */
void __signbit_0600()
{
    double x = -INFINITY;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0600", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_0700
 * @tc.desc      : When the double value is NAN, test the return value of the function
 * @tc.level     : Level 1
 */
void __signbit_0700()
{
    double x = NAN;
    int result = __signbit(x);
    // NAN's sign bit is preserved, here we use negative NAN (optional, positive NAN returns COUNT_ZERO)
    double x_neg_nan = -NAN;
    int result_neg_nan = __signbit(x_neg_nan);
    EXPECT_EQ("_signbit_0700", result_neg_nan, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_0800
 * @tc.desc      : When the float value is a small positive number (0.0001f),
                   test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0800()
{
    float x = 0.0001f;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0800", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0900
 * @tc.desc      : When the float value is a small negative number (-0.0001f),
                   test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0900()
{
    float x = -0.0001f;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0900", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_1000
 * @tc.desc      : When the long double value is positive (123.456L), test the return value
                   of the function
 * @tc.level     : Level 0
 */
void __signbit_1000()
{
    long double x = 123.456L;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1000", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_1100
 * @tc.desc      : When the long double value is negative (-123.456L), test the return value
                   of the function
 * @tc.level     : Level 0
 */
void __signbit_1100()
{
    long double x = -123.456L;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1100", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_1200
 * @tc.desc      : When the double value is the maximum positive normal number (DBL_MAX),
                   test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_1200()
{
    double x = DBL_MAX;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1200", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_1300
 * @tc.desc      : When the double value is the minimum negative normal number (-DBL_MAX),
                   test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_1300()
{
    double x = -DBL_MAX;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1300", result, COUNT_ONE);
}

/**
 * @tc.name      : __signbit_1400
 * @tc.desc      : When the double value is a positive subnormal number (DBL_MIN / 2.0),
                   test the return value of the function
 * @tc.level     : Level 2
 */
void __signbit_1400()
{
    double x = DBL_MIN / 2.0;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1400", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_1500
 * @tc.desc      : When the double value is a negative subnormal number (-(DBL_MIN / 2.0)),
                   test the return value of the function
 * @tc.level     : Level 2
 */
void __signbit_1500()
{
    double x = -(DBL_MIN / 2.0);
    int result = __signbit(x);
    EXPECT_EQ("_signbit_1500", result, COUNT_ONE);
}

TEST_FUN G_Fun_Array[] = {
    __signbit_0100,
    __signbit_0200,
    __signbit_0300,
    __signbit_0400,
    __signbit_0500,
    __signbit_0600,
    __signbit_0700,
    __signbit_0800,
    __signbit_0900,
    __signbit_1000,
    __signbit_1100,
    __signbit_1200,
    __signbit_1300,
    __signbit_1400,
    __signbit_1500,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}