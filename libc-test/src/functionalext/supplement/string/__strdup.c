/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : __strdup_0100
 * @tc.desc      : Verify __strdup process success
 * @tc.level     : Level 0
 */
void __strdup_0100(void)
{
    char *ret = __strdup("strduptest");
    EXPECT_STREQ("__strdup_0100", ret, "strduptest");
}

/**
 * @tc.name      : __strdup_0200
 * @tc.desc      : Verify __strdup process empty string
 * @tc.level     : Level 0
 */
void __strdup_0200(void)
{
    char *ret = __strdup("");
    EXPECT_STREQ("__strdup_0200", ret, "");
}

/**
 * @tc.name      : __strdup_0300
 * @tc.desc      : Verify __strdup process long string
 * @tc.level     : Level 0
 */
void __strdup_0300(void)
{
    char longStr[1025];
    memset(longStr, 'A', 1024);
    longStr[1024] = '\0';
    char *ret = __strdup(longStr);
    EXPECT_STREQ("__strdup_0300", ret, longStr);
    free(ret);
}

/**
 * @tc.name      : __strdup_0400
 * @tc.desc      : Verify __strdup process special characters
 * @tc.level     : Level 0
 */
void __strdup_0400(void)
{
    char *testStr = "test\nstring\twith\rspecial chars\x01\x02";
    char *ret = __strdup(testStr);
    EXPECT_STREQ("__strdup_0400", ret, testStr);
    free(ret);
}

int main(void)
{
    __strdup_0100();
    __strdup_0200();
    __strdup_0300();
    __strdup_0400();
    return t_status;
}