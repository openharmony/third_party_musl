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
#include <stdio_ext.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : __fseterr_0100
 * @tc.desc      : Validate errors that can specify streams (parameter valid)
 * @tc.level     : Level 0
 */
void __fseterr_0100(void)
{
    bool flag = false;
    int result1, result2;
    FILE *fptr = fopen("test.txt", "w+");
    result1 = ferror(fptr);
    __fseterr(fptr);
    result2 = ferror(fptr);
    EXPECT_EQ("__fseterr_0100", result1, COUNT_ZERO);
    if (result2 != 0) {
        flag = true;
    }
    EXPECT_TRUE("__fseterr_0100", flag);
    fclose(fptr);
    remove("test.txt");
}

int main(int argc, char *argv[])
{
    __fseterr_0100();
    return t_status;
}