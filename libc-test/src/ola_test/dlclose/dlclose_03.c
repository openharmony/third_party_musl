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
 * @Casename: dlclose_03
 * @Brief: test dlcose in multi level n-to-1 scenario
 */

#include "common.h"

// Func: init environment
// Args: same as testcase
int tc_pre_test(int argc, char **argv)
{
    // @PRE-1
    char *files[] = {
        "/data/tests/libc-test/ola_test/libd.so",
        "/data/tests/libc-test/ola_test/libe.so",
        "/data/tests/libc-test/ola_test/libf.so",
        "/data/tests/libc-test/ola_test/libg.so",
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
    // @STEP-1: dlopen libf.so
    // @EXPECT-1: load libd.so, libe.so success
    int ret = 0;
    char *direct_libs[] = {
        "/data/tests/libc-test/ola_test/libf.so",
        "/data/tests/libc-test/ola_test/libg.so",
    };
    char *depend_libs[] = {
        "/data/tests/libc-test/ola_test/libd.so",
        "/data/tests/libc-test/ola_test/libe.so",
    };
    void *direct_hdls[] = { NULL, NULL };
    direct_hdls[0] = dlopen(direct_libs[0], RTLD_NOW | RTLD_GLOBAL);
    if (!direct_hdls[0]) {
        rt_err("dlopen %s failed: %s\n", direct_libs[0], dlerror());
    }

    if (!check_loaded(depend_libs[0]) || !check_loaded(depend_libs[1])) {
        rt_err("dso %s or %s should exist in /proc/self/maps\n", depend_libs[0], depend_libs[1]);
    }

    // @SETP-2: dlopen libg.so
    // @EXPECT-2: load libf.so success
    direct_hdls[1] = dlopen(direct_libs[1], RTLD_NOW | RTLD_GLOBAL);
    if (!direct_hdls[1]) {
        rt_err("dlopen %s failed: %s\n", direct_libs[1], dlerror());
    }

    // @STEP-3: dlclose libf.so
    // @EXPECT-3: libd.so libe.so exist
    ret = dlclose(direct_hdls[0]);
    if (ret != 0) {
        rt_err("dlclose %s failed: %s\n", direct_libs[0], dlerror());
    }

    if (!check_loaded(depend_libs[0]) || !check_loaded(depend_libs[1])) {
        rt_err("dso %s or %s should exist in /proc/self/maps\n", depend_libs[0], depend_libs[1]);
    }

    // @STEP-4: dlclose libg.so
    // @EXPECT-4: libd.so libe.so unloaded
    ret = dlclose(direct_hdls[1]);
    if (ret != 0) {
        rt_err("dlclose %s failed: %s\n", direct_libs[1], dlerror());
    }

    if (check_loaded(depend_libs[0]) || check_loaded(depend_libs[1])) {
        rt_err("dso %s or %s should exist in /proc/self/maps\n", depend_libs[0], depend_libs[1]);
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