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
#include <pthread.h>
#include "functionalext.h"

const int SUCCESS = 1;
const int FAILED = 0;

/**
 * @tc.name      : __finite_0100
 * @tc.desc      : The parameter is 0.0, it is judged that the parameter is a finite value.
 * @tc.level     : Level 0
 */
void __finite_0100(void)
{
    int ret = __finite(0.0);
    EXPECT_EQ("__finite_0100", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_0200
 * @tc.desc      : The parameter is DBL_MIN/2.0, and it is judged that the parameter is a finite value
 * @tc.level     : Level 0
 */
void __finite_0200(void)
{
    int ret = __finite(DBL_MIN/2.0);
    EXPECT_EQ("__finite_0200", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_0300
 * @tc.desc      : The parameter is 0.0 / 0.0, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0300(void)
{
    int ret = __finite(0.0 / 0.0);
    EXPECT_EQ("__finite_0300", ret, FAILED);
}

/**
 * @tc.name      : __finite_0400
 * @tc.desc      : The parameter is NAN, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0400(void)
{
    int ret = __finite(NAN);
    EXPECT_EQ("__finite_0400", ret, FAILED);
}

/**
 * @tc.name      : __finite_0500
 * @tc.desc      : The parameter is INFINITY, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0500(void)
{
    int ret = __finite(INFINITY);
    EXPECT_EQ("__finite_0500", ret, FAILED);
}

/**
 * @tc.name      : __finite_0600
 * @tc.desc      : The parameter is -INFINITY, and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0600(void)
{
    int ret = __finite(-INFINITY);
    EXPECT_EQ("__finite_0600", ret, FAILED);
}

/**
 * @tc.name      : __finite_0700
 * @tc.desc      : The parameter is sqrt(-1.0), and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0700(void)
{
    int ret = __finite(sqrt(-1.0));
    EXPECT_EQ("__finite_0700", ret, FAILED);
}

/**
 * @tc.name      : __finite_0800
 * @tc.desc      : The parameter is exp(800), and it is judged that the parameter is not a finite value
 * @tc.level     : Level 0
 */
void __finite_0800(void)
{
    int ret = __finite(exp(800));
    EXPECT_EQ("__finite_0800", ret, FAILED);
}

/**
 * @tc.name      : __finite_0900
 * @tc.desc      : The parameter is normal positive double value (123.456), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_0900(void)
{
    double pos_normal = 123.456;
    int ret = __finite(pos_normal);
    EXPECT_EQ("__finite_0900", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1000
 * @tc.desc      : The parameter is normal negative double value (-789.012), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1000(void)
{
    double neg_normal = -789.012;
    int ret = __finite(neg_normal);
    EXPECT_EQ("__finite_1000", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1100
 * @tc.desc      : The parameter is -0.0 (negative zero), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1100(void)
{
    double neg_zero = -0.0;
    int ret = __finite(neg_zero);
    EXPECT_EQ("__finite_1100", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1200
 * @tc.desc      : The parameter is DBL_MAX (max double value), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1200(void)
{
    double dbl_max = DBL_MAX;
    int ret = __finite(dbl_max);
    EXPECT_EQ("__finite_1200", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1300
 * @tc.desc      : The parameter is DBL_MIN (min double value), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1300(void)
{
    double dbl_min = DBL_MIN;
    int ret = __finite(dbl_min);
    EXPECT_EQ("__finite_1300", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1400
 * @tc.desc      : The parameter is DBL_MIN/10.0 (tiny double value), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1400(void)
{
    double tiny_val = DBL_MIN / 10.0;
    int ret = __finite(tiny_val);
    EXPECT_EQ("__finite_1400", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1500
 * @tc.desc      : The parameter is INFINITY+1000.0 (infinity after addition), judge it is not a finite value
 * @tc.level     : Level 0
 */
void __finite_1500(void)
{
    double inf_add = INFINITY + 1000.0;
    int ret = __finite(inf_add);
    EXPECT_EQ("__finite_1500", ret, FAILED);
}

/**
 * @tc.name      : __finite_1600
 * @tc.desc      : The parameter is INFINITY*2.5 (infinity after multiplication), judge it is not a finite value
 * @tc.level     : Level 0
 */
void __finite_1600(void)
{
    double inf_mul = INFINITY * 2.5;
    int ret = __finite(inf_mul);
    EXPECT_EQ("__finite_1600", ret, FAILED);
}

/**
 * @tc.name      : __finite_1700
 * @tc.desc      : The parameter is float value converted to double (123.456f), judge it is a finite value
 * @tc.level     : Level 0
 */
void __finite_1700(void)
{
    float f_val = 123.456f;
    double d_val = (double)f_val;
    int ret = __finite(d_val);
    EXPECT_EQ("__finite_1700", ret, SUCCESS);
}

/**
 * @tc.name      : __finite_1800
 * @tc.desc      : Repeat judge the same finite value (789.012) 5 times, verify stability (all finite)
 * @tc.level     : Level 0
 */
void __finite_1800(void)
{
    double stable_finite = 789.012;
    int ret;
    for (int i = 0; i < 5; i++) {
        ret = __finite(stable_finite);
        EXPECT_EQ("__finite_1800", ret, SUCCESS);
    }
}

/**
 * @tc.name      : __finite_1900
 * @tc.desc      : Repeat judge the same infinity value 5 times, verify stability (all non-finite)
 * @tc.level     : Level 0
 */
void __finite_1900(void)
{
    double stable_inf = INFINITY;
    int ret;
    for (int i = 0; i < 5; i++) {
        ret = __finite(stable_inf);
        EXPECT_EQ("__finite_1900", ret, FAILED);
    }
}

/**
 * @tc.name      : __finite_2000
 * @tc.desc      : Compare __finite and finite for double finite value (123.456), verify consistency
 * @tc.level     : Level 0
 */
void __finite_2000(void)
{
    double test_val = 123.456;
    int ret1 = __finite(test_val);
    int ret2 = finite(test_val);
    EXPECT_EQ("__finite_2000", (ret1 != 0) == (ret2 != 0), SUCCESS);
}

/**
 * @tc.name      : __finite_2100
 * @tc.desc      : Compare __finite and finite for double infinity value, verify consistency
 * @tc.level     : Level 0
 */
void __finite_2100(void)
{
    double test_val = INFINITY;
    int ret1 = __finite(test_val);
    int ret2 = finite(test_val);
    EXPECT_EQ("__finite_2100", (ret1 != 0) == (ret2 != 0), SUCCESS);
}

void* mt_worker(void* arg)
{
    __finite_0100();
    return NULL;
}

/**
 * @tc.name      : __finite_2200
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __finite_2200(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ("__finite_2200", pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(void)
{
    __finite_0100();
    __finite_0200();
    __finite_0300();
    __finite_0400();
    __finite_0500();
    __finite_0600();
    __finite_0700();
    __finite_0800();
    __finite_0900();
    __finite_1000();
    __finite_1100();
    __finite_1200();
    __finite_1300();
    __finite_1400();
    __finite_1500();
    __finite_1600();
    __finite_1700();
    __finite_1800();
    __finite_1900();
    __finite_2000();
    __finite_2100();
    __finite_2200();
    return t_status;
}