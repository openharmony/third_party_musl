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

#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name       : strstr_0100
 * @tc.desc       : Verify strstr finds substring at the beginning.
 * @tc.level      : Level 0
 */
void strstr_0100(void)
{
    const char *haystack = "hello world";
    const char *needle = "hello";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0100", result, needle);
}

/**
 * @tc.name       : strstr_0200
 * @tc.desc       : Verify strstr finds substring in the middle.
 * @tc.level      : Level 0
 */
void strstr_0200(void)
{
    const char *haystack = "hello world";
    const char *needle = "lo wo";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0200", result, needle);
}

/**
 * @tc.name       : strstr_0300
 * @tc.desc       : Verify strstr finds substring at the end.
 * @tc.level      : Level 0
 */
void strstr_0300(void)
{
    const char *haystack = "hello world";
    const char *needle = "world";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0300", result, needle);
}

/**
 * @tc.name       : strstr_0400
 * @tc.desc       : Verify strstr returns NULL when needle is not found.
 * @tc.level      : Level 1
 */
void strstr_0400(void)
{
    const char *haystack = "hello world";
    const char *needle = "xyz";
    char *result = strstr(haystack, needle);
    EXPECT_PTREQ("strstr_0400", result, NULL);
}

/**
 * @tc.name       : strstr_0500
 * @tc.desc       : Verify strstr returns haystack when needle is empty.
 * @tc.level      : Level 1
 */
void strstr_0500(void)
{
    const char *haystack = "hello world";
    const char *needle = "";
    char *result = strstr(haystack, needle);
    EXPECT_PTREQ("strstr_0500", result, haystack);
}

/**
 * @tc.name       : strstr_0600
 * @tc.desc       : Verify strstr is case-sensitive.
 * @tc.level      : Level 0
 */
void strstr_0600(void)
{
    const char *haystack = "Hello World";
    const char *needle = "hello";
    char *result = strstr(haystack, needle);
    EXPECT_PTREQ("strstr_0600", result, NULL);
}

/**
 * @tc.name       : strstr_0700
 * @tc.desc       : Verify strstr finds single character.
 * @tc.level      : Level 1
 */
void strstr_0700(void)
{
    const char *haystack = "hello world";
    const char *needle = "w";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0700", result, "world");
}

/**
 * @tc.name       : strstr_0800
 * @tc.desc       : Verify strstr handles special characters.
 * @tc.level      : Level 1
 */
void strstr_0800(void)
{
    const char *haystack = "test@example.com";
    const char *needle = "@";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0800", result, "@example.com");
}

/**
 * @tc.name       : strstr_0900
 * @tc.desc       : Verify strstr finds repeated substring.
 * @tc.level      : Level 1
 */
void strstr_0900(void)
{
    const char *haystack = "abababab";
    const char *needle = "aba";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_0900", result, "ababab");
}

/**
 * @tc.name       : strstr_1000
 * @tc.desc       : Verify strstr with numbers.
 * @tc.level      : Level 1
 */
void strstr_1000(void)
{
    const char *haystack = "test123456";
    const char *needle = "123";
    char *result = strstr(haystack, needle);
    EXPECT_STREQ("strstr_1000", result, "123456");
}

int main(void)
{
    strstr_0100();
    strstr_0200();
    strstr_0300();
    strstr_0400();
    strstr_0500();
    strstr_0600();
    strstr_0700();
    strstr_0800();
    strstr_0900();
    strstr_1000();
    return t_status;
}
