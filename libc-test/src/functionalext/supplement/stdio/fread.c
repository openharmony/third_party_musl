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

#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t NUM_ZERO = 0;
const int32_t NUM_TEN = 10;
const int32_t NUM_TWENTY = 20;

/**
 * @tc.name      : fread_0100
 * @tc.desc      : Verify that the number of data read is less than the number of data in the file
 * @tc.level     : Level 0
 */
void fread_0100(void)
{
    char abc[100] = {0};
    const char *wrstring = "starttowritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0100", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);

    size_t rsize = fread(abc, 1, 10, fptr);
    EXPECT_EQ("fread_0100", rsize, 10);
    EXPECT_STREQ("fread_0100", abc, "starttowri");

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0200
 * @tc.desc      : Verify that the number of data read is greater than the number of data in the file
 * @tc.level     : Level 1
 */
void fread_0200(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";

    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);

    size_t rsize = fread(abc, 1, 25, fptr);
    EXPECT_EQ("fread_0200", rsize, 20);
    EXPECT_STREQ("fread_0200", abc, wrstring);

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0300
 * @tc.desc      : Verify that the number of data read is greater than the number of data in the file
 *                 (the file pointer is at the end)
 * @tc.level     : Level 2
 */
void fread_0300(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_END);
    size_t rsize = fread(abc, 1, 10, fptr);
    EXPECT_EQ("fread_0300", rsize, 0);

    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fread_0400
 * @tc.desc      : Verify that the size parameter is invalid and cannot read the specified file content
 * @tc.level     : Level 2
 */
void fread_0400(void)
{
    char abc[100] = {0};
    const char *wrstring = "startwritehelloworld";
    const char *ptr = "/data/Freadtest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("fread_0200", fptr, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_END);

    size_t rsize = fread(abc, 0, 10, fptr);
    EXPECT_EQ("fread_0400", rsize, 0);

    fclose(fptr);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    fread_0100,
    fread_0200,
    fread_0300,
    fread_0400,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}