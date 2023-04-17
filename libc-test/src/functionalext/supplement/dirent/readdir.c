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
#include "functionalext.h"

/**
 * @tc.name      : readdir_0100
 * @tc.desc      : The parameter is valid and can return the next directory entry
 *                 point of the parameter dir directory stream.
 * @tc.level     : Level 0
 */
void readdir_0100(void)
{
    DIR *dir;
    struct dirent *ret;
    dir = opendir("/etc");
    ret = readdir(dir);
    EXPECT_TRUE("readdir_0100", ret != NULL);
    closedir(dir);
}

/**
 * @tc.name      : readdir_0200
 * @tc.desc      : The parameter is invalid, cannot return the next directory entry
 *                 point of the parameter dir directory stream.
 * @tc.level     : Level 2
 */
void readdir_0200(void)
{
    DIR *dir = (DIR *)"";
    struct dirent *ret;
    ret = readdir(dir);
    EXPECT_TRUE(" readdir_0200", NULL == ret);
}

int main(int argc, char *argv[])
{
    readdir_0100();
    readdir_0200();

    return t_status;
}