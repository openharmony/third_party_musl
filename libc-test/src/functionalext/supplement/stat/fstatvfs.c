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
#include <sys/statvfs.h>
#include "test.h"

/**
 * @tc.name      : fstatvfs_0100
 * @tc.desc      : Returns the same information about an open file referenced by descriptor fd
 * @tc.level     : Level 0
 */
void fstatvfs_0100(void)
{
    struct statvfs sts;
    int fd = open("/proc", O_RDONLY);
    if (fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    int result = fstatvfs(fd, &sts);
    if (result != 0) {
        t_error("%s fstatvfs failed\n", __func__);
    }

    close(fd);

    if (sts.f_bsize != 4096U) {
        t_error("%s f_bsize invalid\n", __func__);
    }
    if (sts.f_bfree != 0U) {
        t_error("%s f_bfree invalid\n", __func__);
    }
    if (sts.f_ffree != 0U) {
        t_error("%s f_ffree invalid\n", __func__);
    }
    if (sts.f_fsid != 0U) {
        t_error("%s f_fsid invalid\n", __func__);
    }
    if (sts.f_namemax != 255U) {
        t_error("%s f_namemax invalid\n", __func__);
    }
}

/**
 * @tc.name      : fstatvfs_0200
 * @tc.desc      : Test case when parameter is invalid
 * @tc.level     : Level 2
 */
void fstatvfs_0200(void)
{
    struct statvfs sts;
    int result = fstatvfs(-1, &sts);
    if (result != -1) {
        t_error("%s fstatvfs should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    fstatvfs_0100();
    fstatvfs_0200();
    return t_status;
}