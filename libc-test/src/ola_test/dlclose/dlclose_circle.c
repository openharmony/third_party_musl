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
/*********************************************************************
 * @CaseName: dlclose_circle
 * @Brief: 测试dlclose卸载循环依赖动态库(DTS2023112112760)
 * Author: baoruyi
 *********************************************************************/
#include "common.h" /* PLEASE DON'T MODIFY THIS LINE */

// Func: 本用例执行测试前进行环境初始化
// Args: 同用例参数
int tc_pre_test(int argc, char **argv)
{
    // @PRE-1: 测试so存在
    char *files[] = {
        "/data/tests/libc-test/ola_test/liba2.so",
        "/data/tests/libc-test/ola_test/libb2.so",
        "/data/tests/libc-test/ola_test/libc2.so",
        "/data/tests/libc-test/ola_test/libd2.so",
        "/data/tests/libc-test/ola_test/libe2.so",
        "/data/tests/libc-test/ola_test/libf2.so"
    };
    int num_files = sizeof(files) / sizeof(files[0]);
    if (check_so_exist(num_files, files) != 0) {
        rt_err("check so failed\n");
    }
    return RT_EOK;
}

// Func: 用例执行测试活动，重要：本函数必须定义且函数名不可修改
// Args: 同用例参数
int tc_do_test(int argc, char **argv)
{
    // @STEP-1: dlopen加载liba2.so
    // @EXPECT-1: 5个依赖so加载成功
    int ret = 0;
    char *direct_libs = "/data/tests/libc-test/ola_test/liba2.so";
    char *depend_libs[] = {
        "/data/tests/libc-test/ola_test/libb2.so",
        "/data/tests/libc-test/ola_test/libc2.so",
        "/data/tests/libc-test/ola_test/libd2.so",
        "/data/tests/libc-test/ola_test/libe2.so",
        "/data/tests/libc-test/ola_test/libf2.so"
    };
    void *direct_hdls = NULL;
    int num1 = sizeof(depend_libs) / sizeof(depend_libs[0]);
    direct_hdls = dlopen(direct_libs, RTLD_NOW | RTLD_GLOBAL);
    if (!direct_hdls) {
        rt_err("dlopen %s failed: %s\n", direct_libs, dlerror());
    }
    for (int j = 0; j < num1; j++) {
        if (!check_loaded(depend_libs[j])) {
            rt_err("dso %s should exist in /proc/self/maps\n", depend_libs[j]);
        }
    }

    // @STEP-2: dlclose卸载liba2.so
    // @EXPECT-2: 5个so均被卸载成功
    ret = dlclose(direct_hdls);
    if (ret != 0) {
        rt_err("dlclose %s failed: %s\n", direct_libs, dlerror());
    }
    for (int j = 0; j < num1; j++) {
        if (check_loaded(depend_libs[j])) {
            rt_err("dso %s shouldn't exist in /proc/self/maps\n", depend_libs[j]);
        }
    }
    return RT_EOK;
}

// Func: 本用例执行完测试活动后进行用例清理操作
// Args: 同用例参数
int tc_post_test(int argc, char **argv)
{
    return RT_EOK;
}

// Func: 本用例执行失败后需要进行的日志收集动作
int tc_get_debuginfo(int argc, char **argv)
{
    return RT_EOK;
}

int main(void)
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
        rt_err("tc_get_debuginfo failed:\n");
        return -1;
    }
    return 0;
}
