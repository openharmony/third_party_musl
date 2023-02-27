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
#include <errno.h>
#include "test.h"
#include "dlfcn.h"
#include "namespace.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : dynlink_test_0010
 * @tc.desc      : Test dlns_set_namespace_lib_path with invalid input 
 * @tc.level     : Level 2
 */
static void dynlink_test_0010(void)
{
    int ret = dlns_set_namespace_lib_path(NULL, NULL);
    EXPECT_EQ(dynlink_test_0010, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0020
 * @tc.desc      : Test dlns_set_namespace_lib_path with invalid input 
 * @tc.level     : Level 2
 */
static void dynlink_test_0020(void)
{
    int ret = dlns_set_namespace_lib_path("abc", "abc");
    EXPECT_EQ(dynlink_test_0020, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0030
 * @tc.desc      : Set path for default lib use dlns_set_namespace_lib_path 
 * @tc.level     : Level 1
 */
static void dynlink_test_0030(void)
{
    int ret = dlns_set_namespace_lib_path("default", "/data");
    EXPECT_EQ(dynlink_test_0030, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0040
 * @tc.desc      : Test dlns_set_namespace_separated with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0040(void)
{
    int ret = dlns_set_namespace_separated(NULL, true);
    EXPECT_EQ(dynlink_test_0040, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0050
 * @tc.desc      : Test dlns_set_namespace_separated with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0050(void)
{
    int ret = dlns_set_namespace_separated("abc", true);
    EXPECT_EQ(dynlink_test_0050, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0060
 * @tc.desc      : Set separated for default lib use dlns_set_namespace_separated
 * @tc.level     : Level 0
 */
static void dynlink_test_0060(void)
{
    int ret = dlns_set_namespace_separated("default", true);
    EXPECT_EQ(dynlink_test_0060, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0070
 * @tc.desc      : Test dlns_set_namespace_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0070(void)
{
    int ret = dlns_set_namespace_permitted_paths(NULL, NULL);
    EXPECT_EQ(dynlink_test_0070, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0080
 * @tc.desc      : Test dlns_set_namespace_permitted_paths with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0080(void)
{
    int ret = dlns_set_namespace_permitted_paths("abc", "abc");
    EXPECT_EQ(dynlink_test_0080, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0090
 * @tc.desc      : Set path for default lib use dlns_set_namespace_permitted_paths
 * @tc.level     : Level 0
 */
static void dynlink_test_0090(void)
{
    int ret = dlns_set_namespace_permitted_paths("default", "/data");
    EXPECT_EQ(dynlink_test_0090, ret, 0);
}

/**
 * @tc.name      : dynlink_test_0100
 * @tc.desc      : Test dlns_set_namespace_allowed_libs with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0100(void)
{
    int ret = dlns_set_namespace_allowed_libs(NULL, NULL);
    EXPECT_EQ(dynlink_test_0100, ret, EINVAL);
}

/**
 * @tc.name      : dynlink_test_0110
 * @tc.desc      : Test dlns_set_namespace_allowed_libs with invalid input
 * @tc.level     : Level 2
 */
static void dynlink_test_0110(void)
{
    int ret = dlns_set_namespace_allowed_libs("abc", "abc");
    EXPECT_EQ(dynlink_test_0110, ret, ENOKEY);
}

/**
 * @tc.name      : dynlink_test_0120
 * @tc.desc      : Set allowed libs for default lib use dlns_set_namespace_allowed_libs
 * @tc.level     : Level 0
 */
static void dynlink_test_0120(void)
{
    int ret = dlns_set_namespace_allowed_libs("default", "/data");
    EXPECT_EQ(dynlink_test_0120, ret, 0);
}

TEST_FUN G_Fun_Array[] = {
    dynlink_test_0010,
    dynlink_test_0020,
    dynlink_test_0030,
    dynlink_test_0040,
    dynlink_test_0050,
    dynlink_test_0060,
    dynlink_test_0070,
    dynlink_test_0080,
    dynlink_test_0090,
    dynlink_test_0100,
    dynlink_test_0110,
    dynlink_test_0120,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}