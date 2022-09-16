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
#include <stdint.h>
#include <signal.h>
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;

/**
 * @tc.name      : fstatvfs_0100
 * @tc.desc      : Verify that file system statistics can be obtained (all parameters are valid)
 * @tc.level     : Level 0
 */
void fstatvfs_0100(void)
{
    struct statvfs buff;
    int result;
    int fd = open("test.txt", O_RDWR | O_CREAT);
    result = fstatvfs(fd, &buff);
    EXPECT_EQ("fstatvfs_0100", result, COUNT_ZERO);
    close(fd);
    remove("test.txt");
}

/**
 * @tc.name      : fstatvfs_0200
 * @tc.desc      : Verify that file system statistics cannot be obtained (fd argument is invalid)
 * @tc.level     : Level 2
 */
void fstatvfs_0200(void)
{
    struct statvfs buff;
    int result;
    int fd = open("test.txt", O_RDWR | O_CREAT);
    result = fstatvfs(-1, &buff);
    EXPECT_EQ("fstatvfs_0200", result, COUNT_NEGATIVE);
    close(fd);
    remove("test.txt");
}

int main(int argc, char *argv[])
{
    fstatvfs_0100();
    fstatvfs_0200();
    return t_status;
}