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
 * @tc.name      : wcslen_0100
 * @tc.desc      : Calculate the length of a wide character array
 * @tc.level     : Level 0
 */
void wcslen_0100(void)
{
    wchar_t str[] = L"wcslen";
    int target = 6;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0200
 * @tc.desc      : Calculate the length of an empty wide character string
 * @tc.level     : Level 1
 */
void wcslen_0200(void)
{
    wchar_t str[] = L"";
    int target = 0;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s empty string wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0300
 * @tc.desc      : Calculate the length of a wide character string with spaces
 * @tc.level     : Level 1
 */
void wcslen_0300(void)
{
    wchar_t str[] = L"Hello World";
    int target = 11;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s string with spaces wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0400
 * @tc.desc      : Calculate the length of a wide character string with special characters
 * @tc.level     : Level 1
 */
void wcslen_0400(void)
{
    wchar_t str[] = L"Test@123#";
    int target = 9;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s special characters wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0500
 * @tc.desc      : Calculate the length of a wide character string with unicode characters
 * @tc.level     : Level 2
 */
void wcslen_0500(void)
{
    wchar_t str[] = L"中文测试";
    int target = 4;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s unicode characters wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0600
 * @tc.desc      : Calculate the length of a single wide character
 * @tc.level     : Level 1
 */
void wcslen_0600(void)
{
    wchar_t str[] = L"A";
    int target = 1;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s single character wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0700
 * @tc.desc      : Calculate the length of a wide character string with mixed languages
 * @tc.level     : Level 2
 */
void wcslen_0700(void)
{
    wchar_t str[] = L"Hello 世界 123";
    int target = 12;
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s mixed languages wcslen is not right", __func__);
    }
}

/**
 * @tc.name      : wcslen_0800
 * @tc.desc      : Calculate the length of a wide character string with null character in middle
 * @tc.level     : Level 2
 */
void wcslen_0800(void)
{
    wchar_t str[] = L"Partial\0String";
    int target = 7; // Should stop at null character
    size_t num = wcslen(str);
    if (num != target) {
        t_error("%s null character wcslen is not right", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcslen_0100();
    wcslen_0200();
    wcslen_0300();
    wcslen_0400();
    wcslen_0500();
    wcslen_0600();
    wcslen_0700();
    wcslen_0800();
    return t_status;
}