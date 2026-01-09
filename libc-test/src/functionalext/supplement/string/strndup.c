/*
 * Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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

#include <functionalext.h>
#include <stdlib.h>

/**
 * @tc.name      : strndup_0100
 * @tc.desc      : Test the strndup function to copy the string according to the input parameter
 * @tc.level     : Level 0
 */
void strndup_0100(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 2);
    if (strcmp(result, "he")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
    free(result);
}

/**
 * @tc.name      : strndup_0200
 * @tc.desc      : Test the return value of strndup when the number of copies of the input exceeds the passed string
 * @tc.level     : Level 1
 */
void strndup_0200(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 12);
    if (strcmp(result, "helloworld")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
    free(result);
}

/**
 * @tc.name      : strndup_0300
 * @tc.desc      : The return value of strndup when the number of test copies is 0
 * @tc.level     : Level 1
 */
void strndup_0300(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 0);
    if (strcmp(result, "") != 0) {
        t_error("%s strndup get result is '%s' are not ''\n", __func__, result);
    }
    free(result);
}

#ifdef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : __strndup_0100
 * @tc.desc      : The return value of __strndup when the number of test copies is different with the size of string
 * @tc.level     : Level 1
 */
void __strndup_0100(void)
{
    const char *src = "Hello, World!";
    // n is larger than the length of src
    char *result = __strndup(src, 20);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "Hello, World!");
    EXPECT_LT("__strndup_0100", strlen(result), 20);
    free(result);
    // n is less than the length of src
    result = __strndup(src, 5);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "Hello");
    EXPECT_LT("__strndup_0100", strlen(result), strlen(src));
    free(result);
    // n is equal to the length of src
    result = __strndup(src, strlen(src));
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, src);
    EXPECT_EQ("__strndup_0100", strlen(result), strlen(src));
    free(result);
    // n is 0
    const char *src1 = "Hello";
    result = __strndup(src, 0);
    EXPECT_PTRNE("__strndup_0100", result, NULL);
    EXPECT_STREQ("__strndup_0100", result, "");
    EXPECT_EQ("__strndup_0100", strlen(result), 0);
    free(result);
}

/**
 * @tc.name      : __strndup_0200
 * @tc.desc      : The return value of __strndup in different scenario as the following:
 *                 1. large n value (boundary test)
 *                 2. string containing special characters
 * @tc.level     : Level 1
 */
void __strndup_0200(void)
{    
    // large n value (boundary test)
    const char *str = "Short";
    char *result = __strndup(str, SIZE_MAX);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, "Short");
    EXPECT_LT("__strndup_0200", strlen(result), SIZE_MAX);
    free(result);
    // string containing special characters
    const char *src = "Line1\nLine2\tTab";
    result = __strndup(src, 12);
    EXPECT_PTRNE("__strndup_0200", result, NULL);
    EXPECT_STREQ("__strndup_0200", result, "Line1\nLine2\t");
    EXPECT_EQ("__strndup_0200", result[12], '\0');
    free(result);
}
#endif

int main(int argc, char *argv[])
{
    strndup_0100();
    strndup_0200();
    strndup_0300();
#ifdef MUSL_EXTERNAL_FUNCTION
    __strndup_0100();
    __strndup_0200();
#endif
    return t_status;
}