/*
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

#include <float.h>
#include <pthread.h>
#include "functionalext.h"

#define ASSERT_ISNANF_RESULT(x, expected) \
    do { \
        int ret = __isnanf(x); \
        EXPECT_TRUE(__FUNCTION__, ret == (expected)); \
    } while (0)

const int RETURN_ONE = 1;
const int RETURN_ZERO = 0;
const float TEST_FLOAT = 555.5f;

/**
 * @tc.name      : __isnanf_0100
 * @tc.desc      : The parameter is HUGE_VALF, -HUGE_VALF, NAN and num, __isnanf can correctly return.
 * @tc.level     : Level 0
 */
void __isnanf_0100(void)
{
    EXPECT_EQ("__isnanf_0100", __isnanf(HUGE_VALF), RETURN_ZERO);
    EXPECT_EQ("__isnanf_0100", __isnanf(-HUGE_VALF), RETURN_ZERO);
    EXPECT_EQ("__isnanf_0100", __isnanf(NAN), RETURN_ONE);
    EXPECT_EQ("__isnanf_0100", __isnanf(TEST_FLOAT), RETURN_ZERO);
}

/**
 * @tc.name      : __isnanf_0200
 * @tc.desc      : Test core functions.
 * @tc.level     : Level 0
 */
void __isnanf_0200(void)
{
    float nan_std = NAN;
    float nan_gen1 = 0.0f / 0.0f;
    float nan_gen2 = sqrtf(-1.0f);
    float nan_gen3 = INFINITY - INFINITY;
    ASSERT_ISNANF_RESULT(nan_std, 1);
    ASSERT_ISNANF_RESULT(nan_gen1, 1);
    ASSERT_ISNANF_RESULT(nan_gen2, 1);
    ASSERT_ISNANF_RESULT(nan_gen3, 1);

    float non_nan_values[] = {
        0.0f, -0.0f, 123.456f, -789.0f,
        M_PI, FLT_MAX, FLT_MIN,
        INFINITY, -INFINITY,
        sinf(1.0f), powf(2.0f, 10.0f)
    };
    int non_nan_count = sizeof(non_nan_values) / sizeof(float);
    for (int i = 0; i < non_nan_count; i++) {
        ASSERT_ISNANF_RESULT(non_nan_values[i], 0);
    }
}

/**
 * @tc.name      : __isnanf_0300
 * @tc.desc      : Test boundary scenarios.
 * @tc.level     : Level 0
 */
void __isnanf_0300(void)
{
    float near_max = FLT_MAX * 0.999f;
    float near_min = FLT_MIN * 1.001f;
    ASSERT_ISNANF_RESULT(near_max, 0);
    ASSERT_ISNANF_RESULT(near_min, 0);

    float inf_add = INFINITY + 1000.0f;
    float inf_mul = -INFINITY * 2.0f;
    ASSERT_ISNANF_RESULT(inf_add, 0);
    ASSERT_ISNANF_RESULT(inf_mul, 0);

    double d_nan = 0.0 / 0.0;
    double d_finite = 3.141592653589793;
    ASSERT_ISNANF_RESULT((float)d_nan, 1);
    ASSERT_ISNANF_RESULT((float)d_finite, 0);

    float tiny_calc = FLT_MIN / 2.0f;
    ASSERT_ISNANF_RESULT(tiny_calc, 0);
}

/**
 * @tc.name      : __isnanf_0400
 * @tc.desc      : Test specific scenario.
 * @tc.level     : Level 0
 */
void __isnanf_0400(void)
{
    union FloatUnion {
        float f;
        uint32_t u;
    } qnan, snan;

    qnan.u = 0x7FC00001;
    snan.u = 0x7F800001;
    ASSERT_ISNANF_RESULT(qnan.f, 1);
    ASSERT_ISNANF_RESULT(snan.f, 1);
}

void* mt_worker(void* arg)
{
    __isnanf_0100();
    return NULL;
}

/**
 * @tc.name      : __isnanf_0500
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __isnanf_0500(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(__FUNCTION__, pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    __isnanf_0100();
    __isnanf_0200();
    __isnanf_0300();
    __isnanf_0400();
    __isnanf_0500();

    return t_status;
}