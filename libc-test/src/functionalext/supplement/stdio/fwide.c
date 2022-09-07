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
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : fwide_0100
 * @tc.desc      : Verify that the set byte stream is a wide character stream (valid, mode > 0)
 * @tc.level     : Level 0
 */
void fwide_0100(void)
{
    FILE *fptr = fopen("test.txt", "w+");
    int result;
    bool flag = false;
    result = fwide(fptr, 5);
    if (result > 0) {
        flag = true;
    }
    EXPECT_TRUE("fwide_0100", flag);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : fwide_0200
 * @tc.desc      : Verify that the set byte stream remains as it is (all parameters are valid, mode equals 0)
 * @tc.level     : Level 0
 */
void fwide_0200(void)
{
    FILE *fptr = fopen("test.txt", "w+");
    int result;
    result = fwide(fptr, 0);
    EXPECT_EQ("fwide_0200", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : fwide_0300
 * @tc.desc      : Verify that the set byte stream is a multi-byte character stream (valid with mode less than 0)
 * @tc.level     : Level 0
 */
void fwide_0300(void)
{
    FILE *fptr = fopen("test.txt", "w+");
    int result;
    bool flag = false;
    result = fwide(fptr, -5);
    if (result < 0) {
        flag = true;
    }
    EXPECT_TRUE("fwide_0300", flag);
    fclose(fptr);
    remove("test.txt");
}

int main()
{

       fwide_0100();
    fwide_0200();
    fwide_0300();
    return t_status;
}