/*
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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/xattr.h>
#include <stdbool.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : flistxattr_0100
 * @tc.desc      : Verify that extended attribute names can be listed (each parameter is valid)
 * @tc.level     : Level 0
 */
void flistxattr_0100(void)
{
    bool flag = false;
    int fd = open("/data/test.txt", O_CREAT | O_WRONLY, 0667);
    char name[] = "user.x";
    char value[] = "the past is not dead.";
    ssize_t result;
    fsetxattr(fd, name, value, strlen(value), 0);
    result = flistxattr(fd, name, 0);
    if (result >= 0)
    {
        flag = true;
    }
    EXPECT_TRUE("flistxattr_0100", flag);
    close(fd);
    remove("/data/test.txt");
}

TEST_FUN G_Fun_Array[] = {
    flistxattr_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos)
    {
        G_Fun_Array[pos]();
    }
    return t_status;
}