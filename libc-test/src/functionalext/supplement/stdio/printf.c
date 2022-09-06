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
#include <stdint.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : printf_0100
 * @tc.desc      : Verify the output data in the specified format (the format character is d)
 * @tc.level     : Level 0
 */
void printf_0100(void)
{
    int num = 6;
    int count;
    count = printf("num:%d\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0100", flag);
}

/**
 * @tc.name      : printf_0200
 * @tc.desc      : Verify the output data in the specified format (the format character is o)
 * @tc.level     : Level 0
 */
void printf_0200(void)
{
    int num = 6;
    int count;
    count = printf("num:%o\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0200", flag);
}

/**
 * @tc.name      : printf_0300
 * @tc.desc      : Verify the output data in the specified format (the format character is x,X)
 * @tc.level     : Level 0
 */
void printf_0300(void)
{
    int num = 6;
    int count;
    count = printf("num:%x,X\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0300", flag);
}

/**
 * @tc.name      : printf_0400
 * @tc.desc      : Verify the output data in the specified format (the format character is u)
 * @tc.level     : Level 0
 */
void printf_0400(void)
{
    int num = 6;
    int count;
    count = printf("num:%u\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0400", flag);
}

/**
 * @tc.name      : printf_0500
 * @tc.desc      : Verify the output data in the specified format (the format character is f)
 * @tc.level     : Level 0
 */
void printf_0500(void)
{
    int num = 6;
    int count;
    count = printf("num:%f\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0500", flag);
}

/**
 * @tc.name      : printf_0600
 * @tc.desc      : Verify the output data in the specified format (the format character is e,E)
 * @tc.level     : Level 0
 */
void printf_0600(void)
{
    int num = 6;
    int count;
    count = printf("num:%e,E\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0600", flag);
}

/**
 * @tc.name      : printf_0700
 * @tc.desc      : Verify the output data in the specified format (the format character is g,G)
 * @tc.level     : Level 0
 */
void printf_0700(void)
{
    int num = 6;
    int count;
    count = printf("num:%g,G\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0700", flag);
}

/**
 * @tc.name      : printf_0800
 * @tc.desc      : Verify the output data in the specified format (the format character is c)
 * @tc.level     : Level 0
 */
void printf_0800(void)
{
    int num = 6;
    int count;
    count = printf("num:%c\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0800", flag);
}

/**
 * @tc.name      : printf_0900
 * @tc.desc      : Verify the output data in the specified format (the format character is s)
 * @tc.level     : Level 0
 */
void printf_0900(void)
{
    char num[] = "test";
    int count;
    count = printf("num:%s\n", num);
    bool flag = false;
    if (count > COUNT_ZERO) {
        flag = true;
    }
    EXPECT_TRUE("printf_0900", flag);
}

TEST_FUN G_Fun_Array[] = {
    printf_0100,
    printf_0200,
    printf_0300,
    printf_0400,
    printf_0500,
    printf_0600,
    printf_0700,
    printf_0800,
    printf_0900,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}