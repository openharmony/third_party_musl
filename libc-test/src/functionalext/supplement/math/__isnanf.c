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

#include "functionalext.h"

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

int main(int argc, char *argv[])
{
    __isnanf_0100();
    return t_status;
}