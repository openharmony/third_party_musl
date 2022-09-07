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

#include <errno.h>
#include <ftw.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "functionalext.h"

#define TEST_FD_LIMIT 128
#define TEST_FLAG_SIZE 4
#define TEST_DIGIT_TWO 2

static int nftw_callback(const char *pathname, const struct stat *sb, int flag, struct FTW *ftw)
{
    EXPECT_TRUE("nftw_callback", pathname != NULL);
    EXPECT_TRUE("nftw_callback", sb != NULL);
    if (flag == FTW_NS) {
        struct stat st;
        EXPECT_EQ("nftw_callback", stat(pathname, &st), -1);
        return 0;
    }

    if (S_ISDIR(sb->st_mode)) {
        if (access(pathname, R_OK) == 0) {
            EXPECT_TRUE("nftw_callback", flag == FTW_D || flag == FTW_DP);
        } else {
            EXPECT_EQ("nftw_callback", flag, FTW_DNR);
        }
    } else if (S_ISLNK(sb->st_mode)) {
        EXPECT_EQ("nftw_callback", flag, FTW_SL);
    } else {
        EXPECT_EQ("nftw_callback", flag, FTW_F);
    }
    return 0;
}

/**
 * @tc.name      : nftw_0100
 * @tc.desc      : Traverse directory /data
 * @tc.level     : Level 0
 */
void nftw_0100(void)
{
    int flag[TEST_FLAG_SIZE] = {FTW_PHYS, FTW_MOUNT, FTW_CHDIR, FTW_DEPTH};
    const char *path = "/data";
    int i;
    for (i = 0; i < TEST_FLAG_SIZE; i++) {
        int ret = nftw(path, nftw_callback, TEST_FD_LIMIT, flag[i]);
        EXPECT_EQ("nftw_0100", ret, 0);
    }
}

/**
 * @tc.name      : nftw_0200
 * @tc.desc      : Traverse directory /data, but the maximum number of file descriptors is 0
 * @tc.level     : Level 0
 */
void nftw_0200(void)
{
    const char *path = "/data";
    int ret = nftw(path, nftw_callback, 0, FTW_PHYS);
    EXPECT_EQ("nftw_0200", ret, 0);
}

/**
 * @tc.name      : nftw_0300
 * @tc.desc      : The file path length exceeds PATH_MAX, traverse the directory
 * @tc.level     : Level 2
 */
void nftw_0300(void)
{
    char path[PATH_MAX * TEST_DIGIT_TWO];
    memset(path, 'a', sizeof(path));
    path[PATH_MAX * TEST_DIGIT_TWO - 1] = 0;
    int ret = nftw(path, nftw_callback, TEST_FD_LIMIT, FTW_PHYS);
    EXPECT_EQ("nftw_0300", ret, -1);
    EXPECT_EQ("nftw_0300", errno, ENAMETOOLONG);
}

int main(void)
{
    nftw_0100();
    nftw_0200();
    nftw_0300();
    return t_status;
}