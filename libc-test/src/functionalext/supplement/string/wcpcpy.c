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
 * @tc.name      : wcpcpy_0100
 * @tc.desc      : Test the wcpcpy method to copy a wide string in bits
 * @tc.level     : Level 0
 */
void wcpcpy_0100(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dst[14];
    wchar_t *result = wcpcpy(dst, src);
    if (wcscmp(result, dst + 13) != 0) {
        t_error("%s wcpcpy get result is %ls are not want %ls\n", __func__, result, dst + 13);
    }
    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpcpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

/**
 * @tc.name      : wcpcpy_0200
 * @tc.desc      : Copy empty wide string
 * @tc.level     : Level 0
 */
void wcpcpy_0200(void)
{
    wchar_t src[] = L"";
    wchar_t dst[1];
    wchar_t *result = wcpcpy(dst, src);

    if (result != dst) {
        t_error("%s wcpcpy get result is %p are not want %p\n", __func__, result, dst);
    }

    if (dst[0] != L'\0') {
        t_error("%s wcpcpy get dst is not empty string\n", __func__);
    }
}

/**
 * @tc.name      : wcpcpy_0300
 * @tc.desc      : Copy single character wide string
 * @tc.level     : Level 0
 */
void wcpcpy_0300(void)
{
    wchar_t src[] = L"A";
    wchar_t dst[2];
    wchar_t *result = wcpcpy(dst, src);

    if (wcscmp(result, dst + 1) != 0) {
        t_error("%s wcpcpy get result is %ls are not want %ls\n", __func__, result, dst + 1);
    }

    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpcpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

/**
 * @tc.name      : wcpcpy_0400
 * @tc.desc      : Copy wide string with special characters
 * @tc.level     : Level 1
 */
void wcpcpy_0400(void)
{
    wchar_t src[] = L"Test\nString\t123";
    wchar_t dst[16];
    wchar_t *result = wcpcpy(dst, src);

    if (wcscmp(result, dst + 15) != 0) {
        t_error("%s wcpcpy get result is %ls are not want %ls\n", __func__, result, dst + 15);
    }

    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpcpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

/**
 * @tc.name      : wcpcpy_0500
 * @tc.desc      : Copy to exactly sized buffer
 * @tc.level     : Level 1
 */
void wcpcpy_0500(void)
{
    wchar_t src[] = L"Buffer";
    wchar_t dst[7];
    wchar_t *result = wcpcpy(dst, src);

    if (wcscmp(result, dst + 6) != 0) {
        t_error("%s wcpcpy get result is %ls are not want %ls\n", __func__, result, dst + 6);
    }

    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpcpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

int main(int argc, char *argv[])
{
    wcpcpy_0100();
    wcpcpy_0200();
    wcpcpy_0300();
    wcpcpy_0400();
    wcpcpy_0500();
    return t_status;
}