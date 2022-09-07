/*
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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "functionalext.h"

#define LEN 8192
typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : remap_file_pages_0100
 * @tc.desc      : Verify that a nonlinear mapping file can be created (parameters are valid)
 * @tc.level     : Level 0
 */
void remap_file_pages_0100(void)
{
    int i, fd, ret;
    char str[LEN] = {0};
    char *start = NULL;
    fd = open("text", O_RDWR | O_CREAT);
    start = mmap(NULL, LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ret = remap_file_pages(start, LEN >> 1, 0, 1, MAP_SHARED);
    if (ret < 0) {
        perror("remap file pages failed1");
    }
    EXPECT_EQ("remap_file_pages_0100", ret, 0);
    close(fd);
    remove("text");
}

TEST_FUN G_Fun_Array[] = {
    // remap_file_pages_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}