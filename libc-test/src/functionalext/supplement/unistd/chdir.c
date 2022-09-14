/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include "functionalext.h"

/*
 * @tc.name      : chdir_0100
 * @tc.desc      : Verify that the directory can be switched (parameters are valid)
 * @tc.level     : Level 0
 */
void chdir_0100(void)
{
    int num;
    if (NULL == opendir("/data/dir")) {
        mkdir("/data/dir", 0777);
    }
    num = chdir("/data/dir");
    EXPECT_EQ("chdir_0100", num, 0);
}

int main(int argc, char *argv[])
{
    chdir_0100();

    return t_status;
}