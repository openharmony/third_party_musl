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
#include <dirent.h>
#include <string.h>
#include "test.h"
#include <sys/stat.h>
#include <stdio.h>
/*
 * @tc.name      : opendir_0100
 * @tc.desc      : Verify that the parameters are valid to open the specified directory
 * @tc.level     : Level 0
 */
void opendir_0100(void)
{
    errno = 0;
    char *path = "/data/local/tmp/opendirtest";
    if (access(path, F_OK) == 0) {
        remove(path);
    }
    int ret = mkdir(path, 0777);
    if (ret != 0) {
        t_error("opendirtest create failed: errno = %d\n", errno);
    }
    DIR *dir = opendir(path);
    if (!dir) {
        t_error("%s opendir failed\n", __func__);
    }
    struct dirent *r;
    int issuccess = 0;
    while ((r = readdir(dir)) != NULL) {
        if (strcmp(r->d_name, ".") == 0) {
            issuccess = 1;
            break;
        }
    }
    if (issuccess != 1) {
        t_error("%s opendir is not right", __func__);
    }
    closedir(dir);
    ret = remove(path);
    if (ret != 0) {
        t_error("remove failed: errno = %d\n", errno);
    }
}

/*
 * @tc.name      : opendir_0200
 * @tc.desc      : When the path is not a folder or not exit, test the return value of the function
 * @tc.level     : Level 2
 */
void opendir_0200(void)
{
    DIR *dir1 = opendir("/dev/null");
    if (dir1) {
        t_error("%s dir should be NULL\n", __func__);
    }

    errno = 0;
    DIR *dir2 = opendir("/does/not/exit/folder");
    if (dir2) {
        t_error("%s dir should be NULL\n", __func__);
    }
    if (errno != ENOENT) {
        t_error("%s errno should be ENOTDIR, but now is %d\n", __func__, errno);
    }
}

/*
 * @tc.name      : opendir_0300
 * @tc.desc      : When the path is NULL, test the return value of the function
 * @tc.level     : Level 2
 */
void opendir_0300(void)
{
    DIR *dir = opendir(NULL);
    if (dir) {
        t_error("%s dir should be NULL\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    opendir_0100();
    opendir_0200();
    opendir_0300();

    return t_status;
}