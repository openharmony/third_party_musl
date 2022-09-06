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
#include <stdio_ext.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : __flbf_0100
 * @tc.desc      : Verify file stream is row buffered (valid argument, is row buffered)
 * @tc.level     : Level 0
 */
void __flbf_0100(void)
{
    char buf[1024];
    int result = 0;
    bool flag = false;
    FILE *fptr = fopen("test.txt", "w");
    setvbuf(fptr, buf, _IOLBF, 1024);
    result = __flbf(fptr);
    if (result > 0)
    {
        flag = true;
    }
    EXPECT_TRUE("__flbf_0100", flag);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : __flbf_0200
 * @tc.desc      : Verify file stream is not row buffered (valid argument, is full buffered)
 * @tc.level     : Level 1
 */
void __flbf_0200(void)
{
    char buf[1024];
    int result = 0;
    FILE *fptr = fopen("test.txt", "w");
    setvbuf(fptr, buf, _IOFBF, 1024);
    result = __flbf(fptr);
    EXPECT_EQ("__flbf_0200", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name      : __flbf_0300
 * @tc.desc      :Verify file stream is not row buffered (parameter valid, is not buffered)
 * @tc.level     : Level 1
 */
void __flbf_0300(void)
{
    char buf[1024];
    int result = 0;
    FILE *fptr = fopen("test.txt", "w");
    setvbuf(fptr, buf, _IONBF, 1024);
    result = __flbf(fptr);
    EXPECT_EQ("__flbf_0300", result, COUNT_ZERO);
    fclose(fptr);
    remove("test.txt");
}

int main()
{
    __flbf_0100();
    __flbf_0200();
    __flbf_0300();
    return t_status;
}