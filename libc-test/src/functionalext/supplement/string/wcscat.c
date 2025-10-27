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

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

#define MAX (20)

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : wcscat_0100
 * @tc.desc      : Concatenate wide strings
 * @tc.level     : Level 0
 */
void wcscat_0100(void)
{
    wchar_t dest[MAX] = L"wcscat ";
    wchar_t src[MAX] = L"test";
    wcscat(dest, src);
    if (wcscmp(dest, L"wcscat test")) {
        t_error("%s concatenate wide strings failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0200
 * @tc.desc      : Concatenate empty source string
 * @tc.level     : Level 1
 */
void wcscat_0200(void)
{
    wchar_t dest[MAX] = L"Original";
    wchar_t src[MAX] = L"";
    wcscat(dest, src);
    if (wcscmp(dest, L"Original") != 0) {
        t_error("%s concatenate empty source failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0300
 * @tc.desc      : Concatenate to empty destination
 * @tc.level     : Level 1
 */
void wcscat_0300(void)
{
    wchar_t dest[MAX] = L"";
    wchar_t src[MAX] = L"Source";
    wcscat(dest, src);
    if (wcscmp(dest, L"Source") != 0) {
        t_error("%s concatenate to empty destination failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0400
 * @tc.desc      : Concatenate multiple times
 * @tc.level     : Level 1
 */
void wcscat_0400(void)
{
    wchar_t dest[MAX] = L"Start";
    wcscat(dest, L" Middle");
    wcscat(dest, L" End");
    if (wcscmp(dest, L"Start Middle End") != 0) {
        t_error("%s multiple concatenations failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0500
 * @tc.desc      : Concatenate wide characters with special symbols
 * @tc.level     : Level 1
 */
void wcscat_0500(void)
{
    wchar_t dest[MAX] = L"Text ";
    wchar_t src[MAX] = L"!@#$%";
    wcscat(dest, src);
    if (wcscmp(dest, L"Text !@#$%") != 0) {
        t_error("%s concatenate special symbols failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0600
 * @tc.desc      : Return value verification
 * @tc.level     : Level 1
 */
void wcscat_0600(void)
{
    wchar_t dest[MAX] = L"Return ";
    wchar_t src[MAX] = L"Test";
    wchar_t* result = wcscat(dest, src);
    if (result != dest || wcscmp(result, L"Return Test") != 0) {
        t_error("%s return value verification failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0700
 * @tc.desc      : Concatenate unicode characters
 * @tc.level     : Level 2
 */
void wcscat_0700(void)
{
    wchar_t dest[MAX] = L"Unicode ";
    wchar_t src[MAX] = L"测试";
    wcscat(dest, src);
    if (wcscmp(dest, L"Unicode 测试") != 0) {
        t_error("%s concatenate unicode characters failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0800
 * @tc.desc      : destination and source overlap.
 * @tc.level     : Level 2
 */
void wcscat_0800(void)
{
    signal(SIGSEGV, handler);

    wchar_t dest[MAX] = L"wcscat ";
    wchar_t *src = &dest[1];
    wcscat(dest, src);
}

int main(int argc, char *argv[])
{
    wcscat_0100();
    wcscat_0200();
    wcscat_0300();
    wcscat_0400();
    wcscat_0500();
    wcscat_0600();
    wcscat_0700();
    wcscat_0800();
    return t_status;
}