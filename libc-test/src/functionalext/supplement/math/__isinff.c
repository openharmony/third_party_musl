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
#include "functionalext.h"

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

int main(int argc, char *argv[])
{
    __isinff_0100();
    __isinff_0200();
    return t_status;
}