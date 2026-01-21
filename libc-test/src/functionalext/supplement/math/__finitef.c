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

#ifndef FLT_INFINITY
#define FLT_INFINITY (INFINITY)
#endif

const int RETURN_ONE = 1;
const int RETURN_ZERO = 0;
const int TEST_NUM_800 = 800;
const int TEST_NUM_2 = 2;
const float TEST_FLOAT = 555.5f;

/**
 * @tc.name      : __finitef_0100
 * @tc.desc      : The parameter is HUGE_VALF, -HUGE_VALF, NAN and num, __finitef can correctly return.
 * @tc.level     : Level 0
 */
void __finitef_0100(void)
{
    EXPECT_EQ("__isinf_0100", __finitef(HUGE_VALF), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __finitef(-HUGE_VALF), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __finitef(NAN), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __finitef(TEST_FLOAT), RETURN_ONE);
}

/**
 * @tc.name      : __finitef_0200
 * @tc.desc      : The parameter is exp(800), DBL_MIN / 2 and 0, __finitef can correctly return.
 * @tc.level     : Level 0
 */
void __finitef_0200(void)
{
    EXPECT_EQ("__isinf_0100", __finitef(exp(TEST_NUM_800)), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __finitef(DBL_MIN / TEST_NUM_2), RETURN_ONE);
    EXPECT_EQ("__isinf_0100", __finitef(0), RETURN_ONE);
}

/**
 * @tc.name      : __finitef_0300
 * @tc.desc      : Basic functionality test.
 * @tc.level     : Level 0
 */
void __finitef_0300(void)
{
    int ret_val = 0;
    float pos_normal1 = 123.456f;
    float pos_normal2 = 99999.999f;
    float pos_normal3 = 0.000123f;

    ret_val = __finitef(pos_normal1);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(pos_normal2);
    EXPECT_NE(__FUNCTION__, ret_val, 0);
    ret_val = __finitef(pos_normal3);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float neg_normal1 = -789.012f;
    float neg_normal2 = -88888.888f;
    float neg_normal3 = -0.000456f;

    ret_val = __finitef(neg_normal1);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(neg_normal2);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(neg_normal3);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float zero_pos = 0.0f;
    float zero_neg = -0.0f;

    ret_val = __finitef(zero_pos);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(zero_neg);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float flt_max = FLT_MAX;
    float flt_min = FLT_MIN;

    ret_val = __finitef(flt_max);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(flt_min);
    EXPECT_NE(__FUNCTION__, ret_val, 0);
}

/**
 * @tc.name      : __finitef_0400
 * @tc.desc      : Non-finite Value Check Test.
 * @tc.level     : Level 0
 */
void __finitef_0400(void)
{
    int ret_val = 0;
    float pos_inf = 0.0f;
    float neg_inf = 0.0f;
    float nan_val1 = 0.0f;
    float nan_val2 = 0.0f;

    pos_inf = FLT_INFINITY;
    neg_inf = -FLT_INFINITY;
    float pos_inf_calc = 1.0f / 0.0f;
    float neg_inf_calc = -1.0f / 0.0f;

    nan_val1 = NAN;
    nan_val2 = 0.0f / 0.0f;

    ret_val = __finitef(pos_inf);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(pos_inf_calc);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(neg_inf);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(neg_inf_calc);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(nan_val1);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __finitef(nan_val2);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
}

/**
 * @tc.name      : __finitef_0500
 * @tc.desc      : Boundary and Compatibility Testing.
 * @tc.level     : Level 0
 */
void __finitef_0500(void)
{
    int ret_val = 0;
    float tiny_val1 = FLT_MIN / 10.0f;
    float tiny_val2 = -FLT_MIN / 5.0f;

    ret_val = __finitef(tiny_val1);
    EXPECT_NE(__FUNCTION__, ret_val, 0);
    ret_val = __finitef(tiny_val2);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float inf_add = FLT_INFINITY + 1000.0f;
    float inf_sub = FLT_INFINITY - 5000.0f;
    float inf_mul = FLT_INFINITY * 2.5f;
    float inf_div = FLT_INFINITY / 3.0f;

    ret_val = __finitef(inf_add);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
    ret_val = __finitef(inf_sub);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
    ret_val = __finitef(inf_mul);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
    ret_val = __finitef(inf_div);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float f_val = 123.456f;
    double d_val = 456.789;

    ret_val = __finitef(f_val);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __finitef((float)d_val);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float stable_finite = 789.012f;
    float stable_infinite = FLT_INFINITY;

    for (int i = 0; i < 5; i++) {
        ret_val = __finitef(stable_finite);
        EXPECT_NE(__FUNCTION__, ret_val, 0);
    }

    for (int i = 0; i < 5; i++) {
        ret_val = __finitef(stable_infinite);
        EXPECT_EQ(__FUNCTION__, ret_val, 0);
    }
}

/**
 * @tc.name      : __finitef_0600
 * @tc.desc      : Function Comparison Test.
 * @tc.level     : Level 0
 */
void __finitef_0600(void)
{
    float test_vals[] = {
        123.456f,
        -789.012f,
        0.0f,
        -0.0f,
        FLT_MAX,
        FLT_MIN,
        FLT_INFINITY,
        -FLT_INFINITY,
        NAN
    };
    int val_count = sizeof(test_vals) / sizeof(float);
    int retf, ret;
    int match_flag = 1;

    for (int i = 0; i < val_count; i++) {
        retf = __finitef(test_vals[i]);
        ret = finite(test_vals[i]);
        if ((retf != 0 && ret == 0) || (retf == 0 && ret != 0)) {
            match_flag = 0;
        }
    }

    EXPECT_TRUE(__FUNCTION__, match_flag);
}

void* mt_worker(void* arg)
{
    __finitef_0100();
    return NULL;
}

/**
 * @tc.name      : __finitef_0700
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __finitef_0700(void)
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
    __finitef_0100();
    __finitef_0200();
    __finitef_0300();
    __finitef_0400();
    __finitef_0500();
    __finitef_0600();
    __finitef_0700();

    return t_status;
}