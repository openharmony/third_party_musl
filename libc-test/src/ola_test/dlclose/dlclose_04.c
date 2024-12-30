/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**************
 * @Casename: dlclose_04
 * @Brief: test dlcose in multi-so scenario
 */

#include "common.h"

// Func: init environment
// Args: same as testcase
int tc_pre_test(int argc, char **argv)
{
    // @PRE-1
    char *files[] = {
        "/data/tests/libc-test/ola_test/liba.so",
        "/data/tests/libc-test/ola_test/libb.so",
        "/data/tests/libc-test/ola_test/libcc.so",
        "/data/tests/libc-test/ola_test/libi.so",
    };
    int num_files = sizeof(files) / sizeof(files[0]);
    if (check_so_exist(num_files, files) != 0) {
        rt_err("check so failed\n");
    }
    return RT_EOK;
}

// Func: do test
// Args: same as testcase
int tc_do_test(int argc, char **argv)
{
    // @STEP-1: dlopen libcc.so libi.so
    // @EXPECT-1: all so load success
    int ret = 0;
    char *direct_libs[] = {
        "/data/tests/libc-test/ola_test/libcc.so",
        "/data/tests/libc-test/ola_test/libi.so",
    };
    void *direct_hdls[] = { NULL, NULL };
    char *depend_libs[] = {
        "/data/tests/libc-test/ola_test/liba.so",
        "/data/tests/libc-test/ola_test/libb.so",
    };
    int num1 = sizeof(direct_libs) / sizeof(direct_libs[0]);
    int num2 = sizeof(depend_libs) / sizeof(depend_libs[0]);

    for (int i = 0; i < num1; i++) {
        direct_hdls[i] = dlopen(direct_libs[i], RTLD_NOW | RTLD_GLOBAL);
        if (!direct_hdls[i]) {
            rt_err("dlopen %s failed: %s\n", direct_libs[i], dlerror());
        }
        if (!check_loaded(direct_libs[i])) {
            rt_err("dso %s should exist in /proc/self/maps\n", direct_libs[i]);
        }
    }
    for (int j = 0; j < num2; j++) {
        if (!check_loaded(depend_libs[j])) {
            rt_err("dso %s should exist in /proc/self/maps\n", depend_libs[j]);
        }
    }

    // @SETP-2: dlclose libi.so
    // @EXPECT-2: liba.so and libb.so exist
    ret = dlclose(direct_hdls[0]);
    if (ret != 0) {
        rt_err("dlclose %s failed: %s\n", direct_libs[0], dlerror());
    }
    for (int j = 0; j < num2; j++) {
        if (!check_loaded(depend_libs[j])) {
            rt_err("dso %s should exist in /proc/self/maps\n", depend_libs[j]);
        }
    }

    // @STEP-3: dlclose libi.so
    // @EXPECT-3: liba.so libb.so unloaded
    ret = dlclose(direct_hdls[1]);
    if (ret != 0) {
        rt_err("dlclose %s failed: %s\n", direct_libs[1], dlerror());
    }
    for (int j = 0; j < num2; j++) {
        if (check_loaded(depend_libs[j])) {
            rt_err("dso %s shouldn't exist in /proc/self/maps\n", direct_libs[j]);
        }
    }

    return RT_EOK;
}

// Func: post test
// Args: same as testcase
int tc_post_test(int argc, char **argv)
{
    return RT_EOK;
}

// Func: collect log when testcase failed
int tc_get_debuginfo(int argc, char **argv)
{
    return RT_EOK;
}

int main() 
{
    if (tc_pre_test(0, NULL)) {
        rt_err("tc_pre_test failed:\n");
        return -1;
    }
    if (tc_do_test(0, NULL)) {
        rt_err("tc_do_test failed:\n");
        return -1;
    }
    if (tc_post_test(0, NULL)) {
        rt_err("tc_post_test failed:\n");
        return -1;
    }
    if (tc_get_debuginfo(0, NULL)) {
        rt_err("tc_get_debug_info failed:\n");
        return -1;
    }
    return 0;
}