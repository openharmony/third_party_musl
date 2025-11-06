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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcsspn_0100
 * @tc.desc      : Test the wcsspn function to get the length of the largest initial segment of the wide string pointed
 * @tc.level     : Level 0
 */
void wcsspn_0100(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0200
 * @tc.desc      : Test the wcsspn result when the search string contains spaces
 * @tc.level     : Level 1
 */
void wcsspn_0200(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz ");
    size_t want = 11U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 11U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0300
 * @tc.desc      : The result of the wcsspn function when no target exists for the retrieved string
 * @tc.level     : Level 1
 */
void wcsspn_0300(void)
{
    size_t result = wcsspn(L"hello world", L"!");
    if (result != 0U) {
        t_error("%s wcsspn get result is %d are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0400
 * @tc.desc      : Empty string as input
 * @tc.level     : Level 1
 */
void wcsspn_0400(void)
{
    size_t result = wcsspn(L"", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0500
 * @tc.desc      : Empty accept set
 * @tc.level     : Level 1
 */
void wcsspn_0500(void)
{
    size_t result = wcsspn(L"hello world", L"");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0600
 * @tc.desc      : Mixed alphanumeric characters in accept set
 * @tc.level     : Level 0
 */
void wcsspn_0600(void)
{
    size_t result = wcsspn(L"abc123def", L"abcdefghijklmnopqrstuvwxyz0123456789");
    size_t want = 9U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 9U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0700
 * @tc.desc      : Non-ASCII wide characters in accept set
 * @tc.level     : Level 0
 */
void wcsspn_0700(void)
{
    size_t result = wcsspn(L"中文测试English", L"中文测试");
    size_t want = 4U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 4U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0800
 * @tc.desc      : Space and punctuation characters in accept set
 * @tc.level     : Level 0
 */
void wcsspn_0800(void)
{
    size_t result = wcsspn(L"   Hello, World!", L" ,.!?");
    size_t want = 3U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 3U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0900
 * @tc.desc      : Partial match with digits and letters
 * @tc.level     : Level 0
 */
void wcsspn_0900(void)
{
    size_t result = wcsspn(L"abc123!@#", L"abcdefghijklmnopqrstuvwxyz0123456789");
    size_t want = 6U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 6U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1000
 * @tc.desc      : Single character accept set
 * @tc.level     : Level 1
 */
void wcsspn_1000(void)
{
    size_t result = wcsspn(L"aaaaabbbbb", L"a");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1100
 * @tc.desc      : Characters not in order in accept set
 * @tc.level     : Level 1
 */
void wcsspn_1100(void)
{
    size_t result = wcsspn(L"hello", L"oleh"); // Same characters, different order
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1200
 * @tc.desc      : Null character in the middle of accept set
 * @tc.level     : Level 2
 */
void wcsspn_1200(void)
{
    wchar_t accept_set[] = L"abc\0def"; // Null character in the middle
    size_t result = wcsspn(L"abcdef", accept_set);
    size_t want = 3U; // Should only match up to 'c'
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 3U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1300
 * @tc.desc      : Entire string consists of characters from accept set
 * @tc.level     : Level 0
 */
void wcsspn_1300(void)
{
    size_t result = wcsspn(L"abcdef", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 6U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 6U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_1400
 * @tc.desc      : No characters from accept set at the beginning
 * @tc.level     : Level 0
 */
void wcsspn_1400(void)
{
    size_t result = wcsspn(L"123hello", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 0U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 0U\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsspn_0100();
    wcsspn_0200();
    wcsspn_0300();
    wcsspn_0400();
    wcsspn_0500();
    wcsspn_0600();
    wcsspn_0700();
    wcsspn_0800();
    wcsspn_0900();
    wcsspn_1000();
    wcsspn_1100();
    wcsspn_1200();
    wcsspn_1300();
    wcsspn_1400();
    return t_status;
}