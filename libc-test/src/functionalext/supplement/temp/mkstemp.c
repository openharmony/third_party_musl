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

#include <stdlib.h>
#include <sys/stat.h>
#include "functionalext.h"
#define ST_MODE_1 0600
#define ST_MODE_2 0777

/**
 * @tc.name      : mkstemp_0100
 * @tc.desc      : Verify mkstemp process success. Provide the correct template and no fixed suffix, create a
 *                 temporary file
 * @tc.level     : Level 0
 */
void mkstemp_0100(void)
{
    char tmpfile[] = "/data/mkstemp_0100_XXXXXX";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0100", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkstemp_0100", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkstemp_0200
 * @tc.desc      : Verify mkstemp process fail. Provide error template, create temp file fail
 * @tc.level     : Level 2
 */
void mkstemp_0200(void)
{
    char tmpfile[] = "/data/mkstemp_0200.dat";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0200", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkstemp_0200", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkstemp_0300
 * @tc.desc      : Template with 'XXXXXX' not at the end
 * @tc.level     : Level 2
 */
void mkstemp_0300(void)
{
    char tmpfile[] = "/data/mkstemp_0300_XXXXXX_dir";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0300", fd == -1);
    if (fd == -1) {
        EXPECT_EQ("mkstemp_0300", errno, EINVAL);
        EXPECT_STREQ("mkstemp_0300", tmpfile, "/data/mkstemp_0300_XXXXXX_dir");
    }
}

/**
 * @tc.name      : mkstemp_0400
 * @tc.desc      : Verify file permissions (0600)
 * @tc.level     : Level 1
 */
void mkstemp_0400(void)
{
    char tmpfile[] = "/data/mkstemp_0400_XXXXXX";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0400", fd != -1);
    if (fd != -1) {
        struct stat st;
        fstat(fd, &st);
        EXPECT_EQ("mkstemp_0400", st.st_mode & ST_MODE_2, ST_MODE_1);
        close(fd);
        unlink(tmpfile);
    }
}

/**
 * @tc.name      : mkstemp_0500
 * @tc.desc      : Verify concurrent mkstemp calls
 * @tc.level     : Level 1
 */
void mkstemp_0500(void)
{
    char tmpfile1[] = "/data/mkstemp_0500_XXXXXX";
    char tmpfile2[] = "/data/mkstemp_0500_XXXXXX";
    int fd1 = mkstemp(tmpfile1);
    int fd2 = mkstemp(tmpfile2);
    EXPECT_TRUE("mkstemp_0500", fd1 != -1 && fd2 != -1);
    if (fd1 != -1 && fd2 != -1) {
        EXPECT_STRNE("mkstemp_0500", tmpfile1, tmpfile2);
    }

    if (fd1 != -1) {
        close(fd1);
        unlink(tmpfile1);
    }
    if (fd2 != -1) {
        close(fd2);
        unlink(tmpfile2);
    }
}

int main(void)
{
    mkstemp_0100();
    mkstemp_0200();
    mkstemp_0300();
    mkstemp_0400();
    mkstemp_0500();
    return t_status;
}