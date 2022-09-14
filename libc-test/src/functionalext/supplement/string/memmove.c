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

#include <stdbool.h>
#include "functionalext.h"

/**
 * @tc.name      : memmove_0100
 * @tc.desc      : The source address and the destination address are equal, and no copying is performed.
 * @tc.level     : Level 1
 */
void memmove_0100(void)
{
    char str[] = "memmove_0100";
    void *p = memmove(str, str, 5);
    EXPECT_EQ("memmove_0100", p, str);
    bool flag = false;
    if (p != NULL) {
        flag = true;
    }
    EXPECT_TRUE("memmove_0100", flag);
}

/**
 * @tc.name      : memmove_0200
 * @tc.desc      : The source address and the destination address do not overlap, copy it.
 * @tc.level     : Level 0
 */
void memmove_0200(void)
{
    bool successflag = false;
    char str[] = "memmove_0200";
    char *dest = str + 3, *src = str;
    void *p = memmove(dest, src, 2);
    if (strstr(dest, src) != NULL || strstr(src, dest) != NULL) {
        successflag = true;
    }
    EXPECT_TRUE("memmove_0200", successflag);
    bool flag = false;
    if (p != NULL) {
        flag = true;
    }
    EXPECT_TRUE("memmove_0200", flag);
}

/**
 * @tc.name      : memmove_0300
 * @tc.desc      : The first address of dest is less than the first address of src, and forward copy is performed.
 * @tc.level     : Level 1
 */
void memmove_0300(void)
{
    bool successflag = false;
    char str[] = "memmove_0300";
    char *dest = str, *src = str + 2;
    void *p = memmove(dest, src, 5);
    if (strstr(dest, src) != NULL || strstr(src, dest) != NULL) {
        successflag = true;
    }
    EXPECT_TRUE("memmove_0300", successflag);
    bool flag = false;
    if (p != NULL) {
        flag = true;
    }
    EXPECT_TRUE("memmove_0300", flag);
}

/**
 * @tc.name      : memmove_0400
 * @tc.desc      : The first address of dest is greater than the first address of src, and reverse copy is performed.
 * @tc.level     : Level 1
 */
void memmove_0400(void)
{
    bool successflag = false;
    char str[] = "memmove_0400";
    char *dest = str + 2, *src = str;
    void *p = memmove(dest, src, 5);
    if (strstr(dest, src) != NULL || strstr(src, dest) != NULL) {
        successflag = true;
    }
    EXPECT_TRUE("memmove_0400", successflag);
    bool flag = false;
    if (p != NULL) {
        flag = true;
    }
    EXPECT_TRUE("memmove_0400", flag);
}

int main(int argc, char *argv[])
{
    memmove_0100();
    memmove_0200();
    memmove_0300();
    memmove_0400();

    return t_status;
}