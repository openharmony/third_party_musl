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

#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "functionalext.h"

#define LINE_SIZE 100

/**
 * @tc.name      : open_by_handle_at_0100
 * @tc.desc      : Verify that the open_by_handle_at function was successfully called (all parameters are valid)
 * @tc.level     : Level 0
 */
void open_by_handle_at_0100(void)
{
    int result;
    struct file_handle *fhp;
    int mount_id, fhsize, flags, dirfd;
    char *pathname;
    pathname = "/data";
    fhsize = sizeof(*fhp);
    fhp = malloc(fhsize);
    dirfd = AT_FDCWD;
    flags = 0;
    fhp->handle_bytes = 0;
    name_to_handle_at(dirfd, pathname, fhp, &mount_id, flags);
    fhsize = sizeof(struct file_handle) + fhp->handle_bytes;
    fhp = realloc(fhp, fhsize);
    name_to_handle_at(dirfd, pathname, fhp, &mount_id, flags);
    result = open_by_handle_at(AT_FDCWD, fhp, O_NOFOLLOW);
    bool flag = false;
    if (result > 0) {
        flag = true;
    }
    EXPECT_TRUE("open_by_handle_at_0100", flag);
}

int main(int argc, char *argv[])
{
    // open_by_handle_at_0100();
    return t_status;
}
