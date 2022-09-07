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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "functionalext.h"

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : name_to_handle_at_0100
 * @tc.desc      : Verify that the name_TO_HANDLE_AT function was successfully called (all parameters are valid)
 * @tc.level     : Level 0
 */
void name_to_handle_at_0100(void)
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
    result = name_to_handle_at(dirfd, pathname, fhp, &mount_id, flags);
    EXPECT_EQ("name_to_handle_at_0100", result, COUNT_ZERO);
}

int main()
{
    // name_to_handle_at_0100();
    return t_status;
}
