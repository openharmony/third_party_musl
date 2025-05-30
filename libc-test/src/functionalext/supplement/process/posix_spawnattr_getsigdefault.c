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

#include <sys/mman.h>
#include <spawn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "functionalext.h"

/**
 * @tc.name      : posix_spawnattr_getsigdefault_0100
 * @tc.desc      : get the sigal number of spawn attr(success)
 * @tc.level     : Level 0
 */
void posix_spawnattr_getsigdefault_0100(void)
{
    posix_spawnattr_t spawnattr;
    sigset_t def;
    sigset_t getdef;

    int ret = sigemptyset(&def);
    ret = sigemptyset(&getdef);
    ret = sigaddset(&def, SIGCHLD);
    if (ret == ERREXPECT) {
        EXPECT_NE("posix_spawnattr_getsigdefault_0100", ret, ERREXPECT);
        return;
    }

    posix_spawnattr_init(&spawnattr);
    ret = posix_spawnattr_setsigdefault(&spawnattr, &def);

    ret = posix_spawnattr_getsigdefault(&spawnattr, &getdef);
    EXPECT_EQ("posix_spawnattr_getsigdefault_0100", ret, CMPFLAG);
    EXPECT_NE("posix_spawnattr_getsigdefault_0100", sigismember(&getdef, SIGCHLD), CMPFLAG);
    posix_spawnattr_destroy(&spawnattr);
}

int main(void)
{
    posix_spawnattr_getsigdefault_0100();

    return t_status;
}
