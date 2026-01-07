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

/**
 * @tc.name      : __fpclassifyl_0100
 * @tc.desc      : The parameter is valid, x is 1.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_0100(void)
{
    long double x = 1.0;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0100", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_0200
 * @tc.desc      : The parameter is valid, x is NAN, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyl_0200(void)
{
    long double x = NAN;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0200", ret, FP_NAN);
}

/**
 * @tc.name      : __fpclassifyl_0300
 * @tc.desc      : The parameter is valid, x is INFINITY, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyl_0300(void)
{
    long double x = INFINITY;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0300", ret, FP_INFINITE);
}

/**
 * @tc.name      : __fpclassifyl_0400
 * @tc.desc      : The parameter is valid, x is -0.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyl_0400(void)
{
    long double x = -0.0;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0400", ret, FP_ZERO);
}

/**
 * @tc.name      : __fpclassifyl_0500
 * @tc.desc      : The parameter is valid, x is a negative normal long double number (-3.14L),
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_0500(void)
{
    long double x = -3.14L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0500", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_0600
 * @tc.desc      : The parameter is valid, x is a positive subnormal (denormal) long double number,
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 2
 */
void __fpclassifyl_0600(void)
{
    // Smallest positive subnormal long double value
    long double x = LDBL_MIN / 2.0L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0600", ret, FP_SUBNORMAL);
}

/**
 * @tc.name      : __fpclassifyl_0700
 * @tc.desc      : The parameter is valid, x is a negative subnormal (denormal) long double number,
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 2
 */
void __fpclassifyl_0700(void)
{
    // Smallest negative subnormal long double value
    long double x = -(LDBL_MIN / 2.0L);
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0700", ret, FP_SUBNORMAL);
}

/**
 * @tc.name      : __fpclassifyl_0800
 * @tc.desc      : The parameter is valid, x is the maximum positive normal long double number,
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_0800(void)
{
    long double x = LDBL_MAX;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0800", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_0900
 * @tc.desc      : The parameter is valid, x is the minimum negative normal long double number,
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_0900(void)
{
    long double x = -LDBL_MAX;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_0900", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_1000
 * @tc.desc      : The parameter is valid, x is positive 0.0, can get the integer value matching
                   the classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyl_1000(void)
{
    long double x = 0.0L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_1000", ret, FP_ZERO);
}

/**
 * @tc.name      : __fpclassifyl_1100
 * @tc.desc      : The parameter is valid, x is negative infinity, can get the integer value matching
                   the classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyl_1100(void)
{
    long double x = -INFINITY;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_1100", ret, FP_INFINITE);
}

/**
 * @tc.name      : __fpclassifyl_1200
 * @tc.desc      : The parameter is valid, x is a positive normal small long double number (0.5L),
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_1200(void)
{
    long double x = 0.5L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_1200", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_1300
 * @tc.desc      : The parameter is valid, x is a positive normal large long double number (1e300L),
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_1300(void)
{
    long double x = 1e300L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_1300", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyl_1400
 * @tc.desc      : The parameter is valid, x is a negative normal small long double number (-0.25L),
                   can get the integer value matching the classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyl_1400(void)
{
    long double x = -0.25L;
    int ret = __fpclassifyl(x);
    EXPECT_EQ("__fpclassifyl_1400", ret, FP_NORMAL);
}

int main(int argc, char *argv[])
{
    __fpclassifyl_0100();
    __fpclassifyl_0200();
    __fpclassifyl_0300();
    __fpclassifyl_0400();
    __fpclassifyl_0500();
    __fpclassifyl_0600();
    __fpclassifyl_0700();
    __fpclassifyl_0800();
    __fpclassifyl_0900();
    __fpclassifyl_1000();
    __fpclassifyl_1100();
    __fpclassifyl_1200();
    __fpclassifyl_1300();
    __fpclassifyl_1400();
    return t_status;
}