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

#include <malloc.h>
#include <functionalext.h>
#include <stdio.h>
#include <sys/mman.h>

#define TEST_MEM_SIZE 256
#define NO_JEMALLOC_ZONE 0
#define JEMALLOC_ZONE 1
#define UN_JEMALLOC (-1)
#define NUM_MALLOCS 256

#define EXPECT_EQ_ONE_OF(fun, val, exp1, exp2) do { \
    if (((val) != (exp1)) && ((val) != (exp2))) { \
        t_error("[%s] failed: %d != %d and %d != %d\n", \
                #fun, (int)(val), (int)(exp1), (int)(val), (int)(exp2)); \
    } \
} while (0)

/**
 * @tc.name      : malloc_check_from_ptr_0100
 * @tc.desc      : The memory block was allocated using malloc
 * @tc.level     : Level 0
 */
void malloc_check_from_ptr_0100(void)
{
    void *p = malloc(TEST_MEM_SIZE);
    EXPECT_PTRNE("malloc_check_from_ptr_0100", p, NULL);
    if (!p) {
        return;
    }
    int ret = malloc_check_from_ptr(p);
    EXPECT_EQ_ONE_OF("malloc_check_from_ptr_0100", ret, JEMALLOC_ZONE, UN_JEMALLOC);
    free(p);
}

/**
 * @tc.name      : malloc_check_from_ptr_0200
 * @tc.desc      : The memory block was allocated not using malloc
 * @tc.level     : Level 0
 */
void malloc_check_from_ptr_0200(void)
{
    void *p = mmap(NULL, TEST_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
    EXPECT_PTRNE("malloc_check_from_ptr_0200", p, NULL);
    if (!p) {
        return;
    }
    int ret = malloc_check_from_ptr(p);
    EXPECT_EQ_ONE_OF("malloc_check_from_ptr_0200", ret, NO_JEMALLOC_ZONE, UN_JEMALLOC);
    munmap(p, TEST_MEM_SIZE);
}

/**
 * @tc.name      : malloc_check_from_ptr_0300
 * @tc.desc      : The memory block was allocated using calloc
 * @tc.level     : Level 0
 */
void malloc_check_from_ptr_0300(void)
{
    void *p = calloc(1, TEST_MEM_SIZE);
    EXPECT_PTRNE("malloc_check_from_ptr_0300", p, NULL);
    if (!p) {
        return;
    }
    int ret = malloc_check_from_ptr(p);
    EXPECT_EQ_ONE_OF("malloc_check_from_ptr_0300", ret, JEMALLOC_ZONE, UN_JEMALLOC);
    free(p);
}

/**
 * @tc.name      : malloc_check_from_ptr_0400
 * @tc.desc      : multiple test malloc_check_from_ptr
 * @tc.level     : Level 0
 */
void malloc_check_from_ptr_0400(void)
{
    void *ptrs[NUM_ALLOCS];

    // multiple malloc
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        ptrs[i] = malloc(TEST_MEM_SIZE);
        if (!ptrs[i]) {
            return;
        }
    }

    // malloc_check_from_ptr in ptrs
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        int ret = malloc_check_from_ptr(ptrs[i]);
        EXPECT_EQ_ONE_OF("malloc_check_from_ptr_0400", ret, JEMALLOC_ZONE, UN_JEMALLOC);
    }

    // free all
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        free(ptrs[i]);
    }
}

int main(void)
{
    malloc_check_from_ptr_0100();
    malloc_check_from_ptr_0200();
    malloc_check_from_ptr_0300();
    malloc_check_from_ptr_0400();
    return t_status;
}