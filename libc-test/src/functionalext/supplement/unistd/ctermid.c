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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : ctermid_0100
 * @tc.desc      : Verify the path name of the control terminal that gets the current process (parameter valid)
 * @tc.level     : Level 0
 */
void ctermid_0100(void)
{
    char termid[1025];
    bool flag = false;
    char *result = NULL;
    result = ctermid(termid);
    if (result != NULL) {
        flag = true;
    }
    EXPECT_TRUE("ctermid_0100", flag);
}

/**
 * @tc.name      : ctermid_0200
 * @tc.desc      : Verify the default pathname of the control terminal that got the current process
 *                 (the s parameter is invalid)
 * @tc.level     : Level 1
 */
void ctermid_0200(void)
{
    char termid[1025] = "ssad";
    bool flag = false;
    char *result = NULL;
    result = ctermid(termid);
    if (result != NULL) {
        flag = true;
    }
    EXPECT_TRUE("ctermid_0200", flag);
}

int main(int argc, char *argv[])
{
    ctermid_0100();
    ctermid_0200();
    return t_status;
}