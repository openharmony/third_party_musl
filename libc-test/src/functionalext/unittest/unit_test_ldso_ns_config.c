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
#include "ns_config.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : ns_config_0010
 * @tc.desc      : Set default error callback for configor
 * @tc.level     : Level 1
 */
static void ns_config_test_0010(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(NULL);
    configor_free();

    EXPECT_TRUE(ns_config_test_0010, true);
}

int errorback(const char* format, ...)
{
    return 0;
}

/**
 * @tc.name      : ns_config_0030
 * @tc.desc      : Set custom error callback for configor
 * @tc.level     : Level 0
 */
static void ns_config_test_0020(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    configor->set_error_callback(&errorback);
    configor_free();

    EXPECT_TRUE(ns_config_test_0020, true);
}

/**
 * @tc.name      : ns_config_0040
 * @tc.desc      : Test get_namespaces with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0040(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    strlist *ret = configor->get_namespaces();
    configor_free();
    EXPECT_EQ(ns_config_test_0040, ret, NULL);
}

/**
 * @tc.name      : ns_config_0050
 * @tc.desc      : Test get_namespaces with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0050(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    kvlist kv;
    kv.num = 1;
    char *key = "a";
    char *val = "b";
    kv.key = &key;
    kv.val = &val;
    configor->kvs = &kv;
    strlist *ret = configor->get_namespaces();
    configor_free();
    EXPECT_EQ(ns_config_test_0050, ret, NULL);
}

/**
 * @tc.name      : ns_config_0070
 * @tc.desc      : Test get_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0070(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_lib_paths(NULL);
    configor_free();
    EXPECT_EQ(ns_config_test_0070, ret, NULL);
}

/**
 * @tc.name      : ns_config_0080
 * @tc.desc      : Test get_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0080(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_lib_paths("test");
    configor_free();
    EXPECT_EQ(ns_config_test_0080, ret, NULL);
}

/**
 * @tc.name      : ns_config_0090
 * @tc.desc      : Test get_asan_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0090(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_lib_paths(NULL);
    configor_free();
    EXPECT_EQ(ns_config_test_0090, ret, NULL);
}

/**
 * @tc.name      : ns_config_0100
 * @tc.desc      : Test get_asan_lib_paths with invalid input
 * @tc.level     : Level 2
 */
static void ns_config_test_0100(void)
{
    ns_configor* configor = NULL;
    configor = configor_init();
    char* ret = configor->get_asan_lib_paths("test");
    configor_free();
    EXPECT_EQ(ns_config_test_0100, ret, NULL);
}

TEST_FUN G_Fun_Array[] = {
    ns_config_test_0010,
    ns_config_test_0020,
    ns_config_test_0040,
    ns_config_test_0050,
    ns_config_test_0070,
    ns_config_test_0080,
    ns_config_test_0090,
    ns_config_test_0100,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}