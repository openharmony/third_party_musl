/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "functionalext.h"

/**
 * @tc.name      : __realpath_chk_0100
 * @tc.desc      : Verify NULL resolved buffer delegates to realpath
 * @tc.level     : Level 0
 */
void __realpath_chk_0100(void)
{
    errno = 0;
    char *ret = __realpath_chk(".", NULL, PATH_MAX);
    EXPECT_TRUE("__realpath_chk_0100", ret != NULL);

    if (ret != NULL) {
        free(ret);
    }

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __realpath_chk_0200
 * @tc.desc      : Verify Fortify protection triggers abort when buffer too small
 * @tc.level     : Level 2
 */
void __realpath_chk_0200(void)
{

    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        char buffer[10];
        __realpath_chk(".", buffer, sizeof(buffer));
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE("__realpath_chk_0200", WIFSIGNALED(status));
        EXPECT_EQ("__realpath_chk_0200", WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_0300
 * @tc.desc      : Verify valid path with sufficient buffer size works correctly
 * @tc.level     : Level 0
 */
void __realpath_chk_0300(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk(".", buffer, sizeof(buffer));
    EXPECT_TRUE("__realpath_chk_0300", ret != NULL && ret == buffer);

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(void)
{
    __realpath_chk_0100();
    __realpath_chk_0200();
    __realpath_chk_0300();

    return t_status;
}