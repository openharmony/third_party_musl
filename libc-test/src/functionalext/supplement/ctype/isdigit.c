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

#include <ctype.h>
#include "functionalext.h"


/**
 * @tc.name      : isdigit_0100
 * @tc.desc      : Whether the characters in a string is numeric characters
 * @tc.level     : Level 0
 */
void isdigit_0100(void)
{
    const char *str = "0123456789";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isdigit(*p);
        EXPECT_EQ("isdigit_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : isdigit_0200
 * @tc.desc      : Whether a character in a string is a non-numeric character
 * @tc.level     : Level 2
 */
void isdigit_0200(void)
{
    const char *str = "!@hHiIjJZz";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isdigit(*p);
        EXPECT_EQ("isdigit_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : isdigit_0300
 * @tc.desc      : Verify non-printable characters (e.g., \\0, EOF, negative) return false
 * @tc.level     : Level 2
 */
void isdigit_0300(void)
{
    int test_chars[] = {'\0', EOF, -1, 128, 255};
    for (size_t i = 0; i < sizeof(test_chars)/sizeof(test_chars[0]); i++) {
        int ret = isdigit(test_chars[i]);
        EXPECT_EQ("isdigit_0300", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : isdigit_0400
 * @tc.desc      : Verify mixed numeric and non-numeric characters
 * @tc.level     : Level 1
 */
void isdigit_0400(void)
{
    const char str[] = "a1B2c3D4#";
    const int expected[] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
    for (size_t i = 0; str[i] != '\0'; i++) {
        int ret = isdigit(str[i]);
        EXPECT_EQ("isdigit_0400", ret, expected[i]);
    }
}

int main(void)
{
    isdigit_0100();
    isdigit_0200();
    isdigit_0300();
    isdigit_0400();
    return t_status;
}