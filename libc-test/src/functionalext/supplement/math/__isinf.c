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

const int RETURN_ONE = 1;
const int RETURN_ZERO = 0;
const float TEST_FLOAT = 555.5f;

#define ASSERT_ISINF_RESULT(x, expected)                           \
    do {                                                           \
        int ret = __isinf(x);                                      \
        EXPECT_TRUE(__FUNCTION__, ret == (expected));                \
    } while (0)

/**
 * @tc.name      : __isinf_0100
 * @tc.desc      : The parameter is HUGE_VALF, -HUGE_VALF, NAN and num, __isinf can correctly return.
 * @tc.level     : Level 0
 */
void __isinf_0100(void)
{
    EXPECT_EQ("__isinf_0100", __isinf(HUGE_VALF), RETURN_ONE);
    EXPECT_EQ("__isinf_0100", __isinf(-HUGE_VALF), -RETURN_ONE);
    EXPECT_EQ("__isinf_0100", __isinf(NAN), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __isinf(TEST_FLOAT), RETURN_ZERO);
}

/**
 * @tc.name      : __isinf_0200
 * @tc.desc      : The parameter is INFINITY, HUGE_VAL, HUGE_VALL, -HUGE_VAL, -HUGE_VALL, __isinf can correctly return.
 * @tc.level     : Level 0
 */
void __isinf_0200(void)
{
    EXPECT_EQ("__isinf_0200", __isinf(INFINITY), RETURN_ONE);
    EXPECT_EQ("__isinf_0200", __isinf(HUGE_VAL), RETURN_ONE);
    EXPECT_EQ("__isinf_0200", __isinf(HUGE_VALL), RETURN_ONE);
    EXPECT_EQ("__isinf_0200", __isinf(-HUGE_VAL), -RETURN_ONE);
    EXPECT_EQ("__isinf_0200", __isinf(-HUGE_VALL), -RETURN_ONE);
    EXPECT_EQ("__isinf_0200", __isinf(0), RETURN_ZERO);
}

/**
 * @tc.name      : __isinf_0300
 * @tc.desc      : Test core functions.
 * @tc.level     : Level 0
 */
void __isinf_0300(void)
{
    double inf_pos_gen = 1.0 / 0.0;
    ASSERT_ISINF_RESULT(inf_pos_gen, 1);
    double inf_neg_gen = -1.0 / 0.0;
    ASSERT_ISINF_RESULT(inf_neg_gen, -1);
    double non_inf_values[] = {
        0.0, -0.0, 123.456, -789.0,
        M_PI, DBL_MAX, DBL_MIN,
        NAN, 0.0 / 0.0,
        sin(1.0), sqrt(2.0)
    };
    int non_inf_count = sizeof(non_inf_values) / sizeof(double);
    for (int i = 0; i < non_inf_count; i++) {
        ASSERT_ISINF_RESULT(non_inf_values[i], 0);
    }
}

/**
 * @tc.name      : __isinf_0400
 * @tc.desc      : Test boundary scenarios.
 * @tc.level     : Level 0
 */
void __isinf_0400(void)
{
    double inf_add = INFINITY + 1000.0;
    double inf_mul = -INFINITY * 2.0;
    double inf_nan = INFINITY - INFINITY;
    ASSERT_ISINF_RESULT(inf_add, 1);
    ASSERT_ISINF_RESULT(inf_mul, -1);
    ASSERT_ISINF_RESULT(inf_nan, 0);

    double near_max = DBL_MAX * 0.999;
    double near_max_calc = DBL_MAX / 2.0;
    ASSERT_ISINF_RESULT(near_max, 0);
    ASSERT_ISINF_RESULT(near_max_calc, 0);

    double tiny_pos = DBL_MIN * 1.0e-3;
    double tiny_neg = -DBL_MIN * 1.0e-3;
    ASSERT_ISINF_RESULT(tiny_pos, 0);
    ASSERT_ISINF_RESULT(tiny_neg, 0);
}

/**
 * @tc.name      : __isinf_0500
 * @tc.desc      : Test compatibility of different floating-point types.
 * @tc.level     : Level 0
 */
void __isinf_0500(void)
{
    float f_inf_pos = INFINITY;
    float f_inf_neg = -INFINITY;
    float f_finite = 3.14159f;
    ASSERT_ISINF_RESULT(f_inf_pos, 1);
    ASSERT_ISINF_RESULT(f_inf_neg, -1);
    ASSERT_ISINF_RESULT(f_finite, 0);

    long double ld_inf_pos = INFINITY;
    long double ld_inf_neg = -INFINITY;
    long double ld_finite = 12345.6789L;
    ASSERT_ISINF_RESULT((double)ld_inf_pos, 1);
    ASSERT_ISINF_RESULT((double)ld_inf_neg, -1);
    ASSERT_ISINF_RESULT((double)ld_finite, 0);
}

void* mt_worker(void* arg)
{
    __isinf_0100();
    return NULL;
}

/**
 * @tc.name      : __isinf_0600
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __isinf_0600(void)
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
    __isinf_0100();
    __isinf_0200();
    __isinf_0300();
    __isinf_0400();
    __isinf_0500();
    __isinf_0600();

    return t_status;
}