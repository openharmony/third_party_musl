/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
#define MAXPATH 1000

/*
 * @tc.name      : opendir_0100
 * @tc.desc      : Verify that the parameters are valid to open the specified directory
 * @tc.level     : Level 0
 */
void opendir_0100(void)
{
    bool flag = false;
    char path[MAXPATH];
    DIR *dir_ptr;
    char *data = NULL;
    data = getcwd(path, MAXPATH);
    dir_ptr = opendir(path);
    if (dir_ptr != 0) {
        flag = true;
    }
    EXPECT_TRUE("opendir_0100", flag);
}

/*
 * @tc.name      : opendir_0200
 * @tc.desc      : The verification parameter is invalid, the specified directory cannot be opened
 *                 (name does not exist)
 * @tc.level     : Level 2
 */
void opendir_0200(void)
{
    DIR *dir_ptr;
    dir_ptr = opendir("/dev/key");
    EXPECT_EQ("opendir_0200", dir_ptr, 0);
}

/*
 * @tc.name      : opendir_0300
 * @tc.desc      : The validation parameter is invalid and the specified directory cannot be opened (name is NULL)
 * @tc.level     : Level 2
 */
void opendir_0300(void)
{
    DIR *dir_ptr;
    char *path = NULL;
    path = getcwd(NULL, 0);
    dir_ptr = opendir(NULL);
    EXPECT_EQ("opendir_0300", dir_ptr, 0);
}

TEST_FUN G_Fun_Array[] = {
    opendir_0100,
    opendir_0200,
    opendir_0300,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}