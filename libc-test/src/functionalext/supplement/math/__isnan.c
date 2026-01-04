/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#include <float.h>
#include "functionalext.h"

/**
 * @tc.name      : __isnan_0100
 * @tc.desc      : Test __isnan with float type
 * @tc.level     : Level 0
 */
void __isnan_0100(void)
{
    float floatValue = 0.0f / 0.0f;
    int ret = __isnan(floatValue);
    EXPECT_TRUE("__isnan_0100", ret);
}

/**
 * @tc.name      : __isnan_0200
 * @tc.desc      : Test __isnan with double type
 * @tc.level     : Level 0
 */
void __isnan_0200(void)
{
    double doubleValue = 0.0 / 0.0;
    int ret = __isnan(doubleValue);
    EXPECT_TRUE("__isnan_0200", ret);
}

/**
 * @tc.name      : __isnan_0300
 * @tc.desc      : Test __isnan with long double type
 * @tc.level     : Level 0
 */
void __isnan_0300(void)
{
    long double longDoubleValue = 0.0L / 0.0L;
    int ret = __isnan(longDoubleValue);
    EXPECT_TRUE("__isnan_0300", ret);
}

/**
 * @tc.name      : __isnan_0400
 * @tc.desc      : Test __isnan with normal float type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0400(void)
{
    float floatValue = 3.14f;
    int ret = __isnan(floatValue);
    EXPECT_FALSE("__isnan_0400", ret);
}

/**
 * @tc.name      : __isnan_0500
 * @tc.desc      : Test __isnan with normal double type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0500(void)
{
    double doubleValue = 10.2;
    int ret = __isnan(doubleValue);
    EXPECT_FALSE("__isnan_0500", ret);
}

/**
 * @tc.name      : __isnan_0600
 * @tc.desc      : Test __isnan with normal long double type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0600(void)
{
    long double longDoubleValue = 12.53L;
    int ret = __isnan(longDoubleValue);
    EXPECT_FALSE("__isnan_0600", ret);
}

/**
 * @tc.name      : __isnan_0700
 * @tc.desc      : Test __isnan with float infinity type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0700(void)
{
    float floatInf  = INFINITY;
    int ret = __isnan(floatInf);
    EXPECT_FALSE("__isnan_0700", ret);
    float floatNegInf  = -INFINITY;
    int ret1 = __isnan(floatNegInf);
    EXPECT_FALSE("__isnan_0700_1", ret1);
}

/**
 * @tc.name      : __isnan_0800
 * @tc.desc      : Test __isnan with double HUGE_VAL type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0800(void)
{
    double doubleInf  = HUGE_VAL;
    int ret = __isnan(doubleInf);
    EXPECT_FALSE("__isnan_0800", ret);
    double doubleNegInf  = -HUGE_VAL;
    int ret1 = __isnan(doubleNegInf);
    EXPECT_FALSE("__isnan_0800_1", ret1);
}

/**
 * @tc.name      : __isnan_0900
 * @tc.desc      : Test __isnan with long double HUGE_VALL type (non-NaN)
 * @tc.level     : Level 0
 */
void __isnan_0900(void)
{
    long double longDoubleInf   = HUGE_VALL;
    int ret = __isnan(longDoubleInf);
    EXPECT_FALSE("__isnan_0900", ret);
    long double longDoubleNegInf  = -HUGE_VALL;
    int ret1 = __isnan(longDoubleNegInf);
    EXPECT_FALSE("__isnan_0900_1", ret1);
}

int main(void)
{
    __isnan_0100();
    __isnan_0200();
    __isnan_0300();
    __isnan_0400();
    __isnan_0500();
    __isnan_0600();
    __isnan_0700();
    __isnan_0800();
    __isnan_0900();
    return t_status;
}