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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : getuid_0100
 * @tc.desc      : Can get the UID of the current process.
 * @tc.level     : Level 0
 */
void getuid_0100(void)
{
    char struid[10];
    bool successflag = false;
    system("id -u > ./id.txt");
    FILE *fptr = fopen("id.txt", "r");
    fread(struid, sizeof(struid), 1, fptr);
    int intuid = atoi(struid);
    uid_t uid = getuid();
    remove("id.txt");
    EXPECT_EQ("getuid_0100", uid, intuid);
}

int main()
{
    getuid_0100();

    return t_status;
}