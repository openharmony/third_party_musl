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
 * @tc.name      : wmemcmp_0100
 * @tc.desc      : Test the return value of wmemcmp when two strings are equal
 * @tc.level     : Level 0
 */
void wmemcmp_0100(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCD";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result != 0) {
        t_error("%s two strings are not equal\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0200
 * @tc.desc      : l precedes r in lexicographical order
 * @tc.level     : Level 1
 */
void wmemcmp_0200(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCE";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result >= 0) {
        t_error("%s l is not precedes to r\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0300
 * @tc.desc      : r precedes l in lexicographical order
 * @tc.level     : Level 1
 */
void wmemcmp_0300(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCC";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result <= 0) {
        t_error("%s l is not follows to r\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0400
 * @tc.desc      : Test the return value of the function when n=0
 * @tc.level     : Level 1
 */
void wmemcmp_0400(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCC";
    int n = 0;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s result is not want 0\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0500
 * @tc.desc      : Test wmemcmp comparing only first n characters
 * @tc.level     : Level 1
 */
void wmemcmp_0500(void)
{
    wchar_t l[] = L"ABCDEF";
    wchar_t r[] = L"ABCXYZ";
    int n = 3;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s first 3 chars should be equal, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0600
 * @tc.desc      : Test wmemcmp with wide characters (Unicode)
 * @tc.level     : Level 1
 */
void wmemcmp_0600(void)
{
    wchar_t l[] = L"中文测试";
    wchar_t r[] = L"中文测试";
    int n = 4;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s wide character strings are not equal\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0700
 * @tc.desc      : Test wmemcmp with mixed ASCII and wide characters
 * @tc.level     : Level 1
 */
void wmemcmp_0700(void)
{
    wchar_t l[] = L"ABC中文";
    wchar_t r[] = L"ABC中文";
    int n = 5;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s mixed strings are not equal\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0800
 * @tc.desc      : Test wmemcmp with same pointer
 * @tc.level     : Level 2
 */
void wmemcmp_0800(void)
{
    wchar_t s[] = L"ABCD";
    int n = 4;
    int result = wmemcmp(s, s, 4);
    if (result != 0) {
        t_error("%s same pointer comparison failed, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_0900
 * @tc.desc      : Test wmemcmp with n = 1
 * @tc.level     : Level 2
 */
void wmemcmp_0900(void)
{
    wchar_t l[] = L"A";
    wchar_t r[] = L"A";
    int n = 1;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s single char equality failed, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1000
 * @tc.desc      : Test wmemcmp is case sensitive
 * @tc.level     : Level 1
 */
void wmemcmp_1000(void)
{
    wchar_t l[] = L"abcd";
    wchar_t r[] = L"ABCD";
    int n = 4;
    int result = wmemcmp(l, r, n);
    if (result <= 0) {
        t_error("%s case sensitive comparison failed, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1100
 * @tc.desc      : Test wmemcmp with long strings
 * @tc.level     : Level 2
 */
void wmemcmp_1100(void)
{
    wchar_t l[] = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t r[] = L"abcdefghijklmnopqrstuvwxyz";
    int len = wcslen(l);
    int result = wmemcmp(l, r, len);
    if (result != 0) {
        t_error("%s long strings are not equal, got %d\n", __func__, result);
    }
}

/**
 * @tc.name      : wmemcmp_1200
 * @tc.desc      : Test wmemcmp with strings of different lengths
 * @tc.level     : Level 2
 */
void wmemcmp_1200(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCDE";
    int result = wmemcmp(l, r, 4);
    if (result != 0) {
        t_error("%s comparison of first 4 chars failed, got %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wmemcmp_0100();
    wmemcmp_0200();
    wmemcmp_0300();
    wmemcmp_0400();
    wmemcmp_0500();
    wmemcmp_0600();
    wmemcmp_0700();
    wmemcmp_0800();
    wmemcmp_0900();
    wmemcmp_1000();
    wmemcmp_1100();
    wmemcmp_1200();
    return t_status;
}