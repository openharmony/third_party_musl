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
 * @tc.name      : wcscspn_0100
 * @tc.desc      : Call the wcscspn method to get the length of the maximum initial segment of the wide string points
 *                 to dest
 * @tc.level     : Level 0
 */
void wcscspn_0100(void)
{
    size_t result = wcscspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    if (result != 0U) {
        t_error("%s wcscspn get result is %d are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0200
 * @tc.desc      : The return result of wcscspn when dest is a space
 * @tc.level     : Level 1
 */
void wcscspn_0200(void)
{
    size_t result = wcscspn(L"hello world", L" ");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcscspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0300
 * @tc.desc      : wcscspn returns result when dest is punctuation
 * @tc.level     : Level 1
 */
void wcscspn_0300(void)
{
    size_t result = wcscspn(L"hello world", L"!");
    size_t want = 11U;
    if (result != want) {
        t_error("%s wcscspn get result is %d are not want 11U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0400
 * @tc.desc      : Test with empty reject string
 * @tc.level     : Level 1
 */
void wcscspn_0400(void)
{
    size_t result = wcscspn(L"hello world", L"");
    size_t want = 11U;  // Should return length of entire string
    if (result != want) {
        t_error("%s wcscspn with empty reject get %zu are not want 11U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0500
 * @tc.desc      : Test with single character reject that appears at start
 * @tc.level     : Level 1
 */
void wcscspn_0500(void)
{
    size_t result = wcscspn(L"hello world", L"h");
    if (result != 0U) {
        t_error("%s wcscspn get result is %zu are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0600
 * @tc.desc      : Test with single character reject in middle
 * @tc.level     : Level 1
 */
void wcscspn_0600(void)
{
    size_t result = wcscspn(L"hello world", L"w");
    size_t want = 6U;  // "hello " = 6 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu are not want 6U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0700
 * @tc.desc      : Test with multiple reject characters
 * @tc.level     : Level 0
 */
void wcscspn_0700(void)
{
    size_t result = wcscspn(L"123.456-789", L".-");
    size_t want = 3U;  // "123" = 3 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu are not want 3U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0800
 * @tc.desc      : Test with wide characters in reject set
 * @tc.level     : Level 1
 */
void wcscspn_0800(void)
{
    size_t result = wcscspn(L"中文测试English", L"E");
    size_t want = 4U;  // "中文测试" = 4 wide characters
    if (result != want) {
        t_error("%s wcscspn with wide chars get %zu are not want 4U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0900
 * @tc.desc      : Test with empty source string
 * @tc.level     : Level 1
 */
void wcscspn_0900(void)
{
    size_t result = wcscspn(L"", L"abc");
    if (result != 0U) {
        t_error("%s wcscspn with empty source get %zu are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1000
 * @tc.desc      : Test with both source and reject empty
 * @tc.level     : Level 1
 */
void wcscspn_1000(void)
{
    size_t result = wcscspn(L"", L"");
    if (result != 0U) {
        t_error("%s wcscspn with both empty get %zu are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1100
 * @tc.desc      : Test reject character at end of string
 * @tc.level     : Level 1
 */
void wcscspn_1100(void)
{
    size_t result = wcscspn(L"hello", L"o");
    size_t want = 4U;  // "hell" = 4 characters
    if (result != want) {
        t_error("%s wcscspn get result is %zu are not want 4U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1200
 * @tc.desc      : Test with all characters in reject set
 * @tc.level     : Level 0
 */
void wcscspn_1200(void)
{
    size_t result = wcscspn(L"abcde", L"abcde");
    if (result != 0U) {
        t_error("%s wcscspn get result is %zu are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_1300
 * @tc.desc      : Test with overlapping characters in reject
 * @tc.level     : Level 2
 */
void wcscspn_1300(void)
{
    size_t result = wcscspn(L"test string", L"st ");
    size_t want = 0U;  // 't' is in reject set, at position 0
    if (result != want) {
        t_error("%s wcscspn get result is %zu are not want 0U\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcscspn_0100();
    wcscspn_0200();
    wcscspn_0300();
    wcscspn_0400();
    wcscspn_0500();
    wcscspn_0600();
    wcscspn_0700();
    wcscspn_0800();
    wcscspn_0900();
    wcscspn_1000();
    wcscspn_1100();
    wcscspn_1200();
    wcscspn_1300();
    return t_status;
}