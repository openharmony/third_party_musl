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

#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functionalext.h"
#include "sys/xattr.h"

/**
 * @tc.name      : fgetxattr_0100
 * @tc.desc      : Verify that the extended attribute value can be obtained.
 * @tc.level     : Level 0
 */
void fgetxattr_0100(void)
{
    int fd = open("fgetxattr.txt", O_RDWR | O_CREAT);
    char name[] = "user.x";
    char value[] = "the past is not dead.";
    ssize_t size;
    fsetxattr(fd, name, value, strlen(value), 0);
    size = fgetxattr(fd, name, value, 1024);
    bool flag = false;
    if (size >= 0)
    {
        flag = true;
    }
    EXPECT_TRUE("fgetxattr_0100", flag);
    close(fd);
    remove("fgetxattr.txt");
}

int main(int argc, char *argv[])
{
    fgetxattr_0100();
    return t_status;
}
