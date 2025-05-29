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

#include <fcntl.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : mkostemps_0100
 * @tc.desc      : Verify mkostemps process success.  Provide the correct template and fixed suffix,
 *                 create a temporary file.
 * @tc.level     : Level 0
 */
void mkostemps_0100(void)
{
    char tmpfile[] = "/data/local/tmp/mkostemps_0100_XXXXXX.dat";
    int fd = mkostemps(tmpfile, strlen(".dat"), O_CREAT);
    EXPECT_TRUE("mkostemps_0100", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemps_0100", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemps_0200
 * @tc.desc      : Verify mkostemps process success. Provide the correct template and no fixed suffix,
 *                 create a temporary file.
 * @tc.level     : Level 0
 */
void mkostemps_0200(void)
{
    char tmpfile[] = "/data/local/tmp/mkostemps_0200_XXXXXX";
    int fd = mkostemps(tmpfile, 0, O_CREAT);
    EXPECT_TRUE("mkostemps_0200", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemps_0200", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemps_0300
 * @tc.desc      : Verify mkstemp process fail. Because provide error template.
 * @tc.level     : Level 2
 */
void mkostemps_0300(void)
{
    char tmpfile[] = "/data/local/tmp/mkostemps_0300.dat";
    int fd = mkostemps(tmpfile, strlen(".dat"), O_CREAT);
    EXPECT_TRUE("mkostemps_0300", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemps_0300", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemps_0400
 * @tc.desc      : Template with 'XXXXXX' not at the end
 * @tc.level     : Level 2
 */
void mkostemps_0400(void)
{
    char tmpfile[] = "/data/local/tmp/mkostemps_0400_XXXXXX_dir";
    int fd = mkostemps(tmpfile, 0, O_CREAT);
    EXPECT_TRUE("mkostemps_0400", fd == -1);
    if (fd == -1) {
        EXPECT_EQ("mkostemp_0700", errno, EINVAL);
        EXPECT_STREQ("mkostemp_0700", tmpfile, "/data/local/tmp/mkostemps_0400_XXXXXX_dir");
    }
}

/**
 * @tc.name      : mkostemps_0500
 * @tc.desc      : Verify mkostemps with large suffix length
 * @tc.level     : Level 2
 */
void mkostemps_0500(void)
{
    char tmpfile[] = "/data/local/tmp/mkostemps_0500_XXXXXX.dat";
    int fd = mkostemps(tmpfile, 100, O_CREAT);  // Suffix length larger than actual suffix
    EXPECT_TRUE("mkostemps_0500", fd == -1);
    if (fd == -1) {
        EXPECT_EQ("mkostemps_0500", errno, EINVAL);
    }
}

/**
 * @tc.name      : mkostemps_0600
 * @tc.desc      : Verify mkostemp generates unique filenames on multiple calls
 * @tc.level     : Level 2
 */
void mkostemps_0600(void)
{
    char tmpfile1[] = "/data/local/tmp/mkostemps_0600_XXXXXX";
    char tmpfile2[] = "/data/local/tmp/mkostemps_0600_XXXXXX";
    int fd1 = mkostemps(tmpfile1, 0, O_CREAT);
    int fd2 = mkostemps(tmpfile2, 0, O_CREAT);
    EXPECT_TRUE("mkostemps_0600", fd1 != -1 && fd2 != -1);
    if (fd1 != -1 && fd2 != -1) {
        EXPECT_STRNE("mkstemp_0600", tmpfile1, tmpfile2);
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
    mkostemps_0100();
    mkostemps_0200();
    mkostemps_0300();
    mkostemps_0400();
    mkostemps_0500();
    mkostemps_0600();
    return t_status;
}