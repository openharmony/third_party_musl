/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#define VALUE_0 2
#define VALUE_A 12
#define VALUE_Z 63
#define VALUE 64
#define VALUE_0123 1327298
#define VALUE_ABCD 3990348
#define VALUE_ZZZZZ 1073741823

/**
 * @tc.name      : a64l_0100
 * @tc.desc      : Verify normal character
 * @tc.level     : Level 0
 */
void a64l_0100(void)
{
    long result1 = a64l("0");
    long result2 = a64l("A");
    long result3 = a64l("z");
    long result4 = a64l("./");
    EXPECT_EQ("a64l_0100", result1, VALUE_0); // expect result is 2
    EXPECT_EQ("a64l_0100", result2, VALUE_A); // expect result is 12
    EXPECT_EQ("a64l_0100", result3, VALUE_Z); // expect result is 63
    EXPECT_EQ("a64l_0100", result4, VALUE); // expect result is 64
}

/**
 * @tc.name      : a64l_0200
 * @tc.desc      : Verify multi character
 * @tc.level     : Level 0
 */
void a64l_0200(void)
{
    long result1 = a64l("0123");
    long result2 = a64l("ABCD");
    EXPECT_EQ("a64l_0200", result1, VALUE_0123); // (2 | (3 << 6) | (4 << 12) | (5 << 18))
    EXPECT_EQ("a64l_0200", result2, VALUE_ABCD); // (12 | (13 << 6) | (14 << 12) | (15 << 18)
}

/**
 * @tc.name      : a64l_0300
 * @tc.desc      : Verify illegal character
 * @tc.level     : Level 0
 */
void a64l_0300(void)
{
    long result1 = a64l("A@B");
    long result2 = a64l("0#");
    EXPECT_EQ("a64l_0300", result1, VALUE_A); // expect result is 12
    EXPECT_EQ("a64l_0300", result2, VALUE_0); // expect result is 2
}

/**
 * @tc.name      : a64l_0400
 * @tc.desc      : Verify empty characters
 * @tc.level     : Level 0
 */
void a64l_0400(void)
{
    long result = a64l("");
    EXPECT_EQ("a64l_0400", result, 0);
}

/**
 * @tc.name      : a64l_0500
 * @tc.desc      : Verify the maximum length
 * @tc.level     : Level 0
 */
void a64l_0500(void)
{
    long result1 = a64l("zzzzzz");
    long result2 = a64l("zzzzz");
    EXPECT_EQ("a64l_0500", result1, -1); // (63 | (63 << 6) | (63 << 12) | (63 << 18) | (63 << 24) | (63 << 30))
    EXPECT_EQ("a64l_0500", result2, VALUE_ZZZZZ); // (63 | (63 << 6) | (63 << 12) | (63 << 18) | (63 << 24))
}

int main(int argc, char *argv[])
{
    a64l_0100();
    a64l_0200();
    a64l_0300();
    a64l_0400();
    a64l_0500();
    return t_status;
}