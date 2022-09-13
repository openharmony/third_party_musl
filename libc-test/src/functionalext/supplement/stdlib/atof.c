/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const double COUNT_SUCCESS = 123456.00;

/**
 * @tc.name      : atof_0100
 * @tc.desc      : Verify that the parameter is valid convert the string to an integer
 * @tc.level     : Level 0
 */
void atof_0100()
{
    bool flag = false;
    const char *buff = "123456.00";
    double result = atof(buff);
    if (result == COUNT_SUCCESS) {
        flag = true;
    }
    EXPECT_TRUE("atof_0100", flag);
}

TEST_FUN G_Fun_Array[] = {
    atof_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}