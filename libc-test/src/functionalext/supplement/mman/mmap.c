/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
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
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int EOK = 0;
const int SUCCESS = 0;
const int FAILED = -1;
#define OFF_MASK ((-0x2000ULL << (8 * sizeof(long) - 1)) | (4096ULL - 1))

/**
 * @tc.name      : mmap_0100
 * @tc.desc      : The parameters are valid, prot is PROT_READ,
 *                 and flags is MAP_PRIVATE, which can map the file content into memory
 * @tc.level     : Level 0
 */
void mmap_0100(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!1");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    EXPECT_EQ("mmap_0100", number, 2);
    EXPECT_EQ("mmap_0100", back, SUCCESS);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_0100", flag);
    mm = munmap(start, statbuff.st_size);
    EXPECT_EQ("mmap_0100", mm, SUCCESS);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0200
 * @tc.desc      : The parameters are valid, prot is PROT_READ|PROT_WRITE,
 *                 and flags is MAP_SHARED, which can map the file content into memory
 * @tc.level     : Level 0
 */
void mmap_0200(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!2");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ("mmap_0200", number, 2);
    EXPECT_EQ("mmap_0200", back, SUCCESS);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_0200", flag);
    mm = munmap(start, statbuff.st_size);
    EXPECT_EQ("mmap_0200", mm, SUCCESS);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0300
 * @tc.desc      : The parameters are valid, prot is PROT_READ|PROT_WRITE, and flags is MAP_SHARED|MAP_ANON,
 *                 which can map the file content into memory
 * @tc.level     : Level 1
 */
void mmap_0300(void)
{
    void *start;
    int mm;
    start = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_0300", flag);
    mm = munmap(start, getpagesize());
    EXPECT_EQ("mmap_0300", mm, SUCCESS);
}

/**
 * @tc.name      : mmap_0400
 * @tc.desc      : The parameters are valid, prot is PROT_EXEC, and flags is MAP_LOCKED,
 *                 which can map the file content into memory
 * @tc.level     : Level 1
 */
void mmap_0400(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!4");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_EXEC, MAP_PRIVATE | MAP_LOCKED, fd, 0);
    EXPECT_EQ("mmap_0400", number, 2);
    EXPECT_EQ("mmap_0400", back, SUCCESS);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_0400", flag);
    mm = munmap(start, statbuff.st_size);
    EXPECT_EQ("mmap_0400", mm, SUCCESS);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0500
 * @tc.desc      : The parameters are valid, prot is PROT_READ|PROT_WRITE, and flags is MAP_PRIVATE|MAP_DENYWRITE,
 *                 which can map the file content into memory
 * @tc.level     : Level 1
 */
void mmap_0500(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!5");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_DENYWRITE, fd, 0);
    EXPECT_EQ("mmap_0500", number, 2);
    EXPECT_EQ("mmap_0500", back, SUCCESS);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_0500", flag);
    mm = munmap(start, statbuff.st_size);
    EXPECT_EQ("mmap_0500", mm, SUCCESS);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0600
 * @tc.desc      : The parameters are valid, prot is PROT_NONE, and flags is MAP_PRIVATE|MAP_FIXED,
 *                 which can map the file content into memory
 * @tc.level     : Level 2
 */
void mmap_0600(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDONLY);
    if (fd < 0) {
        printf("Error!6");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_NONE, MAP_PRIVATE | MAP_FIXED, -1, 0);
    EXPECT_EQ("mmap_0600", number, 2);
    EXPECT_EQ("mmap_0600", back, SUCCESS);
    EXPECT_EQ("mmap_0600", start, MAP_FAILED);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0700
 * @tc.desc      : The parameter is invalid, the off parameter is OFF_MASK,
 *                 the file content cannot be mapped into memory
 * @tc.level     : Level 2
 */
void mmap_0700(void)
{
    int fd;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!7");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, OFF_MASK);
    EXPECT_EQ("mmap_0700", number, 2);
    EXPECT_EQ("mmap_0700", back, SUCCESS);
    EXPECT_EQ("mmap_0700", start, MAP_FAILED);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0800
 * @tc.desc      : The parameter is invalid, the len parameter exceeds the maximum value,
 *                 and the file content cannot be mapped into memory
 * @tc.level     : Level 2
 */
void mmap_0800(void)
{
    int fd;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!8");
        exit(1);
    }
    start = mmap(NULL, PTRDIFF_MAX + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ("mmap_0800", number, 2);
    EXPECT_EQ("mmap_0800", start, MAP_FAILED);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_0900
 * @tc.desc      : The parameter is invalid, the len parameter is 0, the file content cannot be mapped into memory
 * @tc.level     : Level 2
 */
void mmap_0900(void)
{
    int fd;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!9");
        exit(1);
    }
    start = mmap(NULL, 0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_EQ("mmap_0900", number, 2);
    EXPECT_EQ("mmap_0900", start, MAP_FAILED);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_1000
 * @tc.desc      : The parameter is invalid, the flags parameter is MAP_SHARED|MAP_ANON, and the off parameter is
 *                 not an integer multiple of the page, the file content cannot be mapped into the memory
 * @tc.level     : Level 2
 */
void mmap_1000(void)
{
    int fd;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!10");
        exit(1);
    }
    start = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, fd, getpagesize() - 1);
    EXPECT_EQ("mmap_1000", number, 2);
    EXPECT_EQ("mmap_1000", back, SUCCESS);
    EXPECT_EQ("mmap_1000", start, MAP_FAILED);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

/**
 * @tc.name      : mmap_1100
 * @tc.desc      : The parameter is invalid, start is not an integer multiple of the page,
 *                 and the file content cannot be mapped into memory
 * @tc.level     : Level 1
 */
void mmap_1100(void)
{
    int fd;
    int mm;
    void *start;
    const char *ptr = "/data/test.txt";
    static char str[] = "this is a sample!";
    int number = 0;
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    struct stat sb;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    if (fgetc(fptr) == EOF) {
        number = 1;
    } else {
        number = 2;
    }
    int back = stat(ptr, &statbuff);
    fclose(fptr);
    fd = open("/data/test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        printf("Error!11");
        exit(1);
    }
    int get = getpagesize() - 1;
    start = mmap((void *)(&get), statbuff.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_DENYWRITE, fd, 0);
    EXPECT_EQ("mmap_1100", number, 2);
    EXPECT_EQ("mmap_1100", back, SUCCESS);
    bool flag = false;
    if (start != MAP_FAILED) {
        flag = true;
    }
    EXPECT_TRUE("mmap_1100", flag);
    mm = munmap(start, statbuff.st_size);
    EXPECT_EQ("mmap_1100", mm, SUCCESS);
    remove(ptr);
    remove("/data/test.txt");
    fptr = NULL;
    ptr = NULL;
    close(fd);
}

int main(int argc, char *argv[])
{
    mmap_0100();
    mmap_0200();
    mmap_0300();
    mmap_0400();
    mmap_0500();
    mmap_0600();
    mmap_0700();
    mmap_0800();
    mmap_0900();
    mmap_1000();
    mmap_1100();

    return t_status;
}
