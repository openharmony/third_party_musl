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

#include <unistd.h>
#include <grp.h>
#include <limits.h>
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
#include <stdio.h>

/*
 * @tc.name      : getgrouplist_0100
 * @tc.desc      : Verify that the list of group IDs can be obtained (parameters are valid)
 * @tc.level     : Level 0
 */
void getgrouplist_0100()
{
    int ngroups;
    char who[1024] = {0};
    system("whoami > /data/user.txt");
    FILE *fptr = fopen("/data/user.txt", "r");
    fread(who, sizeof(who), 1, fptr);

    char buffer[1024] = {0};
    system("id -g > /data/id.txt");
    FILE *ptr = fopen("/data/id.txt", "r");
    fread(buffer, sizeof(buffer), 1, ptr);
    gid_t id = atoi(buffer);
    gid_t gid = getgid();
    int result = getgrouplist(who, gid, &id, &ngroups);

    EXPECT_TRUE("getgrouplist_0100", result != 0);
}

/*
 * @tc.name      : getgrouplist_0200
 * @tc.desc      : Validation cannot get group ID list (parameter invalid)
 * @tc.level     : Level 2
 */
void getgrouplist_0200()
{
    int ngroups;
    char who[1024] = {0};
    char user[1024] = {0};
    system("whoami > /data/user.txt");
    FILE *fptr = fopen("/data/user.txt", "r");
    fread(who, sizeof(who), 1, fptr);

    char buffer[1204] = {0};
    system("id -g > /data/id.txt");
    FILE *ptr = fopen("/data/id.txt", "r");
    fread(buffer, sizeof(buffer), 1, ptr);
    gid_t id = atoi(buffer);
    gid_t gid = getgid();

    int result = getgrouplist(user, gid, &id, &ngroups);

    EXPECT_EQ("getgrouplist_0200", result, -1);
    remove("/data/id.txt");
    remove("/data/user.txt");
}

int main(int argc, char *argv[])
{
    getgrouplist_0100();
    getgrouplist_0200();

    return t_status;
}