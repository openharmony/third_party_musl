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
#include "functionalext.h"

/**
 * @tc.name      : mktemp_0100
 * @tc.desc      : Create a temporary file with the correct parameters
 * @tc.level     : Level 0
 */
void mktemp_0100(void)
{
    char temp_file[] = "test-XXXXXX";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0100", ret, NULL);
    if (ret) {
        EXPECT_NE("mktemp_0100", ret[0], 0);
    }
}

/**
 * @tc.name      : mktemp_0200
 * @tc.desc      : Create temp file with wrong template parameter
 * @tc.level     : Level 2
 */
void mktemp_0200(void)
{
    char temp_file[] = "test";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0200", ret, NULL);
    if (ret) {
        EXPECT_EQ("mktemp_0200", ret[0], 0);
    }
}

/**
 * @tc.name      : mktemp_0300
 * @tc.desc      : Create temp file with wrong template argument (wrong template length)
 * @tc.level     : Level 2
 */
void mktemp_0300(void)
{
    char temp_file[] = "test-XX";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0300", ret, NULL);
    if (ret) {
        EXPECT_EQ("mktemp_0300", ret[0], 0);
    }
}

/**
 * @tc.name      : mktemp_0400
 * @tc.desc      : Create temp file with template argument （'XXXXXX' not at the end）
 * @tc.level     : Level 2
 */
void mktemp_0400(void)
{
    char temp_dir[] = "test-XXXXXX-dir";
    char *ret = mktemp(temp_dir);
    EXPECT_PTRNE("mktemp_0400", ret, NULL);
    //XXXXXX is not at the end, mktemp() will fail
    if (ret) {
        EXPECT_EQ("mktemp_0400", errno, EINVAL);
    }
}

/**
 * @tc.name      : mktemp_0500
 * @tc.desc      : Repeated calls generate unique directories
 * @tc.level     : Level 1
 */
void mktemp_0500(void)
{
    char temp_dir1[] = "test-XXXXXX";
    char temp_dir2[] = "test-XXXXXX";
    char *ret1 = mkdtemp(temp_dir1);
    char *ret2 = mkdtemp(temp_dir2);
    EXPECT_PTRNE("mktemp_0500", ret1, NULL);
    EXPECT_PTRNE("mktemp_0500", ret2, NULL);
    if (ret1 && ret2) {
        EXPECT_STRNE("mktemp_0500", ret1, ret2); // The two results should be different
    }
}

/**
 * @tc.name      : mktemp_0600
 * @tc.desc      : Verify mktemp retries when file exists
 * @tc.level     : Level 1
 */
void mktemp_0600(void)
{
    char temp_file[] = "test-XXXXXX";

    int fd = mkstemp(temp_file); // create file

    if (fd != -1)  {
        char *ret = mktemp(temp_file);
        EXPECT_PTRNE("mktemp_0600", ret, NULL);
        if(ret) {
            EXPECT_EQ("mktemp_0600", ret[0], 0);
        }
        unlink(temp_file); // clean file
    }
}

int main(void)
{
    mktemp_0100();
    mktemp_0200();
    mktemp_0300();
    mktemp_0400();
    mktemp_0500();
    mktemp_0600();
    return t_status;
}