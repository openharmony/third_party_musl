/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : swab_0100
 * @tc.desc      : Swap bytes from source and destination area
 * @tc.level     : Level 0
 */
void swab_0100(void)
{
    char src[20] = "swab";
    char dest[20];
    swab(src, dest, strlen(src));
    if (strcmp(dest, "wsba")) {
        t_error("%s swab failed", __func__);
    }
}

/**
 * @tc.name      : swab_0200
 * @tc.desc      : Swap bytes from source and destination area, and dest is initialized
 * @tc.level     : Level 1
 */
void swab_0200(void)
{
    char src[20] = "swab";
    char dest[20] = "swab";
    swab(src, dest, strlen(src));
    if (strcmp(dest, "wsba")) {
        t_error("%s swab failed", __func__);
    }
}

/**
 * @tc.name      : swab_0300
 * @tc.desc      : Swap bytes with zero length
 * @tc.level     : Level 1
 */
void swab_0300(void)
{
    char src[20] = "test";
    char dest[20] = "original";
    swab(src, dest, 0);
    if (strcmp(dest, "original") != 0) {
        t_error("%s swab get dest is %s are not want %s\n", __func__, dest, "original");
    }
}

/**
 * @tc.name      : swab_0400
 * @tc.desc      : Swap bytes with single byte length
 * @tc.level     : Level 1
 */
void swab_0400(void)
{
    char src[20] = "A";
    char dest[20] = "original";
    swab(src, dest, 1);
    if (strcmp(dest, "original") != 0) {
        t_error("%s swab get dest is %s are not want %s\n", __func__, dest, "original");
    }
}

/**
 * @tc.name      : swab_0500
 * @tc.desc      : Swap bytes from binary data
 * @tc.level     : Level 1
 */
void swab_0500(void)
{
    unsigned char src[] = {0x12, 0x34, 0x56, 0x78};
    unsigned char dest[4] = {0};
    swab(src, dest, 4);
    if (dest[0] != 0x34 || dest[1] != 0x12 || dest[2] != 0x78 || dest[3] != 0x56) {
        t_error("%s swab failed for binary data\n", __func__);
    }
}

/**
 * @tc.name      : swab_0600
 * @tc.desc      : Swap bytes from odd length source string
 * @tc.level     : Level 0
 */
void swab_0600(void)
{
    char src[20] = "hello";
    char dest[20] = {0};
    swab(src, dest, strlen(src));
    if (strcmp(dest, "ehll") != 0) {
        t_error("%s swab get dest is %s are not want %s\n", __func__, dest, "ehll");
    }
}

int main(int argc, char *argv[])
{
    swab_0100();
    swab_0200();
    swab_0300();
    swab_0400();
    swab_0500();
    swab_0600();
    return t_status;
}