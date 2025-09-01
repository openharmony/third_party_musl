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
#include "test.h"

/**
 * @tc.name      : bzero_0100
 * @tc.desc      : Empty the specified number of elements in the string
 * @tc.level     : Level 0
 */
void bzero_0100(void)
{
    char str[20] = "bzero";
    bzero(str, 1);
    if (str[0] != 0) {
        t_error("%s bzero failed", __func__);
    }
}

/**
 * @tc.name      : bzero_0200
 * @tc.desc      : set '\0' to 0
 * @tc.level     : Level 1
 */
void bzero_0200(void)
{
    char str[20] = "bzero";
    int len = strlen(str);
    bzero(str, sizeof(str));
    if (str[len] != 0) {
        t_error("%s bzero failed", __func__);
    }
}

/**
 * @tc.name      : bzero_0300
 * @tc.desc      : Verify that bzero clears partial content of a string
 * @tc.level     : Level 1
 */
void bzero_0300(void)
{
    char str[20] = "hello world";
    bzero(str + 6, 5);  // Clear "world"
    if (str[0] != 'h' || str[6] != 0) {
        t_error("%s bzero failed", __func__);
    }
}

/**
 * @tc.name      : bzero_0600
 * @tc.desc      : Verify that bzero works correctly on the maximum buffer size
 * @tc.level     : Level 2
 */
void bzero_0400(void)
{
    #define MAX_SIZE 1024
    char buffer[MAX_SIZE];
    memset(buffer, 0xFF, MAX_SIZE);
    bzero(buffer, MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        if (buffer[i] != 0) {
            t_error("%s bzero failed at position %d", __func__, i);
        }
    }
}

int main(int argc, char *argv[])
{
    bzero_0100();
    bzero_0200();
    bzero_0300();
    bzero_0400();
    return t_status;
}