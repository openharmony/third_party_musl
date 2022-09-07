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
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : fwrite_0100
 * @tc.desc      : File pointer at the end of a file, the position to which the current file pointer points
 * @tc.level     : Level 0
 */
void fwrite_0100(void)
{
    bool flag = false;
    FILE *fptr = fopen("test.txt", "w+");
    int result = 0;
    char buf[] = "this is test";
    result = fwrite(buf, sizeof(char), strlen(buf), fptr);
    if (result > 0) {
        flag = true;
    }
    fclose(fptr);
    remove("test.txt");
    EXPECT_TRUE("fwrite_0100", flag);
}

/**
 * @tc.name      : fwrite_0200
 * @tc.desc      : File pointer at the beginning of a file, the location to which the current file pointer points
 * @tc.level     : Level 0
 */
void fwrite_0200(void)
{
    bool flag = false;
    FILE *fptr = fopen("test.txt", "w+");
    int result = 0;
    char buf[] = "this is test";
    result = fwrite(buf, 0, strlen(buf), fptr);
    if (result > 0) {
        flag = true;
    }
    fclose(fptr);
    remove("test.txt");
    EXPECT_EQ("fwrite_0200", result, COUNT_ZERO);
}

/**
 * @tc.name      : fwrite_0300
 * @tc.desc      : Invalid argument. Cannot get the location of the current file pointer
 * @tc.level     : Level 2
 */
void fwrite_0300(void)
{
    bool flag = false;
    FILE *fptr = fopen("test.txt", "w+");
    int result = 0;
    char buf[] = "this is test";
    result = fwrite(buf, sizeof(char), 0, fptr);
    if (result > 0) {
        flag = true;
    }
    fclose(fptr);
    remove("test.txt");
    EXPECT_EQ("fwrite_0300", result, COUNT_ZERO);
}

int main()
{
        fwrite_0100();
    fwrite_0200();
    fwrite_0300();
    return t_status;
}