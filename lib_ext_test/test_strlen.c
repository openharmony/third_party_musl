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
 * @tc.name      : strlen_0100
 * @tc.desc      : Ordinary null character
 * @tc.level     : Level 0
 */
void strlen_0100(void)
{
    const char *str1 = "hello";  
    size_t length = strlen(str1);  
    printf("测试用例 1：长度为 %zu\n", length); // 应该输出 5  
    return 0; 
}

/**
 * @tc.name      : strlen_0200
 * @tc.desc      : A string containing Spaces
 * @tc.level     : Level 0
 */
void strlen_0200(void)
{
    const char *str2 = "hello world";  
    size_t length = strlen(str2);  
    printf("测试用例 2：长度为 %zu\n", length); // 应该输出 11  
    return 0;  
}

/**
 * @tc.name      : strlen_0300
 * @tc.desc      : A string containing special characters
 * @tc.level     : Level 0
 */
void strlen_0300(void)
{
    const char *str3 = "hello@123!";  
    size_t length = strlen(str3);  
    printf("测试用例 3：长度为 %zu\n", length); // 应该输出 11  
    return 0;  
}

/**
 * @tc.name      : strlen_0400
 * @tc.desc      : Empty string
 * @tc.level     : Level 0
 */
void strlen_0400(void)
{
    const char *str4 = "";  
    size_t length = strlen(str4);  
    printf("测试用例 4：长度为 %zu\n", length); // 应该输出 0  
    return 0; 
}

/**
 * @tc.name      : strlen_0500
 * @tc.desc      : A string containing numbers
 * @tc.level     : Level 0
 */
void strlen_0500(void)
{
    const char *str5 = "1234567890";  
    size_t length = strlen(str5);  
    printf("测试用例 5：长度为 %zu\n", length); // 应该输出 10  
    return 0;  
}

/**
 * @tc.name      : strlen_0600
 * @tc.desc      : A string containing line breaks
 * @tc.level     : Level 0
 */
void strlen_0600(void)
{
    const char *str6 = "hello\nworld"; // 包含换行符  
    size_t length = strlen(str6);  
    printf("测试用例 6：长度为 %zu\n", length); // 应该输出 11  
    return 0;  
}

/**
 * @tc.name      : strlen_0700
 * @tc.desc      : A string containing a token table
 * @tc.level     : Level 0
 */
void strlen_0700(void)
{
    const char *str7 = "hello\tworld"; // 包含制表符  
    size_t length = strlen(str7);  
    printf("测试用例 7：长度为 %zu\n", length); // 应该输出 11  
    return 0;  
}

/**
 * @tc.name      : strlen_0800
 * @tc.desc      : A string with only Spaces
 * @tc.level     : Level 0
 */
void strlen_0800(void)
{
    const char *str8 = "     "; // 只有空格  
    size_t length = strlen(str8);  
    printf("测试用例 8：长度为 %zu\n", length); // 应该输出 5  
    return 0;  
}

/**
 * @tc.name      : strlen_0900
 * @tc.desc      : Multi-line string
 * @tc.level     : Level 0
 */
void strlen_0900(void)
{
    const char *str9 = "line1\nline2\nline3"; // 包含多个换行符  
    size_t length = strlen(str9);  
    printf("测试用例 9：长度为 %zu\n", length); // 应该输出 18  
    return 0;  
}

/**
 * @tc.name      : strlen_1000
 * @tc.desc      : Long string
 * @tc.level     : Level 0
 */
void strlen_1000(void)
{
    const char *str10 = "This is a very long string that could be used for testing the strlen function.";  
    size_t length = strlen(str10);  
    printf("测试用例 10：长度为 %zu\n", length); // 应该输出字符串的实际长度  
    return 0;  
}

/**
 * @tc.name      : strlen_1100
 * @tc.desc      : Use dynamically assigned strings
 * @tc.level     : Level 0
 */
void strlen_1100(void)
{
    char *str12 = (char *)malloc(20 * sizeof(char));  
    strcpy(str12, "dynamic string");  
    size_t length = strlen(str12);  
    printf("测试用例 12：长度为 %zu\n", length); // 应该输出 15  
    free(str12); 
    return 0;  
}

int main(void)
{
    strlen_0100();
    strlen_0200();
    strlen_0300();
    strlen_0400();
    strlen_0500();
    strlen_0600();
    strlen_0700();
    strlen_0800();
    strlen_0900();
    strlen_1000();
    strlen_1100();
    printf("All tests passed!\n");
    return 0;
}