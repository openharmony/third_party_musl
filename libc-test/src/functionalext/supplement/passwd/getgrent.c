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
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getgrent_0100
 * @tc.desc      : Verify that account data can be retrieved from the group file
 * @tc.level     : Level 0
 */
void getgrent_0100(void)
{
    struct group *data;
    bool flag = false;
    data = getgrent();
    if (data != NULL) {
        flag = true;
    }
    EXPECT_TRUE("getgrent_0100", flag);
    endgrent();
}

int main()
{
    getgrent_0100();
    return t_status;
}