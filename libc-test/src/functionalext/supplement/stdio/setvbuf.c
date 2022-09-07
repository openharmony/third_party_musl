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
const int32_t COUNT_ZERO = 0;
const int32_t COUNT_failed = -1;

/**
 * @tc.name      : setvbuf_0100
 * @tc.desc      : Authentication is successful (call the setvbuf function to see the return value)
 * @tc.level     : Level 0
 */
void setvbuf_0100(void)
{
    char buff[1024];
    const char *ptr = "test.txt";
    int result;
    FILE *fptr = fopen(ptr, "w+");
    result = setvbuf(fptr, buff, _IOFBF, 1024);
    EXPECT_EQ("setvbuf_0100", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : setvbuf_0200
 * @tc.desc      : Authentication is fails (call the setvbuf function to see the return value)
 * @tc.level     : Level 2
 */
void setvbuf_0200(void)
{
    char buff[1024];
    const char *ptr = "test.txt";
    int result;
    FILE *fptr = fopen(ptr, "w+");
    result = setvbuf(fptr, buff, -1, 1024);
    EXPECT_EQ("setvbuf_0200", result, COUNT_failed);
    fclose(fptr);
    remove("test.txt");
}

TEST_FUN G_Fun_Array[] = {
    setvbuf_0100,
    setvbuf_0200,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}