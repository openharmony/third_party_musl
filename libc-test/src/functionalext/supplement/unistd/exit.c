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
#include <stdbool.h>
#include <stdint.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/*
 * @tc.name      : exit_0100
 * @tc.desc      : Verify that the file content of the auxiliary application output information
 *                 is viewed by calling the exit(0) function.
 * @tc.level     : Level 0
 */
void exit_0100(void)
{
    system("cd /data/tests/libc-test/src/functionalext/supplement/unistd/; ./exittest01");
    char abc[100] = {0};
    bool successflag = false;
    const char *ptr = "/data/Exittest01.txt";
    FILE *fptr = fopen(ptr, "r");
    fseek(fptr, 0, SEEK_SET);
    int32_t rsize = fread(abc, sizeof(abc), 1, fptr);
    if (strcmp(abc, "exit before") == 0) {
        successflag = true;
    }
    if (strcmp(abc, "exit after") == 0) {
        successflag = false;
    }
    EXPECT_TRUE("exit_0100", successflag);
    fclose(fptr);
    remove(ptr);
}

/*
 * @tc.name      : exit_0200
 * @tc.desc      : Verify that the file content of the auxiliary application output information
 *                 is viewed by calling the exit(1) function.
 * @tc.level     : Level 0
 */
void exit_0200(void)
{
    system("cd /data/tests/libc-test/src/functionalext/supplement/unistd/; ./exittest02");
    char abc[100] = {0};
    bool successflag = false;
    const char *ptr = "/data/Exittest02.txt";
    FILE *fptr = fopen(ptr, "r");
    fseek(fptr, 0, SEEK_SET);
    char *content = fgets(abc, 27, fptr);
    if (strcmp(content, "exit before") == 0) {
        successflag = true;
    }
    if (strcmp(content, "exit after") == 0) {
        successflag = false;
    }
    EXPECT_TRUE("exit_0200", successflag);
    fclose(fptr);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    exit_0100,
    exit_0200,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}