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

#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include "functionalext.h"
#include "test.h"

#define SAME_NAME_COUT_1 1
#define SAME_NAME_COUT_2 2
#define OOM_FILL 50000

static int header_handler(struct dl_phdr_info *info, size_t size, void *data)
{
    void *addrhead = (void *)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
    if ((strcmp(info->dlpi_name, "/data/libtest.so") == 0) && (addrhead != NULL)) {
        return 1;
    }
    return 0;
}

static int same_name_count = 0;

static int same_name_count_handler(struct dl_phdr_info *info, size_t size, void *data)
{
    void *addrhead = (void *)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
    if ((strcmp(info->dlpi_name, "/data/libtest.so") == 0) && (addrhead != NULL)) {
        same_name_count++;
    }
    return 0;
}

static int lock_recursive_handler(struct dl_phdr_info *info, size_t size, void *data)
{
    if (dl_iterate_phdr(header_handler, NULL)) {
        return 0;
    }
    return 1;
}

/**
 * @tc.name      : dl_iterate_phdr_0100
 * @tc.desc      : The parameter is valid, get the starting address of the dynamic library loaded in the memory.
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0100(void)
{
    void *handle = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0100", (int)ret, 1);

    dlclose(handle);
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0100", (int)ret, 0);
}

/**
 * @tc.name      : dl_iterate_phdr_0200
 * @tc.desc      : Call dlopen twice, dlclose twice, the target so cannot find when second dlclose call.
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0200(void)
{
    void *handle = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0200", (int)ret, 1);

    handle = dlopen("/data/libtest.so", RTLD_NOW);
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0200", (int)ret, 1);

    dlclose(handle);
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0200", (int)ret, 1);

    dlclose(handle);
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0200", (int)ret, 0);
}

/**
 * @tc.name      : dl_iterate_phdr_0300
 * @tc.desc      : Call dlopen twice, dlclose twice, so file use the same name but it is two different files.
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0300(void)
{
    void *handle1 = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    same_name_count = 0;
    ret = dl_iterate_phdr(same_name_count_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0300", same_name_count, SAME_NAME_COUT_1);

    system("rm -rf /data/libtest.so");
    system("cp /data/local/tmp/lib_for_dlopen.so /data/libtest.so");
    system("cp /data/tests/libc-test/src/lib_for_dlopen.so /data/libtest.so");

    void *handle2 = dlopen("/data/libtest.so", RTLD_NOW);
    same_name_count = 0;
    ret = dl_iterate_phdr(same_name_count_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0300", same_name_count, SAME_NAME_COUT_2);

    dlclose(handle1);
    same_name_count = 0;
    ret = dl_iterate_phdr(same_name_count_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0300", same_name_count, SAME_NAME_COUT_1);

    dlclose(handle2);
    same_name_count = 0;
    ret = dl_iterate_phdr(same_name_count_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0300", same_name_count, 0);
}

/**
 * @tc.name      : dl_iterate_phdr_0400
 * @tc.desc      : Test lock is RECURSIVE when call dl_iterate_phdr
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0400(void)
{
    void *handle = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    ret = dl_iterate_phdr(lock_recursive_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0400", (int)ret, 0);

    dlclose(handle);
}

/**
 * @tc.name      : dl_iterate_phdr_0500
 * @tc.desc      : Call dl_iterate_phdr when OOM happen.
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0500(void)
{
    void *handle = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0500", (int)ret, 1);
    // create oom start
    if (t_memfill() < 0) {
        EXPECT_TRUE("dl_iterate_phdr_0500", false);
    }
	errno = 0;
	if (malloc(OOM_FILL)) {
        EXPECT_TRUE("dl_iterate_phdr_0500", false);
    }
	if (errno != ENOMEM) {
        EXPECT_TRUE("dl_iterate_phdr_0500", false);
    }
    // create oom end
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0500", (int)ret, 1);
    dlclose(handle);
}

int main(int argc, char *argv[])
{
    system("cp /data/local/tmp/lib_for_dlopen.so /data/libtest.so");
    system("cp /data/tests/libc-test/src/lib_for_dlopen.so /data/libtest.so");
    dl_iterate_phdr_0100();
    dl_iterate_phdr_0200();
    dl_iterate_phdr_0300();
    system("rm -rf /data/libtest.so");
    exit(EXIT_SUCCESS);
    return t_status;
}