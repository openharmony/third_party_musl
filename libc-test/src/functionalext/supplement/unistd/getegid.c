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
 * @tc.name      : getegid_0100
 * @tc.desc      : Verify that the group ID can be obtained
 * @tc.level     : Level 0
 */
void getegid_0100(void)
{
    char struid[10];
    system("id -g > ./id.txt");
    FILE *fptr = fopen("id.txt", "r");
    fread(struid, sizeof(struid), 1, fptr);
    int id = atoi(struid);
    gid_t gid = getegid();
    remove("id.txt");
    EXPECT_EQ("getegid_0100", gid, id);
}

int main()
{
    getegid_0100();
    return t_status;
}