/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>  
#include <string.h>  

/**
 * @tc.name      : strcasecmp_0100
 * @tc.desc      : Exactly the same string
 * @tc.level     : Level 0
 */
void strcasecmp_0100(void)
{
    const char *str1 = "hello";  
    const char *str2 = "hello";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 1：%d\n", result); // 应该输出 0，因为两个字符串完全相同  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0200
 * @tc.desc      : Different strings
 * @tc.level     : Level 0
 */
void strcasecmp_0200(void)
{
    const char *str1 = "hello";  
    const char *str2 = "world";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 2：%d\n", result); // 应该输出一个负数，因为 "hello" < "world"  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0300
 * @tc.desc      : Strings of different cases
 * @tc.level     : Level 0
 */
void strcasecmp_0300(void)
{
    const char *str1 = "hello";  
    const char *str2 = "Hello";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 3：%d\n", result); // 应该输出 0，因为不区分大小写  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0400
 * @tc.desc      : Exactly the same string
 * @tc.level     : Level 0
 */
void strcasecmp_0400(void)
{
    const char *str1 = "hello";  
    const char *str2 = "helicopter";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 4：%d\n", result); // 应该输出一个负数，因为 "hello" < "helicopter"  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0500
 * @tc.desc      : Comparison between long characters and short characters
 * @tc.level     : Level 0
 */
void strcasecmp_0500(void)
{
    const char *str1 = "Goodbye";  
    const char *str2 = "goodbye";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 5：%d\n", result); // 应该输出 0，因为不区分大小写  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0600
 * @tc.desc      : Compare null characters
 * @tc.level     : Level 0
 */
void strcasecmp_0600(void)
{
    const char *str1 = "";  
    const char *str2 = "hello";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 6：%d\n", result); // 应该输出一个负数，因为空字符串小于 "hello"  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0700
 * @tc.desc      : The same characters but in different cases
 * @tc.level     : Level 0
 */
void strcasecmp_0700(void)
{
    const char *str1 = "abcDEF";  
    const char *str2 = "abcdef";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 7：%d\n", result); // 应该输出 0，因为不区分大小写  
    return 0;  
}

/**
 * @tc.name      : strcasecmp_0800
 * @tc.desc      : Contain special characters
 * @tc.level     : Level 0
 */
void strcasecmp_0800(void)
{
    const char *str1 = "HeLlo@123";  
    const char *str2 = "hello@123";  
    int result = strcasecmp(str1, str2);  
    printf("测试用例 8：%d\n", result); // 应该输出 0，因为不区分大小写  
    return 0;  
}

int main(void)
{
    strcasecmp_0100();
    strcasecmp_0200();
    strcasecmp_0300();
    strcasecmp_0400();
    strcasecmp_0500();
    strcasecmp_0600();
    strcasecmp_0700();
    strcasecmp_0800();
    printf("All tests passed!\n");
    return 0;
}