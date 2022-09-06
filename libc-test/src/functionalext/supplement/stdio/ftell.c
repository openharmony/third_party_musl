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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int COUNT_ZERO = 0;
const int COUNT_ZE = -1;
const int COUNT_EIGHT = 8;
const int COUNT_TWENTYTWO = 22;
#define ERROR_LEN 8

/**
 * @tc.name      : ftell_0100
 * @tc.desc      : Verify that the file pointer is at the beginning (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0100(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    unsigned long filesize = -1;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0L, SEEK_SET);
    filesize = ftell(fptr);
    EXPECT_EQ("ftell_0100", filesize, COUNT_ZERO);
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : ftell_0200
 * @tc.desc      : Verify that the file pointer is in the middle (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0200(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    unsigned long filesize = -1;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 8L, SEEK_SET);
    filesize = ftell(fptr);
    EXPECT_EQ("ftell_0200", filesize, COUNT_EIGHT);
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : ftell_0300
 * @tc.desc      : Verify that the file pointer is at the end (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0300(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    unsigned long filesize = -1;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0L, SEEK_END);
    filesize = ftell(fptr);
    EXPECT_EQ("ftell_0300", filesize, COUNT_TWENTYTWO);
    fclose(fptr);
    remove(ptr);
}


TEST_FUN G_Fun_Array[] = {
    ftell_0100,
    ftell_0200,
    ftell_0300,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}