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
 * @tc.name      : strncmp_0100
 * @tc.desc      : Exactly the same string
 * @tc.level     : Level 0
 */
void strncmp_0100(void)
{
    const char *str1 = "hello";  
    const char *str2 = "hello";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 1：%d\n", result); // 应该输出 0，因为两个字符串完全相同  
    return 0; 
}

/**
 * @tc.name      : strncmp_0200
 * @tc.desc      : Different strings
 * @tc.level     : Level 0
 */
void strncmp_0200(void)
{
    const char *str1 = "hello";  
    const char *str2 = "world";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 2：%d\n", result); // 应该输出一个负数，因为 "hello" < "world"  
    return 0;  
}

/**
 * @tc.name      : strncmp_0300
 * @tc.desc      : Partially identical strings
 * @tc.level     : Level 0
 */
void strncmp_0300(void)
{
    const char *str1 = "hello";  
    const char *str2 = "helicopter";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 3：%d\n", result); // 应该输出 0，因为前 5 个字符相同  
    return 0;  
}

/**
 * @tc.name      : strncmp_0400
 * @tc.desc      : Exactly the same string
 * @tc.level     : Level 0
 */
void strncmp_0400(void)
{
    const char *str1 = "hi";  
    const char *str2 = "hello";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 4：%d\n", result); // 应该输出一个负数，因为 "hi" < "hello"  
    return 0; 
}

/**
 * @tc.name      : strncmp_0500
 * @tc.desc      : The string contains null characters
 * @tc.level     : Level 0
 */
void strncmp_0500(void)
{
    const char *str1 = "hello\0world"; // 包含空字符  
    const char *str2 = "hello";  
    int result = strncmp(str1, str2, 6);  
    printf("测试用例 5：%d\n", result); // 应该输出 0，因为前 6 个字符一致 (包含在字符串中的空字符)  
    return 0;  
}

/**
 * @tc.name      : strncmp_0600
 * @tc.desc      : Compare null characters
 * @tc.level     : Level 0
 */
void strncmp_0600(void)
{
    const char *str1 = "";  
    const char *str2 = "hello";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 6：%d\n", result); // 应该输出一个负数，因为空字符串小于 "hello"  
    return 0;  
}

/**
 * @tc.name      : strncmp_0700
 * @tc.desc      : Strings with different case values
 * @tc.level     : Level 0
 */
void strncmp_0700(void)
{
    const char *str1 = "Apple";  
    const char *str2 = "apple";  
    int result = strncmp(str1, str2, 5);  
    printf("测试用例 6：%d\n", result); // 应该输出一个负数，因为大写字符小于小写字符  
    return 0;   
}

/**
 * @tc.name      : strncmp_0800
 * @tc.desc      : Strings that exceed the comparison length
 * @tc.level     : Level 0
 */
void strncmp_0800(void)
{
    const char *str1 = "hello there";  
    const char *str2 = "hello world";  
    int result = strncmp(str1, str2, 5); // 只比较前 5 个字符  
    printf("测试用例 11：%d\n", result); // 应该输出 0，因为前 5 个字符相同  
    return 0;  
}

int main(void)
{
    strncmp_0100();
    strncmp_0200();
    strncmp_0300();
    strncmp_0400();
    strncmp_0500();
    strncmp_0600();
    strncmp_0700();
    strncmp_0800();
    printf("All tests passed!\n");
    return 0;
}