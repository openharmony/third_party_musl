/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include <string.h>
#include "functionalext.h"

#define LENGTH 50

/**
 * @tc.name      : strcat_0100
 * @tc.desc      : Verify strcat process success
 * @tc.level     : Level 0
 */
void strcat_0100(void)
{
    char src[LENGTH] = "Source";
    char *ret = strcat(src, "Destination");
    EXPECT_STREQ("strcat_0100", ret, "SourceDestination");
}

/**
 * @tc.name      : strcat_0200
 * @tc.desc      : Verify concatenating empty string
 * @tc.level     : Level 1
 */
void strcat_0200(void)
{
    char src[LENGTH] = "Hello";
    char *ret = strcat(src, "");
    if (ret != NULL) {
        EXPECT_STREQ("strcat_0200", ret, "Hello");
        EXPECT_STREQ("strcat_0200", src, "Hello");
    }
}

/**
 * @tc.name      : strcat_0300
 * @tc.desc      : Verify concatenating to empty string
 * @tc.level     : Level 1
 */
void strcat_0300(void)
{
    char src[LENGTH] = "";
    char *ret = strcat(src, "World");
    if (ret != NULL) {
      EXPECT_STREQ("strcat_0300", ret, "World");
      EXPECT_STREQ("strcat_0300", src, "World");
    }
}

/**
 * @tc.name      : strcat_0400
 * @tc.desc      : Verify concatenation with exact buffer size
 * @tc.level     : Level 2
 */
void strcat_0400(void)
{
    char src[12] = "Hello";
    char *ret = strcat(src, "World");
    if (ret != NULL) {
        EXPECT_STREQ("strcat_0400", ret, "HelloWorld");
    }
}

/**
 * @tc.name      : strcat_0500
 * @tc.desc      : Verify multiple concatenations
 * @tc.level     : Level 1
 */
void strcat_0500(void)
{
    char src[LENGTH] = "Start";
    strcat(src, "-");
    strcat(src, "Middle");
    char *ret = strcat(src, "-End");
    if (ret != NULL) {
        EXPECT_STREQ("strcat_0500", ret, "Start-Middle-End");
    }
}

/**
 * @tc.name      : strcat_0600
 * @tc.desc      : Verify concatenation with special characters
 * @tc.level     : Level 1
 */
void strcat_0600(void)
{
    char src[LENGTH] = "Text";
    char *ret = strcat(src, "\t\n\r\0Special");
    if (ret != NULL) {
        // Note: Strcmp will stop at \0, so only the content before \0 will be compared
        EXPECT_EQ("strcat_0600", memcmp(ret, "Text\t\n\r", 7), 0);
    }
}

/**
 * @tc.name      : strcat_0700
 * @tc.desc      : Verify return pointer points to dest
 * @tc.level     : Level 1
 */
void strcat_0700(void)
{
    char src[LENGTH] = "Base";
    char *ret = strcat(src, "String");
    if (ret != NULL) {
        EXPECT_PTREQ("strcat_0700", ret, src);
    }
}

int main(void)
{
    strcat_0100();
    strcat_0200();
    strcat_0300();
    strcat_0400();
    strcat_0500();
    strcat_0600();
    strcat_0700();
    return t_status;
}
