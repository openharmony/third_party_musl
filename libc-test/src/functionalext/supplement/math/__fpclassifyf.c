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
 * @tc.name      : __fpclassifyf_0100
 * @tc.desc      : The parameter is valid, x is 1.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyf_0100(void)
{
    float x = 1.0;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0100", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyf_0200
 * @tc.desc      : The parameter is valid, x is NAN, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0200(void)
{
    float x = NAN;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0200", ret, FP_NAN);
}

/**
 * @tc.name      : __fpclassifyf_0300
 * @tc.desc      : The parameter is valid, x is INFINITY, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0300(void)
{
    float x = INFINITY;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0300", ret, FP_INFINITE);
}

/**
 * @tc.name      : __fpclassifyf_0400
 * @tc.desc      : The parameter is valid, x is -0.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0400(void)
{
    float x = -0.0;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0400", ret, FP_ZERO);
}

/**
* @tc.name      : __fpclassifyf_0500
* @tc.desc      : The parameter is valid, x is a negative normal float number (-3.14f),
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 0
*/
void __fpclassifyf_0500(void)
{
    float x = -3.14f;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0500", ret, FP_NORMAL);
}

/**
* @tc.name      : __fpclassifyf_0600
* @tc.desc      : The parameter is valid, x is a positive subnormal (denormal) float number,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 2
*/
void __fpclassifyf_0600(void)
{
    // Smallest positive subnormal float value
    float x = FLT_MIN / 2.0f;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0600", ret, FP_SUBNORMAL);
}

/**
* @tc.name      : __fpclassifyf_0700
* @tc.desc      : The parameter is valid, x is a negative subnormal (denormal) float number,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 2
*/
void __fpclassifyf_0700(void)
{
    // Smallest negative subnormal float value
    float x = -(FLT_MIN / 2.0f);
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0700", ret, FP_SUBNORMAL);
}

/**
* @tc.name      : __fpclassifyf_0800
* @tc.desc      : The parameter is valid, x is the maximum positive normal float number,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 0
*/
void __fpclassifyf_0800(void)
{
    float x = FLT_MAX;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0800", ret, FP_NORMAL);
}

/**
* @tc.name      : __fpclassifyf_0900
* @tc.desc      : The parameter is valid, x is the minimum negative normal float number,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 0
*/
void __fpclassifyf_0900(void)
{
    float x = -FLT_MAX;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0900", ret, FP_NORMAL);
}

/**
* @tc.name      : __fpclassifyf_1000
* @tc.desc      : The parameter is valid, x is positive 0.0,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 1
*/
void __fpclassifyf_1000(void)
{
    float x = 0.0f;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_1000", ret, FP_ZERO);
}

/**
* @tc.name      : __fpclassifyf_1100
* @tc.desc      : The parameter is valid, x is negative infinity,
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 1
*/
void __fpclassifyf_1100(void)
{
    float x = -INFINITY;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_1100", ret, FP_INFINITE);
}

/**
* @tc.name      : __fpclassifyf_1200
* @tc.desc      : The parameter is valid, x is a positive normal small float number (0.5f),
                  can get the integer value matching the classification macro constant.
* @tc.level     : Level 0
*/
void __fpclassifyf_1200(void)
{
    float x = 0.5f;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_1200", ret, FP_NORMAL);
}

int main(int argc, char *argv[])
{
    __fpclassifyf_0100();
    __fpclassifyf_0200();
    __fpclassifyf_0300();
    __fpclassifyf_0400();
    __fpclassifyf_0500();
    __fpclassifyf_0600();
    __fpclassifyf_0700();
    __fpclassifyf_0800();
    __fpclassifyf_0900();
    __fpclassifyf_1000();
    __fpclassifyf_1100();
    __fpclassifyf_1200();
    return t_status;
}