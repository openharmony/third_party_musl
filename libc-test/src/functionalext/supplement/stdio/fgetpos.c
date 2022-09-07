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
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t NUM_ZERO = 0;
const int32_t NUN_ELEVEN = 11;

/**
 * @tc.name      : fgetpos_0100
 * @tc.desc      : File pointer at the end of a file, the position to which the current file pointer points
 * @tc.level     : Level 0
 */
void fgetpos_0100()
{
    fpos_t pos;
    int result;
    char buff[] = "hello world";
    FILE *fptr = fopen("test.txt", "w+");
    fputs(buff, fptr);
    fseek(fptr, 0, SEEK_END);
    result = fgetpos(fptr, &pos);
    EXPECT_EQ("fgetpos_0100", result, NUM_ZERO);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : fgetpos_0200
 * @tc.desc      : File pointer at the beginning of a file, the location to which the current file pointer points
 * @tc.level     : Level 0
 */
void fgetpos_0200()
{
    fpos_t pos;
    int result;
    char buff[] = "hello world";
    FILE *fptr = fopen("test.txt", "w+");
    fputs(buff, fptr);
    fseek(fptr, 0, SEEK_SET);
    result = fgetpos(fptr, &pos);
    EXPECT_EQ("fgetpos_0200", result, NUM_ZERO);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : fgetpos_0300
 * @tc.desc      : Invalid argument. Cannot get the location of the current file pointer
 * @tc.level     : Level 2
 */
void fgetpos_0300()
{
    FILE *fptr = fopen("test.txt", "w+");
    remove("test.txt");
    fpos_t pos;
    int result;
    result = fgetpos(fptr, &pos);
    EXPECT_EQ("fgetpos_0300", result, NUM_ZERO);
}

TEST_FUN G_Fun_Array[] = {
    fgetpos_0100,
    fgetpos_0200,
    fgetpos_0300,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}