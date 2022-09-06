/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include <string.h>
#include <stdio.h>
#include "functionalext.h"
const int SUCCESS = 0;

/**
 * @tc.name       : fputs_0100
 * @tc.desc       : Verify that an empty string is written in the specified file.
 * @tc.level      : level 0.
 */
void fputs_0100(void)
{
    char str[80] = "string\n";
    FILE *fptr = NULL;
    fptr = fopen("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt", "w+");
    if (fptr == NULL) {
        perror("fopen");
    }
    int len = fputs(str, fptr);
    EXPECT_EQ("fputs_0100", len, SUCCESS);
    fclose(fptr);
    remove("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt");
    fptr = NULL;
}

/**
 * @tc.name       : fputs_0200
 * @tc.desc       : Verify that an empty string is written in the specified file （parameter is empty）.
 * @tc.level      : level 0.
 */
void fputs_0200(void)
{
    FILE *fptr = NULL;
    fptr = fopen("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt", "w+");
    if (fptr == NULL) {
        perror("fopen");
    }
    int len = fputs("", fptr);
    EXPECT_EQ("fputs_02 00", len, SUCCESS);
    fclose(fptr);
    remove("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt");
    fptr = NULL;
}

/**
 * @tc.name       : fputs_0300
 * @tc.desc       : Verify that an empty string cannot be written to the specified file.
 * @tc.level      : level 2.
 */
void fputs_0300(void)
{
    FILE *fptr = NULL;
    FILE *p = fopen("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt", "w");
    fclose(p);
    fptr = fopen("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt", "r");
    int rsize = fputs("this is a test string", fptr);
    EXPECT_EQ("fputs_0300", rsize, EOF);
    remove("/data/tests/libc-test/src/functionalext/supplement/stdio/fputs.txt");
    fptr = NULL;
}

int main()
{
    fputs_0100();
    fputs_0200();
    fputs_0300();
    return t_status;
}
