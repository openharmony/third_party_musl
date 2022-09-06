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

const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : fwprintf_0100
 * @tc.desc      : Verify the number of wide characters returned by the function (if the file parameter is stdout)
 * @tc.level     : Level 0
 */
void fwprintf_0100(void)
{
    int result;
    result = fwprintf(stdout, L"This is a test!!QAZ@WSX12");
    printf("\n");
    int ret;
    ret = strlen("This is a test!!QAZ@WSX12");
    EXPECT_EQ("fwprintf_0100", result, ret);
}

/**
 * @tc.name      : fwprintf_0200
 * @tc.desc      : Verify the number of wide characters returned by the function (if the file parameter is stderr)
 * @tc.level     : Level 0
 */
void fwprintf_0200(void)
{
    int result;
    result = fwprintf(stderr, L"This is a test!");
    printf("\n");
    int ret;
    ret = strlen("This is a test!");
    EXPECT_EQ("fwprintf_0200", result, ret);
}

/**
 * @tc.name      : fwprintf_0300
 * @tc.desc      : Verifies the number of wide characters returned by the function
                  (if the file is successfully opened in W mode and its return value is used as the file parameter)
 * @tc.level     : Level 0
 */
void fwprintf_0300(void)
{
    int result;
    FILE *fptr = fopen("test.txt", "w");
    result = fwprintf(fptr, L"This is a test!");
    int ret;
    ret = strlen("This is a test!");
    fclose(fptr);
    remove("test.txt");
    EXPECT_EQ("fwprintf_0300", result, ret);
}

/**
 * @tc.name      : fwprintf_0400
 * @tc.desc      : Verifies the number of wide characters returned by the function
                  (if the file is successfully opened in R mode and its return value is used as a file parameter)
 * @tc.level     : Level 2
 */
void fwprintf_0400(void)
{
    int result;
    FILE *fptr1 = fopen("test.txt", "w");
    fclose(fptr1);
    FILE *fptr2 = fopen("test.txt", "r");
    result = fwprintf(fptr2, L"This is a test!");
    fclose(fptr2);
    remove("test.txt");
    EXPECT_EQ("fwprintf_0400", result, COUNT_NEGATIVE);
}

int main()
{
    fwprintf_0100();
    fwprintf_0200();
    fwprintf_0300();
    fwprintf_0400();
    return t_status;
}