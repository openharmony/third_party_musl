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

#define DIR_MODE 0700
/**
 * @tc.name      : mkdtemp_0100
 * @tc.desc      : Create a temporary directory with the correct parameters
 * @tc.level     : Level 0
 */
void mkdtemp_0100(void)
{
    char temp_dir[] = "test-XXXXXX";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTRNE("mkdtemp_0100", ret, NULL);
    if (ret) {
        EXPECT_NE("mkdtemp_0100", ret[0], CMPFLAG);
        rmdir(ret);
    }
}

/**
 * @tc.name      : mkdtemp_0200
 * @tc.desc      : Create temp directory with wrong template parameter
 * @tc.level     : Level 2
 */
void mkdtemp_0200(void)
{
    char temp_dir[] = "test";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTREQ("mkdtemp_0200", ret, NULL);
}

/**
 * @tc.name      : mkdtemp_0300
 * @tc.desc      : Create temp directory with wrong template argument (wrong template length)
 * @tc.level     : Level 2
 */
void mkdtemp_0300(void)
{
    char temp_dir[] = "test-XX";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTREQ("mkdtemp_0300", ret, NULL);
}

/**
 * @tc.name      : mkdtemp_0400
 * @tc.desc      : Template with 'XXXXXX' not at the end
 * @tc.level     : Level 2
 */
void mkdtemp_0400(void)
{
    char temp_dir[] = "test-XXXXXX-dir";
    char *ret = mkdtemp(temp_dir);
    //XXXXXX is not at the end, mkdtemp() will fail and return NULL. Now template is unchanged.
    EXPECT_PTREQ("mkdtemp_0400", ret, NULL);
    if (ret == NULL) {
        EXPECT_EQ("mkdtemp_0400", errno, EINVAL);
        EXPECT_STREQ("mkdtemp_0400", temp_dir, "test-XXXXXX-dir");
    }
}

/**
 * @tc.name      : mkdtemp_0500
 * @tc.desc      : Repeated calls generate unique directories
 * @tc.level     : Level 1
 */
void mkdtemp_0500(void)
{
    char temp_dir1[] = "test-XXXXXX";
    char temp_dir2[] = "test-XXXXXX";
    char *ret1 = mkdtemp(temp_dir1);
    char *ret2 = mkdtemp(temp_dir2);
    EXPECT_PTRNE("mkdtemp_0500", ret1, NULL);
    EXPECT_PTRNE("mkdtemp_0500", ret2, NULL);
    if (ret1 && ret2) {
        EXPECT_STRNE("mkdtemp_0500", ret1, ret2); // The two results should be different
        rmdir(ret1);
        rmdir(ret2);
    }
}

/**
 * @tc.name      : mkdtemp_0600
 * @tc.desc      : Check directory permissions (should be 0700)
 * @tc.level     : Level 2
 */
void mkdtemp_0600(void)
{
    char temp_dir[] = "test-XXXXXX";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTRNE("mkdtemp_0600", ret, NULL);
    if (ret) {
        struct stat st;
        stat(ret, &st);
        EXPECT_EQ("mkdtemp_0600", st.st_mode & DIR_MODE, DIR_MODE);
        rmdir(ret);
    }
}

int main(void)
{
    mkdtemp_0100();
    mkdtemp_0200();
    mkdtemp_0300();
    mkdtemp_0400();
    mkdtemp_0500();
    mkdtemp_0600();
    return t_status;
}