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

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include "test.h"
#include "functionalext.h"

struct __dirstream {
    long long tell;
    int fd;
    int buf_pos;
    int buf_end;
    volatile int lock[1];
    char buf[2048];
};
#define MAXPATH 1000
const int COUNT_NEGATIVE = -1;

/*
 * @tc.name      : dirfd_0100
 * @tc.desc      : Verify that the file descriptor for the directory file referred to by parameter d is available
 *                 (parameter valid)
 * @tc.level     : Level 0
 */
void dirfd_0100(void)
{
    bool flag = true;
    char path[MAXPATH];
    DIR *dir_ptr;
    getcwd(path, MAXPATH);
    dir_ptr = opendir(path);
    int result = 0;
    result = dirfd(dir_ptr);
    if (result > 0) {
        flag = true;
    }
    EXPECT_TRUE("dirfd_0100", flag);
    closedir(dir_ptr);
}

int main()
{
    dirfd_0100();
    return t_status;
}