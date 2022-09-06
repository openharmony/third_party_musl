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
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <spawn.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "functionalext.h"

/*
 * @tc.name      : getgroups_0100
 * @tc.desc      : Verify that the group ID can be obtained (parameter valid)
 * @tc.level     : Level 0
 */
void getgroups_0100(void)
{
    char buffer[1024] = {0};
    gid_t list[500];
    system("id -g > id.txt");
    FILE *fptr = fopen("id.txt", "r");
    fread(buffer, sizeof(buffer), 1, fptr);
    int id = atoi(buffer);
    int result = getgroups(id, list);
    EXPECT_EQ("getgroups_0100", result, id);
}

/*
 * @tc.name      : getgroups_0200
 * @tc.desc      : Verify that the group ID cannot be obtained (parameter invalid)
 * @tc.level     : Level 2
 */
void getgroups_0200(void)
{
    char buffer[1024] = {0};
    gid_t list[500];
    system("id -g > id.txt");
    FILE *fptr = fopen("id.txt", "r");
    fread(buffer, sizeof(buffer), 1, fptr);
    int id = atoi(buffer);
    int result = getgroups(-1, list);
    EXPECT_EQ("getgroups_0200", result, -1);
}

int main()
{
    getgroups_0100();
    getgroups_0200();

    return t_status;
}