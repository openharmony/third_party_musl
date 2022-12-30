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

#include "test.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

#ifdef UNIT_TEST_STATIC
enum EnumHookMode;

int parse_hook_variable(enum EnumHookMode* mode, char* path, int size);
/**
 * @tc.name      : preinit_test_0010
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0010(void)
{
    int ret = parse_hook_variable(NULL, NULL, 0);
    EXPECT_EQ(preinit_test_0010, ret, -1);
}

bool get_proc_name(pid_t pid, char *buf, unsigned int buf_len);
/**
 * @tc.name      : preinit_test_0020
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0020(void)
{
    int ret = get_proc_name(0, NULL, 0);
    EXPECT_EQ(preinit_test_0020, ret, false);
}

void clear_function_table();
/**
 * @tc.name      : preinit_test_0030
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0030(void)
{
    clear_function_table();
    EXPECT_TRUE(preinit_test_0030, true);
}

void get_memleak_hook_param();
/**
 * @tc.name      : preinit_test_0040
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0040(void)
{
    get_memleak_hook_param();
    EXPECT_TRUE(preinit_test_0040, true);
}

void __restore_hook_function_table();
/**
 * @tc.name      : preinit_test_0050
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0050(void)
{
    __restore_hook_function_table();
    EXPECT_TRUE(preinit_test_0050, true);
}

void __install_malloc_hook();
/**
 * @tc.name      : preinit_test_0060
 * @tc.desc      :  test
 * @tc.level     : Level 2
 */
static void preinit_test_0060(void)
{
    __install_malloc_hook();
    EXPECT_TRUE(preinit_test_0060, true);
}

#endif

TEST_FUN G_Fun_Array[] = {
#ifdef UNIT_TEST_STATIC
    preinit_test_0010,
    preinit_test_0020,
    preinit_test_0030,
    preinit_test_0040,
    preinit_test_0050,
    preinit_test_0060,
#endif
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}