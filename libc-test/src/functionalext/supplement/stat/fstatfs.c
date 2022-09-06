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
#include <sys/stat.h>
#include <stdint.h>
#include <signal.h>
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : fstatfs_0100
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 0
 */
void fstatfs_0100(void)
{
    struct statfs buff;
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT);
    int result = fstatfs(fd, &buff);
    EXPECT_EQ("fstatfs_0100", result, 0);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : fstatfs_0200
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 2
 */
void fstatfs_0200(void)
{
    struct statfs buff;
    int fd = open("/data/readtest.txt", O_RDWR);
    int result = fstatfs(fd, &buff);
    EXPECT_EQ("fstatfs_0200", result, -1);
}

/**
 * @tc.name      : fstatfs_0300
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 2
 */
void fstatfs_0300(void)
{
    struct statfs buff;
    buff.f_type = -666666;
    buff.f_bsize = -1111111;
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT);
    int result = fstatfs(fd, &buff);
    EXPECT_EQ("fstatfs_0300", result, 0);
    close(fd);
    remove("/data/readtest.txt");
}

int main()
{
    fstatfs_0100();
    fstatfs_0200();
    fstatfs_0300();
    return t_status;
}