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
const float TEST_FLOAT = 555.5f;
const int TEST_NUM_800 = 800;
const int TEST_NUM_2 = 2;

/**
 * @tc.name      : __isinff_0100
 * @tc.desc      : The parameter is HUGE_VALF, -HUGE_VALF, NAN and num, __isinff can correctly return.
 * @tc.level     : Level 0
 */
void __isinff_0100(void)
{
    EXPECT_EQ("__isinf_0100", __isinff(HUGE_VALF), RETURN_ONE);
    EXPECT_EQ("__isinf_0100", __isinff(-HUGE_VALF), -RETURN_ONE);
    EXPECT_EQ("__isinf_0100", __isinff(NAN), RETURN_ZERO);
    EXPECT_EQ("__isinf_0100", __isinff(TEST_FLOAT), RETURN_ZERO);
}

/**
 * @tc.name      : __isinff_0200
 * @tc.desc      : The parameter is exp(800), FLT_MIN / 2 and 0, __finitef can correctly return.
 * @tc.level     : Level 0
 */
void __isinff_0200(void)
{
    EXPECT_EQ("__isinff_0200", __isinff(exp(TEST_NUM_800)), RETURN_ONE);
    EXPECT_EQ("__isinff_0200", __isinff(FLT_MIN / TEST_NUM_2), RETURN_ZERO);
    EXPECT_EQ("__isinff_0200", __isinff(0), RETURN_ZERO);
}

/**
 * @tc.name      : __isinff_0300
 * @tc.desc      : Test core functions.
 * @tc.level     : Level 0
 */
void __isinff_0300(void)
{
    float pos_inf = 0.0f;
    float neg_inf = 0.0f;
    int ret_val = 0;

    pos_inf = FLT_INFINITY;
    float pos_inf_calc = 1.0f / 0.0f;

    neg_inf = -FLT_INFINITY;
    float neg_inf_calc = -1.0f / 0.0f;

    ret_val = __isinff(pos_inf);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(pos_inf_calc);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(neg_inf);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(neg_inf_calc);
    EXPECT_NE(__FUNCTION__, ret_val, 0);
}

/**
 * @tc.name      : __isinff_0400
 * @tc.desc      : Non-Infinite Determination Test.
 * @tc.level     : Level 0
 */
void __isinff_0400(void)
{
    int ret_val = 0;
    float normal_pos1 = 123.456f;
    float normal_pos2 = 9999.999f;

    ret_val = __isinff(normal_pos1);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(normal_pos2);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float normal_neg1 = -789.012f;
    float normal_neg2 = -8888.888f;

    ret_val = __isinff(normal_neg1);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(normal_neg2);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float zero_pos = 0.0f;
    float zero_neg = -0.0f;

    ret_val = __isinff(zero_pos);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(zero_neg);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float nan_val1 = NAN;
    float nan_val2 = 0.0f / 0.0f;

    ret_val = __isinff(nan_val1);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(nan_val2);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float flt_max = FLT_MAX;
    float flt_min = FLT_MIN;

    ret_val = __isinff(flt_max);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(flt_min);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
}

/**
 * @tc.name      : __isinff_0500
 * @tc.desc      : Boundary and Compatibility Testing.
 * @tc.level     : Level 0
 */
void __isinff_0500(void)
{
    int ret_val = 0;
    float f_val = FLT_INFINITY;
    double d_val = INFINITY;

    ret_val = __isinff(f_val);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff((float)d_val);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float inf_add = FLT_INFINITY + 1000.0f;
    float inf_sub = FLT_INFINITY - 5000.0f;
    float inf_mul = FLT_INFINITY * 2.5f;
    float inf_div = FLT_INFINITY / 3.0f;

    ret_val = __isinff(inf_add);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(inf_sub);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(inf_mul);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    ret_val = __isinff(inf_div);
    EXPECT_NE(__FUNCTION__, ret_val, 0);

    float tiny_val1 = FLT_MIN / 10.0f;
    float tiny_val2 = -FLT_MIN / 5.0f;

    ret_val = __isinff(tiny_val1);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);
    ret_val = __isinff(tiny_val2);
    EXPECT_EQ(__FUNCTION__, ret_val, 0);

    float stable_inf = FLT_INFINITY;
    for (int i = 0; i < 5; i++)
    {
        ret_val = __isinff(stable_inf);
        EXPECT_NE(__FUNCTION__, ret_val, 0);
    }
}

/**
 * @tc.name      : __isinff_0600
 * @tc.desc      : Function Comparison Test.
 * @tc.level     : Level 0
 */
void __isinff_0600(void)
{
    float test_vals[] = {
        FLT_INFINITY,
        -FLT_INFINITY,
        123.456f,
        -789.012f,
        0.0f,
        -0.0f,
        NAN,
        FLT_MAX,
        FLT_MIN
    };
    int val_count = sizeof(test_vals) / sizeof(float);
    int retff = 0;
    int retf = 0;
    int match_flag = 1;

    for (int i = 0; i < val_count; i++)
    {
        retff = __isinff(test_vals[i]);
        retf = isinf(test_vals[i]);
        if ((retff != 0 && retf == 0) || (retff == 0 && retf != 0)) {
            match_flag = 0;
        }
    }

    EXPECT_TRUE(__FUNCTION__, match_flag);
}

void* mt_worker(void* arg)
{
    __isinff_0100();
    return NULL;
}

/**
 * @tc.name      : __isinff_0700
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __isinff_0700(void)
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
    __isinff_0100();
    __isinff_0200();
    __isinff_0300();
    __isinff_0400();
    __isinff_0500();
    __isinff_0600();
    __isinff_0700();

    return t_status;
}