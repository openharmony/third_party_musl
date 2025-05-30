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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : drand48_0100
 * @tc.desc      : The function call is successful and returns a double random number between [0.0, 1.0).
 * @tc.level     : Level 0
 */
void drand48_0100(void)
{
    srand48(1);
    double ret = drand48();
    EXPECT_TRUE("drand48_0100", ret >= 0.0 && ret < 1.0);
}

int main(int argc, char *argv[])
{
    drand48_0100();
    return t_status;
}