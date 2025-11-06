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
 * @tc.name      : wmemmove_0100
 * @tc.desc      : Copies the specified number of wide characters from the source to the destination
 * @tc.level     : Level 0
 */
void wmemmove_0100(void)
{
    wchar_t dest[] = L"This is a c test for wmemmove function";
    const wchar_t src[] = L"src content";
    int count = 5;
    wchar_t tmp[count + 1];
    wcsncpy(tmp, src, count);
    wmemmove(dest, src, count);
    if (wcsncmp(dest, tmp, count)) {
        t_error("The dest specified bits are not equal to the src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0200
 * @tc.desc      : Dest and src overlaps
 * @tc.level     : Level 1
 */
void wmemmove_0200(void)
{
    wchar_t src[] = L"This is a c test for wmemmove function";
    wchar_t *dest = &src[2];
    int count = 5;
    wchar_t tmp[count + 1];
    wcsncpy(tmp, src, count);
    wmemmove(dest, src, count);
    if (wcsncmp(dest, tmp, count)) {
        t_error("The dest specified bits are not equal to the src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0300
 * @tc.desc      : The first address of dest is the same as src
 * @tc.level     : Level 1
 */
void wmemmove_0300(void)
{
    wchar_t src[] = L"This is a c test for wmemmove function";
    wchar_t *dest = &src[0];
    int count = 5;
    wchar_t *result = wmemmove(dest, src, count);
    if (!(result == dest)) {
        t_error("The first address of dest is not the same as src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0400
 * @tc.desc      : Copies entire string from source to destination
 * @tc.level     : Level 0
 */
void wmemmove_0400(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"Complete wmemmove test";
    size_t n = wcslen(src);
    wmemmove(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0500
 * @tc.desc      : Copies zero wide characters (n = 0)
 * @tc.level     : Level 1
 */
void wmemmove_0500(void)
{
    wchar_t dest[20] = L"Original content";
    wchar_t src[] = L"Should not be copied";
    wchar_t original[20];
    wcscpy(original, dest);
    wmemmove(dest, src, 0);
    if (wcscmp(dest, original)) {
        t_error("%s Destination was modified when n=0\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0600
 * @tc.desc      : Source and destination are the same pointer
 * @tc.level     : Level 1
 */
void wmemmove_0600(void)
{
    wchar_t buf[30] = L"Same pointer test";
    wchar_t *src = buf;
    wchar_t *dest = buf;
    size_t n = 5;
    wchar_t original[30];
    wcscpy(original, buf);
    wmemmove(dest, src, n);
    if (wcscmp(buf, original)) {
        t_error("%s Buffer changed when src and dest are same\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0700
 * @tc.desc      : Copies wide characters including non-ASCII characters
 * @tc.level     : Level 0
 */
void wmemmove_0700(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"中文测试 こんにちは Hello";
    size_t n = 6;
    wmemmove(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s Wide character copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0800
 * @tc.desc      : Verifies return value is the destination pointer
 * @tc.level     : Level 1
 */
void wmemmove_0800(void)
{
    wchar_t dest[20];
    wchar_t src[] = L"Test return value";
    size_t n = wcslen(src);
    wchar_t *ret = wmemmove(dest, src, n);
    if (ret != dest) {
        t_error("%s Return value is not destination pointer\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0900
 * @tc.desc      : Handles exact buffer size copy
 * @tc.level     : Level 1
 */
void wmemmove_0900(void)
{
    wchar_t src[] = L"Exact";
    wchar_t dest[6]; // Exactly fits the string + null terminator
    size_t n = wcslen(src);
    wmemmove(dest, src, n);
    dest[n] = L'\0';
    if (wcscmp(dest, src)) {
        t_error("%s Exact buffer copy failed\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_1000
 * @tc.desc      : Handles overlapping memory when source is before destination
 * @tc.level     : Level 1
 */
void wmemmove_1000(void)
{
    wchar_t buf[50] = L"abcdefghijklmnopqrstuvwxyz";
    wchar_t *src = buf;
    wchar_t *dest = buf + 5;
    size_t n = 10;
    wmemmove(dest, src, n);
    wchar_t expected[] = L"abcdeabcdefghijpqrstuvwxyz";
    if (wcscmp(buf, expected)) {
        t_error("%s Forward overlapping copy failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemmove_0100();
    wmemmove_0200();
    wmemmove_0300();
    wmemmove_0400();
    wmemmove_0500();
    wmemmove_0600();
    wmemmove_0700();
    wmemmove_0800();
    wmemmove_0900();
    wmemmove_1000();
    return t_status;
}