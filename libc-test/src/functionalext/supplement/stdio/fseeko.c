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

/**
 * @tc.name      : fseeko_0100
 * @tc.desc      : Verify that the file pointer is moved to the beginning of the file
 * @tc.level     : Level 0
 */
void fseeko_0100(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t data = fseek(fptr, 0L, SEEK_SET);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0100", data, 0);
    EXPECT_EQ("fseeko_0100", ch, 'T');
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fseeko_0200
 * @tc.desc      : Verify that the file pointer is moved to any position in the file
 * @tc.level     : Level 0
 */
void fseeko_0200(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t data = fseek(fptr, 8L, SEEK_SET);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0200", data, 0);
    EXPECT_EQ("fseeko_0200", ch, 'a');
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fseeko_0300
 * @tc.desc      : Verify that the file pointer is moved to the current position of the file
 * @tc.level     : Level 0
 */
void fseeko_0300(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t code = fseek(fptr, 10L, SEEK_SET);
    int32_t data = fseek(fptr, 0L, SEEK_CUR);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0300", data, 0);
    EXPECT_EQ("fseeko_0300", ch, 't');
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fseeko_0400
 * @tc.desc      : Verify that the file pointer is moved to the end of the file
 * @tc.level     : Level 0
 */
void fseeko_0400(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t data = fseek(fptr, -1L, SEEK_END);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0400", data, 0);
    EXPECT_EQ("fseeko_0400", ch, '!');
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fseeko_0500
 * @tc.desc      : Verify that the moved file pointer position exceeds the starting position pointer movement failed
 * @tc.level     : Level 2
 */
void fseeko_0500(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t data = fseek(fptr, -10L, SEEK_SET);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0500", data, -1);
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : fseeko_0600
 * @tc.desc      : Verify that the moved file pointer position exceeds the end position pointer movement failed
 * @tc.level     : Level 1
 */
void fseeko_0600(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int32_t data = fseek(fptr, 10L, SEEK_END);
    ch = fgetc(fptr);
    EXPECT_EQ("fseeko_0600", data, 0);
    fclose(fptr);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    fseeko_0100,
    fseeko_0200,
    fseeko_0300,
    fseeko_0400,
    fseeko_0500,
    fseeko_0600,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}