/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const size_t ERRORLEN = -1;

/**
 * @tc.name:      fprintf_0100
 * @tc.desc:      Verify fprintf and stdout process success.
 * @tc.level:     level 0.
 */
void fprintf_0100(void)
{
    char str[] = "This is a fprintf_0100";
    size_t len = fprintf(stdout, "%s", str);
    EXPECT_EQ("fprintf_0100", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0200
 * @tc.desc:      Verify fprintf and stderr process success.
 * @tc.level:     level 0.
 */
void fprintf_0200(void)
{
    char str[] = "This is a test fprintf_0200";
    size_t len = fprintf(stderr, "%s", str);
    EXPECT_EQ("fprintf_0200", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0300
 * @tc.desc:      Verify fprintf and fopen("w") process success.
 * @tc.level:     level 0.
 */
void fprintf_0300(void)
{
    char str[] = "这是一个测试";
    int len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("fprintf_0300", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0400
 * @tc.desc:      Verify fflush and fopen("r") and fclose and return -1.
 * @tc.level:     level 2.
 */
void fprintf_0400(void)
{
    char str[] = "这是一个测试";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "r");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("fprintf_0400", len, ERRORLEN);
    remove("tempory_testfprintf.txt");
}

/**
 * @tc.name:      fprintf_0500
 * @tc.desc:      Verify fprintf with multiple format specifiers.
 * @tc.level:     level 0.
 */
void fprintf_0500(void)
{
    char str[] = "Test";
    int num = 123;
    double d = 45.67;
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        len = fprintf(fptr, "String: %s, Int: %d, Double: %.2f", str, num, d);
        fclose(fptr);
    }
    int expected_len = strlen("String: Test, Int: 123, Double: 45.67");
    EXPECT_EQ("fprintf_0500", len, expected_len);
    (void)remove("tempory_testfprintf.txt");
}


/**
 * @tc.name:      fprintf_0600
 * @tc.desc:      Verify fprintf with append mode.
 * @tc.level:     level 0.
 */
void fprintf_0600(void)
{
    char str1[] = "First line\n";
    char str2[] = "Second line\n";
    
    // Write first line
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        fprintf(fptr, "%s", str1);
        fclose(fptr);
    }
    
    // Append second line
    fptr = fopen("tempory_testfprintf.txt", "a");
    size_t len = 0;
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str2);
        fclose(fptr);
    }
    
    EXPECT_EQ("fprintf_0600", len, strlen(str2));
    (void)remove("tempory_testfprintf.txt");
}

/**
 * @tc.name:      fprintf_0700
 * @tc.desc:      Verify fprintf with large string.
 * @tc.level:     level 0.
 */
void fprintf_0700(void)
{
    char large_str[1024];
    memset(large_str, 'A', sizeof(large_str)-1);
    large_str[sizeof(large_str)-1] = '\0';
    
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", large_str);
        fclose(fptr);
    }
    
    EXPECT_EQ("fprintf_0700", len, strlen(large_str));
    (void)remove("tempory_testfprintf.txt");
}

/**
 * @tc.name:      fprintf_0800
 * @tc.desc:      Verify fprintf with special characters.
 * @tc.level:     level 0.
 */
void fprintf_0800(void)
{
    char str[] = "Special chars: \t\n\r\\\"\'";
    size_t len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str);
        fclose(fptr);
    }
    
    EXPECT_EQ("fprintf_0800", len, strlen(str));
    (void)remove("tempory_testfprintf.txt");
}

int main(void)
{
    fprintf_0100();
    fprintf_0200();
    fprintf_0300();
    fprintf_0400();
    fprintf_0500();
    fprintf_0600();
    fprintf_0700();
    fprintf_0800();
    return t_status;
}
