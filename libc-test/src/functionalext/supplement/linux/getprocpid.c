/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getprocpid_0100
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0100(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1 > 1, true);
    pid_t pid2 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1 == pid2, true);
}

int main(int argc, char *argv[])
{
    getprocpid_0100();
    return t_status;
}
