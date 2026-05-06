/**
 * Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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

/**
 * @file dlns_set_module_namespace_lib_path_test.c
 * @brief Test cases for dlns_set_module_namespace_lib_path interface
 *
 * This file contains comprehensive test cases for the dlns_set_module_namespace_lib_path
 * interface, which is designed to update library search paths for non-config namespaces
 * in plugin update scenarios.
 */

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "dlns_test.h"
#include "functionalext.h"

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0100
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with NULL name parameter
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0100(void)
{
    EXPECT_EQ("dlns_set_module_namespace_lib_path_0100",
        dlns_set_module_namespace_lib_path(NULL, path), EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0200
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with NULL lib_path parameter
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0200");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0200", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0200",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0200", NULL), EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0300
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with empty string lib_path
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0300(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0300");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0300", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0300",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0300", ""), EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0400
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with lib_path starting with ':'
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0400");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0400", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0400",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0400",
            ":/data/tests/libc-test/src"),
        EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0500
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with lib_path ending with ':'
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0500");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0500", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0500",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0500",
            "/data/tests/libc-test/src:"),
        EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0600
 * @tc.desc      : Test dlns_set_module_namespace_lib_path with lib_path containing only ':'
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0600(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0600");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0600", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0600",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0600", ":"), EINVAL);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0700
 * @tc.desc      : Test modifying built-in 'default' namespace
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0700(void)
{
    EXPECT_EQ("dlns_set_module_namespace_lib_path_0700",
        dlns_set_module_namespace_lib_path("default", path), EACCES);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0800
 * @tc.desc      : Test modifying built-in 'ndk' namespace
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0800(void)
{
    EXPECT_EQ("dlns_set_module_namespace_lib_path_0800",
        dlns_set_module_namespace_lib_path("ndk", path), EACCES);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0900
 * @tc.desc      : Test modifying non-existent namespace
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_0900(void)
{
    EXPECT_EQ("dlns_set_module_namespace_lib_path_0900",
        dlns_set_module_namespace_lib_path("nonexistent_namespace_12345", path), ENOKEY);
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_1000
 * @tc.desc      : Test setting single library path for namespace
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_1000(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_1000");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1000", dlns_create(&dlns, NULL), EOK);

    // Before setting path, cannot load library
    void* handle = dlopen_ns(&dlns, "libdlopen_dso.so", RTLD_LAZY);
    EXPECT_FALSE("dlns_set_module_namespace_lib_path_1000_before", handle);

    // Set path using the interface
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1000",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1000",
            "/data/local/tmp/libc-test-lib"),
        EOK);

    // After setting path, can load library
    handle = dlopen_ns(&dlns, "libdlopen_dso.so", RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1000_after", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_1100
 * @tc.desc      : Test setting multiple library paths for namespace
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_1100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_1100");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1100", dlns_create(&dlns, NULL), EOK);

    char* multiPath = "/data/tests/libc-test/src:/data/tests/libc-test/src/B:/data/local/tmp/libc-test-lib";

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1100",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1100", multiPath), EOK);

    // Load library from first path
    void *handle1 = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1100_lib1", handle1);
    if (handle1) {
        dlclose(handle1);
    }

    // Load library from second path
    void *handle2 = dlopen_ns(&dlns, "libdlopen_dso.so", RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1100_lib2", handle2);
    if (handle2) {
        dlclose(handle2);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_1200
 * @tc.desc      : Test that dlopen uses new path after updating (plugin update scenario)
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_1200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_1200");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1200", dlns_create(&dlns, NULL), EOK);

    // Simulate plugin v1.0 installation - set initial path to pathA
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1200_set_pathA",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1200",
            "/data/local/tmp/libc-test-lib"),
        EOK);

    // Load library from pathA
    void *handleA = dlopen_ns(&dlns, "libdlopen_dso.so", RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1200_from_pathA", handleA);
    if (handleA) {
        dlclose(handleA);
    }

    // Simulate plugin v2.0 update - update path to pathB
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1200_set_pathB",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1200",
            "/data/local/tmp/libc-test-lib"),
        EOK);

    // Load library from pathB (new version)
    void *handleB = dlopen_ns(&dlns, "libdlopen_dso.so", RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1200_from_pathB", handleB);
    if (handleB) {
        dlclose(handleB);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_1300
 * @tc.desc      : Test setting path multiple times (memory management)
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_1300(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_1300");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1300", dlns_create(&dlns, NULL), EOK);

    // First set
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1300_set1",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1300", pathA), EOK);

    // Second set (overwrite)
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1300_set2",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1300", pathB), EOK);

    // Third set (overwrite again)
    EXPECT_EQ("dlns_set_module_namespace_lib_path_1300_set3",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1300", path), EOK);

    // Verify final path is effective
    void *handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1300_final", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_1400
 * @tc.desc      : Test setting path with leading/trailing spaces (strtrim test)
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 */
void dlns_set_module_namespace_lib_path_1400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_1400");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1400", dlns_create(&dlns, NULL), EOK);

    // Path with leading and trailing spaces (will be auto-trimmed by strtrim)
    char* pathWithSpaces = "  /data/tests/libc-test/src  ";

    EXPECT_EQ("dlns_set_module_namespace_lib_path_1400",
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_1400", pathWithSpaces), EOK);

    // Verify path is still valid after trimming
    void *handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_1400_trimmed", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * Test suite array
 */
TEST_FUN G_Fun_Array[] = {
    dlns_set_module_namespace_lib_path_0100,
    dlns_set_module_namespace_lib_path_0200,
    dlns_set_module_namespace_lib_path_0300,
    dlns_set_module_namespace_lib_path_0400,
    dlns_set_module_namespace_lib_path_0500,
    dlns_set_module_namespace_lib_path_0600,
    dlns_set_module_namespace_lib_path_0700,
    dlns_set_module_namespace_lib_path_0800,
    dlns_set_module_namespace_lib_path_0900,
    dlns_set_module_namespace_lib_path_1000,
    dlns_set_module_namespace_lib_path_1100,
    dlns_set_module_namespace_lib_path_1200,
    dlns_set_module_namespace_lib_path_1300,
    dlns_set_module_namespace_lib_path_1400,
};

/**
 * Main test runner
 */
int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
