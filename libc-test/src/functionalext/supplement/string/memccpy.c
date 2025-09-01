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

#include <string.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 128
#define TEST_DATA_LEN 2

/**
 * @tc.name      : memccpy_0100
 * @tc.desc      : Copies a string until spaces or data is greater than the specified length
 * @tc.level     : Level 0
 */
void memccpy_0100(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, ' ', TEST_DATA_LEN);
    EXPECT_PTREQ("memccpy_0100", rev, NULL);
    EXPECT_EQ("memccpy_0100", strlen(dest), TEST_DATA_LEN);
    for (i = 0; i < TEST_DATA_LEN; i++) {
        EXPECT_EQ("memccpy_0100", dest[i], src[i]);
    }

    memset(dest, 0x0, sizeof(dest));
    rev = memccpy(dest, src, ' ', sizeof(src));
    EXPECT_PTRNE("memccpy_0100", rev, NULL);
    EXPECT_EQ("memccpy_0100", strlen(dest), strlen("musl "));
    for (i = 0; i < strlen("musl "); i++) {
        EXPECT_EQ("memccpy_0100", dest[i], src[i]);
    }
}

/**
 * @tc.name      : memccpy_0200
 * @tc.desc      : Use a character not contained in the original string as the end position
 *                 for processing the copied string
 * @tc.level     : Level 0
 */
void memccpy_0200(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, 'A', strlen(src));
    EXPECT_PTREQ("memccpy_0200", rev, NULL);
    EXPECT_STREQ("memccpy_0200", dest, src);
}

/**
 * @tc.name      : memccpy_0300
 * @tc.desc      : The length of the copied string is 0
 * @tc.level     : Level 2
 */
void memccpy_0300(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, ' ', 0);
    EXPECT_PTREQ("memccpy_0300", rev, NULL);
    EXPECT_EQ("memccpy_0300", strlen(dest), 0);
}

/**
 * @tc.name      : memccpy_0400
 * @tc.desc      : Verify memccpy stops at first occurrence of specified character
 * @tc.level     : Level 1
 */
void memccpy_0400(void)
{
    const char src[] = "multiple|delimited|values";
    char dest[32] = {0};
    void *result = memccpy(dest, src, '|', sizeof(src));

    EXPECT_PTRNE("memccpy_0400", result, NULL);
    if (result != NULL) {
        EXPECT_STREQ("memccpy_0400", dest, "multiple|");
        EXPECT_EQ("memccpy_0400", (char *)result - dest, strlen("multiple") + 1);
    }
}

/**
 * @tc.name      : memccpy_0500
 * @tc.desc      : Verify memccpy handles binary data with null bytes
 * @tc.level     : Level 2
 */
void memccpy_0500(void)
{
    const char src[] = {'a', 'b', '\0', 'c', 'd'};
    char dest[5] = {0};
    void *result = memccpy(dest, src, 'd', sizeof(src));

    EXPECT_PTRNE("memccpy_0500", result, NULL);
    if (result != NULL) {
        EXPECT_EQ("memccpy_0500", memcmp(dest, src, 5), 0);
        EXPECT_EQ("memccpy_0500", (char *)result - dest, 5);
    }
}

/**
 * @tc.name      : memccpy_0600
 * @tc.desc      : Verify memccpy copies full length when stop char not found
 * @tc.level     : Level 1
 */
void memccpy_0600(void)
{
    const char src[] = "no_stop_char_here";
    char dest[32] = {0};
    void *result = memccpy(dest, src, 'X', strlen(src));

    EXPECT_PTREQ("memccpy_0600", result, NULL);
    EXPECT_STREQ("memccpy_0600", dest, src);
}

/**
 * @tc.name      : memccpy_0700
 * @tc.desc      : Verify memccpy with stop character at last position
 * @tc.level     : Level 1
 */
void memccpy_0700(void)
{
    const char src[] = "stop_at_end$";
    char dest[32] = {0};
    void *result = memccpy(dest, src, '$', strlen(src));

    EXPECT_PTRNE("memccpy_0700", result, NULL);
    if (result != NULL) {
        EXPECT_STREQ("memccpy_0700", dest, src);
        EXPECT_EQ("memccpy_0700", (char *)result - dest, strlen(src));
    }
}

/**
 * @tc.name      : memccpy_0900
 * @tc.desc      : Verify memccpy with maximum copy length before stop char
 * @tc.level     : Level 2
 */
void memccpy_0800(void)
{
    const char src[] = "copy|this";
    char dest[32] = {0};
    void *result = memccpy(dest, src, '|', 4); // Should stop at length before seeing '|'

    EXPECT_PTREQ("memccpy_0800", result, NULL);
    EXPECT_EQ("memccpy_0800", memcmp(dest, "copy", 4), 0);
}

/**
 * @tc.name      : memccpy_0900
 * @tc.desc      : Verify memccpy with stop character being the first byte
 * @tc.level     : Level 1
 */
void memccpy_0900(void)
{
    const char src[] = "*start*with*star";
    char dest[32] = {0};
    void *result = memccpy(dest, src, '*', strlen(src));

    if (result != NULL) {
        EXPECT_PTREQ("memccpy_0900", result, dest + 1);
        EXPECT_EQ("memccpy_0900", dest[0], '*');
    }
}

int main(void)
{
    memccpy_0100();
    memccpy_0200();
    memccpy_0300();
    memccpy_0400();
    memccpy_0500();
    memccpy_0600();
    memccpy_0700();
    memccpy_0800();
    memccpy_0900();
    return t_status;
}