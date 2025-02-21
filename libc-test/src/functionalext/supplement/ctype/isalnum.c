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

const int COUNT = 62;
const int ISALNUM_SUCCESS_RETURN = 1;
const int COUNT_DEFAULT_LOCALE_UTF_8_1_BYTE = 62;
const int COUNT_DEFAULT_LOCALE_UTF_8_2_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_3_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE = 0;
const int COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE_NEGTIVE = 0;
const int UTF_8_1_BYTE_MIN = 0x00;
const int UTF_8_1_BYTE_MAX = 0x7f;
const int UTF_8_2_BYTE_MIN = 0x0080;
const int UTF_8_2_BYTE_MAX = 0x07ff;
const int UTF_8_3_BYTE_MIN = 0x0800;
const int UTF_8_3_BYTE_MAX = 0xffff;
const int UTF_8_4_BYTE_MIN = 0x10000;
const int UTF_8_4_BYTE_MAX = 0x10ffff;
const int EOK = 0;

/**
 * @tc.name      : isalnum_0100
 * @tc.desc      : The parameter c is an English letter, and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_0100(void)
{
    int ret = isalnum('a');
    EXPECT_NE("isalnum_0100", ret, EOK);
}

/**
 * @tc.name      : isalnum_0200
 * @tc.desc      : The parameter c is an English number, and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
void isalnum_0200(void)
{
    int ret = isalnum('1');
    EXPECT_NE("isalnum_0200", ret, EOK);
}

/**
 * @tc.name      : isalnum_0300
 * @tc.desc      : The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_0300(void)
{
    int ret = isalnum('*');
    EXPECT_EQ("isalnum_0300", ret, EOK);
}

/**
 * @tc.name      : isalnum_0400
 * @tc.desc      : Determine the number of letters and numbers in the ascii code table.
 * @tc.level     : Level 1
 */
void isalnum_0400(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isalnum((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isalnum_0400", total, COUNT);
}

/**
 * @tc.name      : isalnum_0500
 * @tc.desc      : Test all input:0x00 - 0x7f(UTF-8 1 byte).
 * @tc.level     : Level 1
 */
void isalnum_0500(void)
{
    int total = 0;
    for (int i = UTF_8_1_BYTE_MIN; i <= UTF_8_1_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_0500", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_0500", total, COUNT_DEFAULT_LOCALE_UTF_8_1_BYTE);
}

/**
 * @tc.name      : isalnum_0600
 * @tc.desc      : Test all input:0x0080 - 0x07ff(UTF-8 2 byte).
 * @tc.level     : Level 1
 */
void isalnum_0600(void)
{
    int total = 0;
    for (int i = UTF_8_2_BYTE_MIN; i <= UTF_8_2_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_0600", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_0600", total, COUNT_DEFAULT_LOCALE_UTF_8_2_BYTE);
}

/**
 * @tc.name      : isalnum_0700
 * @tc.desc      : Test all input:0x0800 - 0xffff(UTF-8 3 byte).
 * @tc.level     : Level 1
 */
void isalnum_0700(void)
{
    int total = 0;
    for (int i = UTF_8_3_BYTE_MIN; i <= UTF_8_3_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_0700", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_0700", total, COUNT_DEFAULT_LOCALE_UTF_8_3_BYTE);
}

/**
 * @tc.name      : isalnum_0800
 * @tc.desc      : Test all input:0x10000 - 0x10ffff(UTF-8 4 byte).
 * @tc.level     : Level 1
 */
void isalnum_0800(void)
{
    int total = 0;
    for (int i = UTF_8_4_BYTE_MIN; i <= UTF_8_4_BYTE_MAX; i++) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_0800", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_0800", total, COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE);
}

/**
 * @tc.name      : isalnum_0900
 * @tc.desc      : Test zero input.
 * @tc.level     : Level 1
 */
void isalnum_0900(void)
{
    int ret = isalnum(0);
    EXPECT_EQ("isalnum_0900", ret, 0);
}

/**
 * @tc.name      : isalnum_1000
 * @tc.desc      : Test negative input -1.
 * @tc.level     : Level 1
 */
void isalnum_1000(void)
{
    int ret = isalnum(-1);
    EXPECT_EQ("isalnum_1000", ret, 0);
}

/**
 * @tc.name      : isalnum_1100
 * @tc.desc      : Test negative input, max -1 * 0x10ffff.
 * @tc.level     : Level 1
 */
void isalnum_1100(void)
{
    int total = 0;
    for (int i = -1; i >= -1 * UTF_8_4_BYTE_MAX; i--) {
        int ret = isalnum(i);
        if (ret) {
            EXPECT_EQ("isalnum_1100", ret, ISALNUM_SUCCESS_RETURN);
            total++;
        }
    }
    EXPECT_EQ("isalnum_1100", total, COUNT_DEFAULT_LOCALE_UTF_8_4_BYTE_NEGTIVE);
}

int main(void)
{
    isalnum_0100();
    isalnum_0200();
    isalnum_0300();
    isalnum_0400();
    isalnum_0500();
    isalnum_0600();
    isalnum_0700();
    isalnum_0800();
    isalnum_0900();
    isalnum_1000();
    isalnum_1100();

    return t_status;
}