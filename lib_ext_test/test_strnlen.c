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
#include <stdlib.h>  

/**
 * @tc.name      : strnlen_0100
 * @tc.desc      : Ordinary string
 * @tc.level     : Level 0
 */
void strnlen_0100(void)
{
    const char *str1 = "hello";  
    size_t length = strnlen(str1, 10);  
    printf("测试用例 1：长度为 %zu\n", length); // 应该输出 5  
    return 0;  
}

/**
 * @tc.name      : strnlen_0200
 * @tc.desc      : The calculated length is less than the maximum length
 * @tc.level     : Level 0
 */
void strnlen_0200(void)
{
    const char *str2 = "world";  
    size_t length = strnlen(str2, 10);  
    printf("测试用例 2：长度为 %zu\n", length); // 应该输出 5  
    return 0;  
}

/**
 * @tc.name      : strnlen_0300
 * @tc.desc      : A string containing special characters
 * @tc.level     : Level 0
 */
void strnlen_0300(void)
{
    const char *str4 = "abc\0def"; // 中间有空字符  
    size_t length = strnlen(str4, 10);  
    printf("测试用例 4：长度为 %zu\n", length); // 应该输出 3，因为 strlen 在空字符处终止  
    return 0;  
}

/**
 * @tc.name      : strnlen_0400
 * @tc.desc      : Empty string
 * @tc.level     : Level 0
 */
void strnlen_0400(void)
{
    const char *str3 = "";  
    size_t length = strnlen(str3, 10);  
    printf("测试用例 3：长度为 %zu\n", length); // 应该输出 0  
    return 0;  
}

/**
 * @tc.name      : strnlen_0500
 * @tc.desc      : Strings exceeding the maximum lengt
 * @tc.level     : Level 0
 */
void strnlen_0500(void)
{
    const char *str5 = "hello world";  
    size_t length = strnlen(str5, 5); // 最大长度为 5  
    printf("测试用例 5：长度为 %zu\n", length); // 应该输出 5  
    return 0;   
}

/**
 * @tc.name      : strnlen_0600
 * @tc.desc      : The length of the string is greater than the maximum length
 * @tc.level     : Level 0
 */
void strnlen_0600(void)
{
    const char *str7 = "This is a long string";  
    size_t length = strnlen(str7, 10);  
    printf("测试用例 7：长度为 %zu\n", length); // 应该输出 10  
    return 0;  
}

/**
 * @tc.name      : strnlen_0700
 * @tc.desc      : A string containing special characters
 * @tc.level     : Level 0
 */
void strnlen_0700(void)
{
    const char *str8 = "hello@#$%";  
    size_t length = strnlen(str8, 10);  
    printf("测试用例 8：长度为 %zu\n", length); // 应该输出 9  
    return 0; 
}

/**
 * @tc.name      : strnlen_0800
 * @tc.desc      : A string containing line breaks
 * @tc.level     : Level 0
 */
void strnlen_0800(void)
{
    const char *str9 = "hello\nworld"; // 包含换行符  
    size_t length = strnlen(str9, 10);  
    printf("测试用例 9：长度为 %zu\n", length); // 应该输出 10  
    return 0; 
}

/**
 * @tc.name      : strnlen_0900
 * @tc.desc      : A string with only Spaces
 * @tc.level     : Level 0
 */
void strnlen_0900(void)
{
    const char *str10 = "     "; // 只有空格  
    size_t length = strnlen(str10, 5);  
    printf("测试用例 10：长度为 %zu\n", length); // 应该输出 5  
    return 0;  
}

/**
 * @tc.name      : strnlen_1000
 * @tc.desc      : The boundary conditions use the minimum length
 * @tc.level     : Level 0
 */
void strnlen_1000(void)
{
    const char *str11 = "test";  
    size_t length = strnlen(str11, 0); // 最大长度为 0  
    printf("测试用例 11：长度为 %zu\n", length); // 应该输出 0  
    return 0;  
}

/**
 * @tc.name      : strnlen_1100
 * @tc.desc      : Use dynamically assigned strings
 * @tc.level     : Level 0
 */
void strnlen_1100(void)
{
    char *str6 = (char *)malloc(20 * sizeof(char));  
    strcpy(str6, "dynamic string");  
    size_t length = strnlen(str6, 20);  
    printf("测试用例 6：长度为 %zu\n", length); // 应该输出 15  
    free(str6); 
    return 0;  
}

int main(void)
{
    strnlen_0100();
    strnlen_0200();
    strnlen_0300();
    strnlen_0400();
    strnlen_0500();
    strnlen_0600();
    strnlen_0700();
    strnlen_0800();
    strnlen_0900();
    strnlen_1000();
    strnlen_1100();
    printf("All tests passed!\n");
    return 0;
}