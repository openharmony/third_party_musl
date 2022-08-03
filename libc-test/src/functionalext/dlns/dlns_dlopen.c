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
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "dlns_test.h"
#include "functionalext.h"


/**
 * @tc.name      : dlopen_0100
 * @tc.desc      : Enter a valid path, dlopen compatibility test
 * @tc.level     : Level 0
 */
void dlopen_0100(void)
{
    void* handle = dlopen(dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlopen_0100", handle);
    dlclose(handle);
}


/**
 * @tc.name      : dlopen_0200
 * @tc.desc      : Enter invalid path, dlopen compatibility test
 * @tc.level     : Level 2
 */
void dlopen_0200(void)
{
    void* handle = dlopen(errPath_ns, RTLD_LAZY);
    EXPECT_FALSE("dlopen_0200", handle);
}

/**
 * @tc.name      : dlns_init_0100
 * @tc.desc      : Use a string less than 255 bytes as the dlns_init parameter
 * @tc.level     : Level 1
 */
void dlns_init_0100(void)
{
    Dl_namespace dlns;
    char n[] = "dlns_init_0100";
    dlns_init(&dlns, n);
    EXPECT_EQ("dlns_init_0100", strcmp(dlns.name, n), 0);
}

/**
 * @tc.name      : dlns_init_0200
 * @tc.desc      : Use very long strings as dlns_init parameters
 * @tc.level     : Level 2
 */
void dlns_init_0200(void)
{
    Dl_namespace dlns;
    int size = 256;
    char n[] = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 \
                012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 \
                012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 \
                012345678901234567890123456789";
    int len = strlen(n);
    dlns_init(&dlns, n);
    EXPECT_TRUE("dlns_init_0200", size < len);

    int dlnslen = strlen(dlns.name);
    EXPECT_TRUE("dlns_init_0200", size > dlnslen);
}

/**
 * @tc.name      : dlns_create_0100
 * @tc.desc      : Using dlns_create, create a new namespace
 * @tc.level     : Level 1
 */
void dlns_create_0100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_create_0100");

    EXPECT_EQ("dlns_create_0100", dlns_create(&dlns, path), EOK);
}

/**
 * @tc.name      : dlns_create_0200
 * @tc.desc      : Using dlns_create, create a new namespace
 * @tc.level     : Level 2
 */
void dlns_create_0200(void)
{
    // ns_no_allowed_libs already exists
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    EXPECT_EQ("dlns_create_0200", dlns_create(&dlns, NULL), EEXIST);
}

/**
 * @tc.name      : dlns_create_0300
 * @tc.desc      : After creating a new namespace, check lib_path validity
 * @tc.level     : Level 1
 */
void dlns_create_0300(void)
{
    // ns_no_allowed_libs already exists
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_create_0300");

    EXPECT_EQ("dlns_create_0300", dlns_create(&dlns, path), EOK);

    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_create_0300", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_create_0400
 * @tc.desc      : Namespace creation when dlns=NULL
 * @tc.level     : Level 2
 */
void dlns_create_0400(void)
{
    EXPECT_EQ("dlns_create_0400", dlns_create(NULL, path), EINVAL);
}

/**
 * @tc.name      : dlns_create_0500
 * @tc.desc      : Namespace creation when lib_path=NULL
 * @tc.level     : Level 2
 */
void dlns_create_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_create_0500");

    EXPECT_EQ("dlns_create_0500", dlns_create(&dlns, NULL), EOK);
}


/**
 * @tc.name      : dlopen_ns_0100
 * @tc.desc      : dlopen_ns valid full path test
 * @tc.level     : Level 1
 */
void dlopen_ns_0100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlopen_ns_0100", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlopen_ns_0200
 * @tc.desc      : dlopen_ns invalid full path test
 * @tc.level     : Level 2
 */
void dlopen_ns_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void* handle = dlopen_ns(&dlns, errdllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlopen_ns_0200", handle);
}

/**
 * @tc.name      : dlopen_ns_0300
 * @tc.desc      : dlopen_ns valid short name test
 * @tc.level     : Level 1
 */
void dlopen_ns_0300(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlopen_ns_0300", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlopen_ns_0400
 * @tc.desc      : When dlns=NULL, call dlopen_ns to load the library
 * @tc.level     : Level 1
 */
void dlopen_ns_0400(void)
{
    void* handle = dlopen_ns(NULL, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlopen_ns_0400", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlopen_ns_0500
 * @tc.desc      : When file=NULL, call dlopen_ns to load the library
 * @tc.level     : Level 2
 */
void dlopen_ns_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void *handle = dlopen_ns(&dlns, NULL, RTLD_LAZY);
    EXPECT_TRUE("dlopen_ns_0500", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_get_0100
 * @tc.desc      : Gets an existing namespace handle.
 * @tc.level     : Level 1
 */
void dlns_get_0100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_get_0100");
    dlns_create(&dlns, NULL);
    dlns_get("dlns_get_0100", &dlns);
    EXPECT_EQ("dlns_get_0100", strcmp(dlns.name, "dlns_get_0100"), 0);
}

/**
 * @tc.name      : dlns_get_0200
 * @tc.desc      : Gets the current namespace handle when name=NULL.
 * @tc.level     : Level 1
 */
void dlns_get_0200(void)
{
    Dl_namespace dlns;
    dlns_get(NULL, &dlns);
    EXPECT_EQ("dlns_get_0200", strcmp(dlns.name, "default"), 0);
}

/**
 * @tc.name      : dlns_get_0300
 * @tc.desc      : Call dlns_get when dlns=NULL.
 * @tc.level     : Level 2
 */
void dlns_get_0300(void)
{
    dlns_get(NULL, NULL);
    EXPECT_EQ("dlns_get_0300", dlns_get(NULL, NULL), EINVAL);
}

/**
 * @tc.name      : dlns_get_0400
 * @tc.desc      : Gets a namespace handle that does not exist.
 * @tc.level     : Level 2
 */
void dlns_get_0400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_get_0400");
    EXPECT_EQ("dlns_get_0400", dlns_get("dlns_get_0400", &dlns), ENOKEY);
}

TEST_FUN G_Fun_Array[] = {
    dlopen_0100,
    dlopen_0200,
    dlns_init_0100,
    dlns_init_0200,
    dlns_create_0100,
    dlns_create_0200,
    dlns_create_0300,
    dlns_create_0400,
    dlns_create_0500,
    dlopen_ns_0100,
    dlopen_ns_0200,
    dlopen_ns_0300,
    dlopen_ns_0400,
    dlopen_ns_0500,
    dlns_get_0100,
    dlns_get_0200,
    dlns_get_0300,
    dlns_get_0400,
};

int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}