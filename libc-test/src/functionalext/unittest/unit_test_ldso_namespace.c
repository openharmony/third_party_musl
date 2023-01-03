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
#include "namespace.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

struct dso {
    char* name;
};

/**
 * @tc.name      : namespace_test_0010
 * @tc.desc      : ns_free test arg is null
 * @tc.level     : Level 2
 */
static void namespace_test_0010(void)
{
    ns_free(NULL);
    EXPECT_TRUE(namespace_test_0010, true);
}

/**
 * @tc.name      : namespace_test_0020
 * @tc.desc      : ns_free test normal condition
 * @tc.level     : Level 0
 */
static void namespace_test_0020(void)
{
    ns_t ns;
    int tmp = 1;
    ns.ns_dsos = (dsolist*)&tmp;
    ns.ns_name = (char*)&tmp;
    ns.env_paths = (char*)&tmp;
    ns.lib_paths = (char*)&tmp;
    ns.asan_lib_paths = (char*)&tmp;
    ns.permitted_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.allowed_libs = NULL;
    ns_inherit_list list;
    list.num = 1;
    ns_inherit inherit;
    inherit.shared_libs = NULL;
    ns_inherit* pInherit = &inherit;
    list.inherits = &pInherit;
    ns.ns_inherits = &list;

    ns_free(&ns);
    EXPECT_TRUE(namespace_test_0020, true);
}

/**
 * @tc.name      : namespace_test_0030
 * @tc.desc      : ns_free test arg ns's memebers are NULL 
 * @tc.level     : Level 2
 */
static void namespace_test_0030(void)
{
    ns_t ns;
    ns.ns_dsos = NULL;
    ns.ns_name = NULL;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.permitted_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.allowed_libs = NULL;
    ns.ns_inherits = NULL;

    ns_free(&ns);
    EXPECT_TRUE(namespace_test_0030, true);
}

/**
 * @tc.name      : namespace_test_0040
 * @tc.desc      : ns_add_dso test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0040(void)
{
    ns_add_dso(NULL, NULL);
    EXPECT_TRUE(namespace_test_0040, true);
}

/**
 * @tc.name      : namespace_test_0050
 * @tc.desc      : ns_add_dso test arg dso is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0050(void)
{
    ns_t ns;
    ns_add_dso(&ns, NULL);
    EXPECT_TRUE(namespace_test_0050, true);
}

/**
 * @tc.name      : namespace_test_0060
 * @tc.desc      : ns_add_dso test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0060(void)
{
    struct dso d;
    ns_add_dso(NULL, &d);
    EXPECT_TRUE(namespace_test_0060, true);
}

/**
 * @tc.name      : namespace_test_0070
 * @tc.desc      : ns_add_dso test normal condition
 * @tc.level     : Level 1
 */
static void namespace_test_0070(void)
{
    ns_t ns;
    struct dso d;
    dsolist list;
    list.num = 1;
    list.size = 1;
    ns.ns_dsos = &list;
    ns_add_dso(&ns, &d);
    EXPECT_EQ(namespace_test_0070, list.size, 2);
}

/**
 * @tc.name      : namespace_test_0080
 * @tc.desc      : nslist_add_ns test many times
 * @tc.level     : Level 1
 */
static void namespace_test_0080(void)
{
    ns_t ns;
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    nslist_add_ns(&ns);
    EXPECT_TRUE(namespace_test_0080, true);
}

/**
 * @tc.name      : namespace_test_0090
 * @tc.desc      : nslist_add_ns test arg is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0090(void)
{
    nslist_add_ns(NULL);
    EXPECT_TRUE(namespace_test_0090, true);
}

/**
 * @tc.name      : namespace_test_0100
 * @tc.desc      : ns_set_name test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0100(void)
{
    ns_set_name(NULL, NULL);
    EXPECT_TRUE(namespace_test_0100, true);
}

/**
 * @tc.name      : namespace_test_0110
 * @tc.desc      : ns_set_name test arg name is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0110(void)
{
    ns_t ns;
    ns_set_name(&ns, NULL);
    EXPECT_TRUE(namespace_test_0110, true);
}

/**
 * @tc.name      : namespace_test_0120
 * @tc.desc      : ns_set_name test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0120(void)
{
    ns_set_name(NULL, "abc");
    EXPECT_TRUE(namespace_test_0120, true);
}

/**
 * @tc.name      : namespace_test_0130
 * @tc.desc      : ns_set_env_paths test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0130(void)
{
    ns_set_env_paths(NULL, "abc");
    EXPECT_TRUE(namespace_test_0130, true);
}

/**
 * @tc.name      : namespace_test_0140
 * @tc.desc      : ns_set_env_paths test arg ns.env_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0140(void)
{
    ns_t ns;
    ns.env_paths = NULL;
    ns_set_env_paths(&ns, "abc");
    EXPECT_EQ(namespace_test_0140, strcmp(ns.env_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0150
 * @tc.desc      : ns_set_env_paths test arg ns.env_paths is equal to arg name
 * @tc.level     : Level 1
 */
static void namespace_test_0150(void)
{
    ns_t ns;
    ns.env_paths = "abc";
    ns_set_env_paths(&ns, "abc");
    EXPECT_EQ(namespace_test_0150, strcmp(ns.env_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0160
 * @tc.desc      : ns_set_env_paths test arg name is NULL and ns.env_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0160(void)
{
    ns_t ns;
    ns.env_paths = NULL;
    ns_set_env_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0160, ns.env_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0170
 * @tc.desc      : ns_set_env_paths test arg name is NULL and ns.env_paths is not NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0170(void)
{
    ns_t ns;
    ns.env_paths = "abc";
    ns_set_env_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0170, ns.env_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0180
 * @tc.desc      : ns_set_asan_lib_paths test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0180(void)
{
    ns_set_asan_lib_paths(NULL, "abc");
    EXPECT_TRUE(namespace_test_0180, true);
}

/**
 * @tc.name      : namespace_test_0190
 * @tc.desc      : ns_set_asan_lib_paths test arg ns.asan_lib_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0190(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;
    ns_set_asan_lib_paths(&ns, "abc");
    EXPECT_EQ(namespace_test_0190, strcmp(ns.asan_lib_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0200
 * @tc.desc      : ns_set_asan_lib_paths test arg ns.asan_lib_paths is equal to arg name
 * @tc.level     : Level 1
 */
static void namespace_test_0200(void)
{
    ns_t ns;
    ns.asan_lib_paths = "abc";
    ns_set_asan_lib_paths(&ns, "abc");
    EXPECT_EQ(namespace_test_0200, strcmp(ns.asan_lib_paths, "abc"), 0);
}

/**
 * @tc.name      : namespace_test_0210
 * @tc.desc      : ns_set_asan_lib_paths test arg name is NULL and ns.asan_lib_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0210(void)
{
    ns_t ns;
    ns.asan_lib_paths = NULL;
    ns_set_asan_lib_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0210, ns.asan_lib_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0220
 * @tc.desc      : ns_set_asan_lib_paths test arg name is NULL and ns.asan_lib_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0220(void)
{
    ns_t ns;
    ns.asan_lib_paths = "abc";
    ns_set_asan_lib_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0220, ns.asan_lib_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0230
 * @tc.desc      : ns_set_permitted_paths test arg permitted_paths is NULL and ns.permitted_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0230(void)
{
    ns_t ns;
    strlist list;
    ns.permitted_paths = &list;
    ns_set_permitted_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0230, ns.permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0240
 * @tc.desc      : ns_set_permitted_paths test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0240(void)
{
    ns_set_permitted_paths(NULL, NULL);
    EXPECT_TRUE(namespace_test_0240, true);
}

/**
 * @tc.name      : namespace_test_0250
 * @tc.desc      : ns_set_permitted_paths test arg permitted_paths is NULL and ns.permitted_paths is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0250(void)
{
    ns_t ns;
    ns.permitted_paths = NULL;
    ns_set_permitted_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0250, ns.permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0260
 * @tc.desc      : ns_set_asan_permitted_paths test arg asan_permitted_paths is NULL and ns.asan_permitted_paths is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0260(void)
{
    ns_t ns;
    strlist list;
    ns.asan_permitted_paths = &list;
    ns_set_asan_permitted_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0260, ns.asan_permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0270
 * @tc.desc      : ns_set_asan_permitted_paths test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0270(void)
{
    ns_set_asan_permitted_paths(NULL, NULL);
    EXPECT_TRUE(namespace_test_0270, true);
}

/**
 * @tc.name      : namespace_test_0280
 * @tc.desc      : ns_set_asan_permitted_paths test arg asan_permitted_paths is NULL and ns.asan_permitted_paths is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0280(void)
{
    ns_t ns;
    ns.asan_permitted_paths = NULL;
    ns_set_asan_permitted_paths(&ns, NULL);
    EXPECT_EQ(namespace_test_0280, ns.asan_permitted_paths, NULL);
}

/**
 * @tc.name      : namespace_test_0290
 * @tc.desc      : ns_set_separated test arg ns is NULL and separated is false
 * @tc.level     : Level 2
 */
static void namespace_test_0290(void)
{
    ns_set_separated(NULL, false);
    EXPECT_TRUE(namespace_test_0290, true);
}

/**
 * @tc.name      : namespace_test_0300
 * @tc.desc      : ns_set_allowed_libs test arg ns is NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0300(void)
{
    ns_set_allowed_libs(NULL, "abc");
    EXPECT_TRUE(namespace_test_0300, true);
}

/**
 * @tc.name      : namespace_test_0310
 * @tc.desc      : ns_set_allowed_libs test arg ns is ns.allowed_libs NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0310(void)
{
    ns_t ns;
    ns.allowed_libs = NULL;
    ns_set_allowed_libs(&ns, "abc");
    EXPECT_EQ(namespace_test_0310, (ns.allowed_libs != NULL), true);
}

/**
 * @tc.name      : namespace_test_0320
 * @tc.desc      : ns_set_allowed_libs test normal condition
 * @tc.level     : Level 0
 */
static void namespace_test_0320(void)
{
    ns_t ns;
    strlist list;
    ns.allowed_libs = &list;
    ns_set_allowed_libs(&ns, "abc");
    EXPECT_EQ(namespace_test_0320, (ns.allowed_libs != NULL), true);
}

/**
 * @tc.name      : namespace_test_0330
 * @tc.desc      : ns_set_allowed_libs test allowed_libs is NULL and ns.allowed_libs is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0330(void)
{
    ns_t ns;
    ns.allowed_libs = NULL;
    ns_set_allowed_libs(&ns, NULL);
    EXPECT_EQ(namespace_test_0330, ns.allowed_libs, NULL);
}

/**
 * @tc.name      : namespace_test_0400
 * @tc.desc      : ns_set_allowed_libs test allowed_libs is NULL and ns.allowed_libs is not NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0400(void)
{
    ns_t ns;
    strlist list;
    ns.allowed_libs = &list;
    ns_set_allowed_libs(&ns, NULL);
    EXPECT_EQ(namespace_test_0400, ns.allowed_libs, NULL);
}

/**
 * @tc.name      : namespace_test_0340
 * @tc.desc      : ns_add_inherit test arg ns_inherited and shared_libs are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0340(void)
{
    ns_t ns;
    ns_add_inherit(&ns, NULL, NULL);
    EXPECT_TRUE(namespace_test_0340, true);
}

/**
 * @tc.name      : namespace_test_0350
 * @tc.desc      : ns_add_inherit test args are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0350(void)
{

    ns_add_inherit(NULL, NULL, NULL);
    EXPECT_TRUE(namespace_test_0350, true);
}

/**
 * @tc.name      : namespace_test_0360
 * @tc.desc      : ns_add_inherit test arg ns and shared_libs are NULL
 * @tc.level     : Level 2
 */
static void namespace_test_0360(void)
{
    ns_t ns;
    ns_add_inherit(NULL, &ns, NULL);
    EXPECT_TRUE(namespace_test_0360, true);
}

/**
 * @tc.name      : namespace_test_0370
 * @tc.desc      : is_accessible test arg lib_pathname is NULL
 * @tc.level     : Level 1
 */
static void namespace_test_0370(void)
{
    ns_t ns;
    ns.separated = true;
    bool ret = is_accessible(&ns, NULL , true, true);
    EXPECT_EQ(namespace_test_0370, ret, false);
}

/**
 * @tc.name      : namespace_test_0380
 * @tc.desc      : is_accessible test args is (&ns, NULL, true, false) and ns.separated is false
 * @tc.level     : Level 1
 */
static void namespace_test_0380(void)
{
    ns_t ns;
    ns.separated = false;
    bool ret = is_accessible(&ns, NULL, true, false);
    EXPECT_EQ(namespace_test_0380, ret, false);
}

/**
 * @tc.name      : namespace_test_0390
 * @tc.desc      : is_accessible test args is (&ns, "/data/test.so", true, false) and ns.separated is false
 * @tc.level     : Level 1
 */
static void namespace_test_0390(void)
{
    ns_t ns;
    ns.separated = false;
    strlist list;
    list.num = 1;
    char *name = "test.so";
    list.strs = &name;

    ns.allowed_libs = &list;
    ns.env_paths = NULL;
    ns.lib_paths = NULL;
    ns.asan_lib_paths = NULL;
    ns.asan_permitted_paths = NULL;
    ns.permitted_paths = NULL;
    bool ret = is_accessible(&ns, "/data/test.so", true, false);
    EXPECT_EQ(namespace_test_0390, ret, false);
}

/**
 * @tc.name      : namespace_test_0410
 * @tc.desc      : is_sharable test args are NULL
 * @tc.level     : Level 0
 */
static void namespace_test_0410(void)
{
    bool ret = is_sharable(NULL, NULL);
    EXPECT_EQ(namespace_test_0410, ret, true);    
}

TEST_FUN G_Fun_Array[] = {
    namespace_test_0010,
    namespace_test_0020,
    namespace_test_0030,
    namespace_test_0040,
    namespace_test_0050,
    namespace_test_0060,
    namespace_test_0070,
    namespace_test_0080,
    namespace_test_0090,
    namespace_test_0100,
    namespace_test_0110,
    namespace_test_0120,
    namespace_test_0130,
    namespace_test_0140,
    namespace_test_0150,
    namespace_test_0160,
    namespace_test_0170,
    namespace_test_0180,
    namespace_test_0190,
    namespace_test_0200,
    namespace_test_0210,
    namespace_test_0220,
    namespace_test_0230,
    namespace_test_0250,
    namespace_test_0260,
    namespace_test_0270,
    namespace_test_0280,
    namespace_test_0290,
    namespace_test_0300,
    namespace_test_0310,
    namespace_test_0320,
    namespace_test_0330,
    namespace_test_0340,
    namespace_test_0350,
    namespace_test_0360,
    namespace_test_0370,
    namespace_test_0380,
    namespace_test_0390,
    namespace_test_0400,
    namespace_test_0410,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
