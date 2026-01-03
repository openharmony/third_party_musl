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

int main(int argc, char *argv[])
{
    __finitef_0100();
    __finitef_0200();
    return t_status;
}