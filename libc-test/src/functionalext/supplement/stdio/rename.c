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

#include "functionalext.h"

/**
 * @tc.name      : rename_0100
 * @tc.desc      : Each parameter is valid, and the file name can be changed.
 * @tc.level     : Level 0
 */
void rename_0100(void)
{
    const char *ptr = "oldfile.txt";
    const char *newptr = "newfile.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_TRUE("rename_0100", fptr != NULL);
    int ret = rename(ptr, newptr);
    EXPECT_EQ("rename_0100", ret, 0);
    fclose(fptr);
    remove(ptr);
    remove(newptr);
    fptr = NULL;
    ptr = NULL;
    newptr = NULL;
}

/**
 * @tc.name      : rename_0200
 * @tc.desc      : Parameter 1 is invalid, the file name cannot be changed.
 * @tc.level     : Level 2
 */
void rename_0200(void)
{
    const char *ptr = "test.txt";
    const char *newptr = "newfile.txt";
    int ret = rename(ptr, newptr);
    EXPECT_EQ("rename_0200", ret, -1);
    ptr = NULL;
    newptr = NULL;
}

/**
 * @tc.name      : rename_0300
 * @tc.desc      : Parameter 2 is invalid, the file name cannot be changed.
 * @tc.level     : Level 2
 */
void rename_0300(void)
{
    const char *ptr = "oldfile.txt";
    const char *newptr = "newfile.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_TRUE("rename_0300", fptr != NULL);
    FILE *newfptr = fopen(newptr, "w");
    EXPECT_TRUE("rename_0300", fptr != NULL);
    int ret = rename(ptr, newptr);
    EXPECT_EQ("rename_0300", ret, 0);
    fclose(fptr);
    fclose(newfptr);
    remove(ptr);
    remove(newptr);
    fptr = NULL;
    ptr = NULL;
    newptr = NULL;
}

int main()
{
    rename_0100();
    rename_0200();
    rename_0300();
    return t_status;
}