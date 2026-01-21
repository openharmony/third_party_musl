/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <unistd.h>
#include "functionalext.h"

/*
 * @tc.name      : __getauxval_0100
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SECURE)
 * @tc.level     : Level 0
 */
void __getauxval_0100(void)
{
    unsigned long int result;
    result = __getauxval(AT_SECURE);
    EXPECT_EQ("__getauxval_0100", result, 0);
}

/*
 * @tc.name      : __getauxval_0200
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO)
 * @tc.level     : Level 0
 */
void __getauxval_0200(void)
{
    unsigned long int result;
    result = __getauxval(AT_SYSINFO);
    EXPECT_EQ("__getauxval_0200", result, 0);
}

/*
 * @tc.name      : __getauxval_0300
 * @tc.desc      : Verify that the value can be retrieved from the auxiliary vector (parameter is AT_SYSINFO_EHDR)
 * @tc.level     : Level 0
 */
void __getauxval_0300(void)
{
    unsigned long int result;
    result = __getauxval(AT_SYSINFO_EHDR);
    EXPECT_TRUE("__getauxval_0300", result != 0);
}

/*
 * @tc.name      : __getauxval_0400
 * @tc.desc      : Verify that __getauxval returns 0 and sets errno for non-existent item
 * @tc.level     : Level 0
 */
void __getauxval_0400(void)
{
    unsigned long int result;
    errno = 0;
    result = __getauxval(0xFFFFFFFF);
    EXPECT_EQ("__getauxval_0400", result, 0);
    EXPECT_EQ("__getauxval_0400", errno, ENOENT);
}

/*
 * @tc.name      : __getauxval_0500
 * @tc.desc      : Verify boundary conditions
 * @tc.level     : Level 0
 */
void __getauxval_0500(void)
{
    unsigned long int result;
    errno = 0;
    result = __getauxval(0);
    EXPECT_EQ("__getauxval_0500", result, 0);
    EXPECT_EQ("__getauxval_0500", errno, ENOENT);
}

/*
 * @tc.name      : __getauxval_0600
 * @tc.desc      : Verify common AT tags (AT_PAGESZ/AT_HWCAP/AT_CLKTCK etc.)
 * @tc.level     : Level 0
 */
void __getauxval_0600(void)
{
    unsigned long pagesz = __getauxval(AT_PAGESZ);
    EXPECT_TRUE("__getauxval_0600_AT_PAGESZ", pagesz != 0);
    EXPECT_TRUE("__getauxval_0600_AT_PAGESZ_VALID", (pagesz == 4096) || (pagesz == 8192) || (pagesz == 16384));

    unsigned long clktck = __getauxval(AT_CLKTCK);
    EXPECT_TRUE("__getauxval_0600_AT_CLKTCK", clktck != 0);

    unsigned long phdr = __getauxval(AT_PHDR);
    unsigned long phent = __getauxval(AT_PHENT);
    unsigned long phnum = __getauxval(AT_PHNUM);
    EXPECT_TRUE("__getauxval_0600_AT_PHDR", phdr != 0);
    EXPECT_TRUE("__getauxval_0600_AT_PHENT", phent != 0);
    EXPECT_TRUE("__getauxval_0600_AT_PHNUM", phnum != 0);
}

/*
 * @tc.name      : __getauxval_0700
 * @tc.desc      : Verify extended invalid tags (AT_IGNORE/negative value)
 * @tc.level     : Level 0
 */
void __getauxval_0700(void)
{
    unsigned long int result;
    errno = 0;

    result = __getauxval(AT_IGNORE);
    EXPECT_EQ("__getauxval_0700_AT_IGNORE", result, 0);
    EXPECT_EQ("__getauxval_0700_AT_IGNORE_ERRNO", errno, ENOENT);

    errno = 0;
    result = __getauxval(-1);
    EXPECT_EQ("__getauxval_0700_NEGATIVE_TAG", result, 0);
    EXPECT_EQ("__getauxval_0700_NEGATIVE_TAG_ERRNO", errno, ENOENT);
}

/*
 * @tc.name      : __getauxval_0800
 * @tc.desc      : Verify AT_RANDOM (random seed) with deep check
 * @tc.level     : Level 0
 */
void __getauxval_0800(void)
{
    unsigned long random_val = __getauxval(AT_RANDOM);
    EXPECT_TRUE("__getauxval_0800_AT_RANDOM_PTR", random_val != 0);

    unsigned char *random_ptr = (unsigned char *)random_val;
    int valid = 1;
    for (int i = 0; i < 8; i++) {
        volatile unsigned char val = random_ptr[i];
        (void)val;
    }
    EXPECT_TRUE("__getauxval_0800_AT_RANDOM_ADDR_VALID", valid);
    int all_zero = 1;
    for (int i = 0; i < 8; i++) {
        if (random_ptr[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    EXPECT_FALSE("__getauxval_0800_AT_RANDOM_NOT_ALL_ZERO", all_zero);
}

void* mt_worker(void* arg)
{
    __getauxval_0100();
    return NULL;
}

/**
 * @tc.name      : __getauxval_0900
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __getauxval_0900(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ("__getauxval_0900", pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    __getauxval_0100();
    __getauxval_0200();
    __getauxval_0300();
    __getauxval_0400();
    __getauxval_0500();
    __getauxval_0600();
    __getauxval_0700();
    __getauxval_0800();
    __getauxval_0900();
    return t_status;
}