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

#include <string.h>
#include "functionalext.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @tc.name       : fgetln_0100
 * @tc.desc       : Verify that a pointer to the next row can be obtained.
 * @tc.level      : level 0
 */
void fgetln_0100(void)
{

    char *p = NULL;
    size_t len;
    FILE *fptr = fopen("test.txt", "w+");
    fputs("this is test", fptr);
    fprintf(fptr, "\n");
    fputs("that is test.txt", fptr);
    fseek(fptr, 0, SEEK_SET);
    bool flag = false;
    p = fgetln(fptr, &len);
    if (p != NULL) {
        flag = true;
    }
    EXPECT_TRUE("fgetln_0100", flag);
    fclose(fptr);
    remove("test.txt");
}

/**
 * @tc.name       : fgetln_0200
 * @tc.desc       : Validation cannot get pointer to next row (f argument invalid)
 * @tc.level      : level 2
 */
void fgetln_0200(void)
{
    char *p = NULL;
    size_t len;
    FILE *fptr = fopen("test.txt", "w");
    fseek(fptr, 0, SEEK_SET);
    bool flag = false;
    p = fgetln(fptr, &len);
    if (p == NULL) {
        flag = true;
    }
    EXPECT_TRUE("fgetln_0200", flag);
    fclose(fptr);
}

int main(int argc, char *argv[])
{
    fgetln_0100();
    fgetln_0200();
    return t_status;
}