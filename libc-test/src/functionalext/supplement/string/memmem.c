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

/**
 * @tc.name      : memmem_0100
 * @tc.desc      : Normal value test, assert whether the address value of the return value is
 *                 the created buffer address
 * @tc.level     : Level 0
 */
void memmem_0100(void)
{
    const char buffer[] = "musl test";
    const char cmp[] = "musl";
    char *ptr = NULL;

    for (int i = 0; i < sizeof(cmp) / sizeof(cmp[0]); i++) {
        ptr = memmem(buffer, sizeof(buffer), cmp, i);
        if (ptr == NULL) {
            EXPECT_PTRNE("memmem_0100", ptr, NULL);
            return;
        }
        EXPECT_PTREQ("memmem_0100", ptr, buffer);
    }
}

/**
 * @tc.name      : memmem_0200
 * @tc.desc      : Assert whether the returned value of an invalid parameter is null
 * @tc.level     : Level 2
 */
void memmem_0200(void)
{
    const char buffer[] = "musl test";
    size_t buflen = sizeof(buffer) / sizeof(buffer[0]);
    size_t errlen = buflen + 1;
    char *ptr = NULL;

    ptr = memmem(buffer, sizeof(buffer), "musl", errlen);
    EXPECT_PTREQ("memmem_0200", ptr, NULL);

    ptr = memmem(buffer, sizeof(buffer), "muls", 3);
    EXPECT_PTREQ("memmem_0200", ptr, NULL);
}

/**
 * @tc.name      : memmem_0300
 * @tc.desc      : Verify memmem returns NULL when substring not found
 * @tc.level     : Level 1
 */
void memmem_0300(void)
{
    const char buffer[] = "test musl";
    const char needle[] = "notfound";
    char *ptr = memmem(buffer, sizeof(buffer), needle, sizeof(needle)-1);
    EXPECT_PTREQ("memmem_0300", ptr, NULL);
}

/**
 * @tc.name      : memmem_0400
 * @tc.desc      : Verify memmem handles empty needle correctly
 * @tc.level     : Level 1
 */
void memmem_0400(void)
{
    const char buffer[] = "test musl";
    const char needle[] = "";
    char *ptr = memmem(buffer, sizeof(buffer), needle, 0);
    EXPECT_PTREQ("memmem_0400", ptr, buffer);
}

/**
 * @tc.name      : memmem_0500
 * @tc.desc      : Verify memmem handles binary data correctly
 * @tc.level     : Level 2
 */
void memmem_0500(void)
{
    const unsigned char buffer[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    const unsigned char needle[] = {0x03, 0x04};
    char *ptr = memmem(buffer, sizeof(buffer), needle, sizeof(needle));
    EXPECT_PTREQ("memmem_0500", ptr, (char *)buffer + 2);
}

/**
 * @tc.name      : memmem_0600
 * @tc.desc      : Verify memmem returns NULL when needle larger than haystack
 * @tc.level     : Level 2
 */
void memmem_0600(void)
{
    const char buffer[] = "test";
    const char needle[] = "test string";
    char *ptr = memmem(buffer, sizeof(buffer)-1, needle, sizeof(needle)-1);
    EXPECT_PTREQ("memmem_0600", ptr, NULL);
}

/**
 * @tc.name      : memmem_0700
 * @tc.desc      : Verify memmem finds partial matches correctly
 * @tc.level     : Level 2
 */
void memmem_0700(void)
{
    const char buffer[] = "test musl";
    const char needle[] = "musx";
    char *ptr = memmem(buffer, sizeof(buffer), needle, 3); // Only compare first 3 chars
    EXPECT_PTREQ("memmem_0700", ptr, buffer + 5);
}

/**
 * @tc.name      : memmem_0800
 * @tc.desc      : Verify memmem handles zero-length haystack correctly
 * @tc.level     : Level 2
 */
void memmem_0800(void)
{
    const char buffer[] = "";
    const char needle[] = "test";
    char *ptr = memmem(buffer, 0, needle, sizeof(needle)-1);
    EXPECT_PTREQ("memmem_0800", ptr, NULL);
}

int main(void)
{
    memmem_0100();
    memmem_0200();
    memmem_0300();
    memmem_0400();
    memmem_0500();
    memmem_0600();
    memmem_0700();
    memmem_0800();

    return t_status;
}