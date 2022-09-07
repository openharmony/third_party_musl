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
#include <errno.h>
#include <wchar.h>
#include <errno.h>
#include "functionalext.h"

/**
 * @tc.name      : fgetws_0100
 * @tc.desc      : Verify that a string of wide characters can be read from the specified file
 * @tc.level     : Level 0
 */
void fgetws_0100()
{
    bool flag = false;
    wchar_t wrstring[100];
    wchar_t line[] = L"hello\n";
    const char *ptr = "test.txt";
    wchar_t *ch;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(line, sizeof(char), 100, fptr);
    fseek(fptr, 0, SEEK_SET);
    rewind(fptr);
    ch = fgetws(wrstring, 100, fptr);
    if (ch > 0)
    {
        flag = true;
    }
    EXPECT_TRUE("fgetws_0100", flag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fgetws_0200
 * @tc.desc      : Verify that a string of wide characters cannot be read from the specified file
 * @tc.level     : Level 2
 */
void fgetws_0200()
{
    bool flag = false;
    wchar_t wrstring[100];
    wchar_t line[] = L"hello\n";
    const char *ptr = "test.txt";
    wchar_t *ch = NULL;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(line, sizeof(char), 100, fptr);
    fflush(fptr);
    ch = fgetws(wrstring, EINVAL, fptr);
    EXPECT_TRUE("fgetws_0200", ch <= 0);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

int main()
{
    fgetws_0100();
    fgetws_0200();
    return t_status;
}