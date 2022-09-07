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
 * @tc.name      : getc_0100
 * @tc.desc      : Verify that the characters in the file can be get
 * @tc.level     : Level 0
 */
void getc_0100(void)
{
    const char *wrstring = "This is a test sample!";
    const char *ptr = "test.txt";
    char ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);
    ch = fgetc(fptr);
    EXPECT_EQ("getc_0100", ch, 'T');
    fclose(fptr);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    getc_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}