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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;
const int32_t COUNT_NEGATIVE = -1;
const char *path = "/data/file.txt";

/**
 * @tc.name      : futimesat_0100
 * @tc.desc      : Verify that you can set the file access time and modify time (valid parameters, times is not 0)
 * @tc.level     : Level 0
 */
void futimesat_0100(void)
{
    int result = 10;
    int fd = open(path, O_RDWR | O_CREAT);
    result = futimesat(fd, path, NULL);
    EXPECT_EQ("futimesat_0100", result, COUNT_ZERO);
    close(fd);
    remove(path);
}

/**
 * @tc.name      : futimesat_0200
 * @tc.desc      : Verify that you can set the file access time and modify time (valid parameters, times is 0)
 * @tc.level     : Level 1
 */
void futimesat_0200(void)
{
    int result = 10;
    int fd = open(path, O_RDWR | O_CREAT);
    result = futimesat(fd, path, 0);
    EXPECT_EQ("futimesat_0200", result, COUNT_ZERO);
    close(fd);
    remove(path);
}

/**
 * @tc.name      : futimesat_0300
 * @tc.desc      : Verify that file access time and modification time cannot be set (the dirfd parameter is invalid)
 * @tc.level     : Level 2
 */
void futimesat_0300(void)
{
    int result = 10;
    int fd = open(path, O_RDWR | O_CREAT);
    close(fd);
    remove(path);
    result = futimesat(fd, path, NULL);
    EXPECT_EQ("futimesat_0300", result, COUNT_NEGATIVE);
}

/**
 * @tc.name      : futimesat_0400
 * @tc.desc      : Validation cannot set file access time and modify time (pathname parameter invalid)
 * @tc.level     : Level 2
 */
void futimesat_0400(void)
{
    int result = 10;
    int fd = open(path, O_RDWR | O_CREAT);
    result = futimesat(fd, "", NULL);
    EXPECT_EQ("futimesat_0400", result, COUNT_NEGATIVE);
    close(fd);
    remove(path);
}

int main()
{
    futimesat_0100();
    futimesat_0200();
    futimesat_0300();
    futimesat_0400();
    return t_status;
}