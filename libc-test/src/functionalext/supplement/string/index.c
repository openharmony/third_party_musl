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
 * @tc.name      : index_0100
 * @tc.desc      : Test the index method to get the position of the first occurrence of the target character
 * @tc.level     : Level 0
 */
void index_0100(void)
{
    char *str = "hello world";
    char *result = index(str, 'e');
    if (strcmp(str + 1, result) != 0) {
        t_error("%s index get result is %s are not  want %s\n", __func__, result, str + 1);
    }
}

/**
 * @tc.name      : index_0200
 * @tc.desc      : The result returned by the index function when the target character appears multiple times in the
 * test string
 * @tc.level     : Level 1
 */
void index_0200(void)
{
    int add_size = 2;
    char *str = "hello world";
    char *result = index(str, 'l');
    if (strcmp(str + add_size, result) != 0) {
        t_error("%s index get result is %s are not  want %s\n", __func__, result, str + add_size);
    }
}

/**
 * @tc.name      : index_0300
 * @tc.desc      : The result returned by the index function when the target character does not appear in the test
 * string
 * @tc.level     : Level 1
 */
void index_0300(void)
{
    char *str = "hello world";
    char *result = index(str, 'a');
    if (result) {
        t_error("%s index get result is %s are not  want ''\n", __func__, result);
    }
}

/**
 * @tc.name      : index_0400
 * @tc.desc      : Verify index finds null terminator character
 * @tc.level     : Level 1
 */
void index_0400(void)
{
    char *str = "hello";
    char *result = index(str, '\0');
    if (result != str + 5) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str + 5);
    }
}

/**
 * @tc.name      : index_0500
 * @tc.desc      : Verify index works with empty string
 * @tc.level     : Level 2
 */
void index_0500(void)
{
    char *str = "";
    char *result = index(str, 'h');
    if (result != NULL) {
        t_error("%s index get result is %s are not want NULL\n", __func__, result);
    }
}

/**
 * @tc.name      : index_0600
 * @tc.desc      : Verify index finds first character in string
 * @tc.level     : Level 1
 */
void index_0600(void)
{
    char *str = "hello world";
    char *result = index(str, 'h');
    if (result != str) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str);
    }
}

/**
 * @tc.name      : index_0700
 * @tc.desc      : Verify index finds last character in string
 * @tc.level     : Level 1
 */
void index_0700(void)
{
    char *str = "hello world";
    char *result = index(str, 'd');
    if (result != str + 10) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str + 10);
    }
}

/**
 * @tc.name      : index_0800
 * @tc.desc      : Verify index works with binary data containing null bytes
 * @tc.level     : Level 2
 */
void index_0800(void)
{
    char data[] = {'a', 'b', '\0', 'c', 'd'};
    char *result = index(data, 'c');
    if (result != NULL) { // Expected to return NULL
        t_error("%s index should stop at '\\0', but got %p\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    index_0100();
    index_0200();
    index_0300();
    index_0400();
    index_0500();
    index_0600();
    index_0700();
    index_0800();
    return t_status;
}