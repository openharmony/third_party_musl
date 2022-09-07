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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;

/*
 * @tc.name      : munmap_0100
 * @tc.desc      : Verify that the parameters are valid to unmap the memory
 * @tc.level     : Level 0
 */
void munmap_0100(void)
{
    const char *ptr = "/data/test.txt";
    const char str[] = "this is a sample!";
    FILE *fptr = fopen(ptr, "w+");
    char *p_map;
    struct stat statbuff;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    p_map = mmap(NULL, sizeof(char) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fptr), 0);
    fclose(fptr);
    int data = munmap(p_map, sizeof(char) * 10);
    EXPECT_EQ("munmap_0100", data, COUNT_ZERO);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    munmap_0100,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}