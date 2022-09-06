/*
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

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int COUNT_ZERO = 0;
const int COUNT_ONE = 1;

/**
 * @tc.name      : inet_aton_0100
 * @tc.desc      : Verify converting an IP address string to a network binary number (parameter valid)
 * @tc.level     : Level 0
 */
void inet_aton_0100()
{
    bool flag = false;
    char ip4test[] = "127.0.0.1";
    struct in_addr sin_addr;
    int result;
    result = inet_aton(ip4test, &sin_addr);
    if (sin_addr.s_addr == 16777343) {
        flag = true;
    }
    EXPECT_TRUE("inet_aton_0100", flag);
    EXPECT_EQ("inet_aton_0100", result, COUNT_ONE);
}

/**
 * @tc.name      : inet_aton_0200
 * @tc.desc      : Validation cannot convert an IP address string to a network binary number
 *                 (the s0 argument is invalid and has a negative number)
 * @tc.level     : Level 2
 */
void inet_aton_0200()
{
    char ip4test[] = "-127.0.0.1";
    struct in_addr sin_addr;
    int result;
    result = inet_aton(ip4test, &sin_addr);
    EXPECT_EQ("inet_aton_0200", result, COUNT_ZERO);
}

/**
 * @tc.name      : inet_aton_0300
 * @tc.desc      : Validation cannot convert an IP address string to a network binary number
 *                 (s0 argument invalid, number greater than 255 exists)
 * @tc.level     : Level 2
 */
void inet_aton_0300()
{
    char ip4test[] = "127.0.256.1";
    struct in_addr sin_addr;
    int result;
    result = inet_aton(ip4test, &sin_addr);
    EXPECT_EQ("inet_aton_0300", result, COUNT_ZERO);
}

/**
 * @tc.name      : inet_aton_0400
 * @tc.desc      : Validation cannot convert an IP address string to a network binary number
 *                 (the s0 argument is invalid, the format is not correct)
 * @tc.level     : Level 2
 */
void inet_aton_0400()
{
    char ip4test[] = "127.1.2.3.4";
    struct in_addr sin_addr;
    int result;
    result = inet_aton(ip4test, &sin_addr);
    EXPECT_EQ("inet_aton_0400", result, COUNT_ZERO);
}

/**
 * @tc.name      : inet_aton_0500
 * @tc.desc      : Validation cannot convert an IP address string to a network binary number
 *                 (the s0 argument is invalid and contains characters)
 * @tc.level     : Level 2
 */
void inet_aton_0500()
{
    char ip4test[] = "127.0.w.1";
    struct in_addr sin_addr;
    int result;
    result = inet_aton(ip4test, &sin_addr);
    EXPECT_EQ("inet_aton_0500", result, COUNT_ZERO);
}

TEST_FUN G_Fun_Array[] = {
    inet_aton_0100,
    inet_aton_0200,
    inet_aton_0300,
    inet_aton_0400,
    inet_aton_0500,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}