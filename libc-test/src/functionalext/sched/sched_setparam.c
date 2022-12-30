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

#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "functionalext.h"
#include "test.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : sched_setparam
 * @tc.desc      : Test the function of sched_setparam.
 * @tc.level     : Level 2
 */
static void sched_setparam_0010(void)
{
    pid_t pid;
    pid = getpid();
    EXPECT_EQ("sched_setparam_0010", sched_setparam(pid, NULL), -1);
}

TEST_FUN G_Fun_Array[] = {
    sched_setparam_0010,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}