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
#include <unistd.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : fsetxattr_0100
 * @tc.desc      : Verify that you can set the extended properties of the specified file (all parameters are valid)
 * @tc.level     : Level 0
 */
void fsetxattr_0100(void)
{
    int fd = open("/data/test.txt", O_CREAT | O_WRONLY, 0667);
    char name[] = "user.x";
    char value[] = "the past is not dead.";
    int result;
    result = fsetxattr(fd, name, value, strlen(value), 0);
    EXPECT_EQ("fsetxattr_0100", result, COUNT_ZERO);
    close(fd);
    remove("/data/test.txt");
}

int main()
{
    fsetxattr_0100();
    return t_status;
}