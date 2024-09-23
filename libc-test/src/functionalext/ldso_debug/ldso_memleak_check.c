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

#include "functionalext.h"

#define MAX_BUF 256
const char* g_libPath_1 = "/data/local/tmp/libc-test-lib/libldso_debug_test_lib_6.so";
const char* g_libPath_2 = "/data/local/tmp/libc-test-lib/libldso_debug_test_lib_8.so";
const char* g_libPath_3 = "/data/local/tmp/libc-test-lib/libldso_debug_test_lib_9.so";

int check_loaded(char* so)
{
    int pid = getpid();
    char path[MAX_BUF] = { 0 };
    sprintf(path, "/proc/%d/maps", pid);
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }

    char buffer[MAX_BUF] = { 0 };
    while (fgets(buffer, MAX_BUF, fp) != NULL) {
        if (strstr(buffer, so) != NULL) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/**
 * @tc.name      : ldso_memleak_check_0100
 * @tc.desc      : dlopen so file libldso_debug_test_lib_6.so which has another DTNEEDED so file
 *                 libldso_debug_test_lib_7.so. but the libldso_debug_test_lib_7.so is not in device,
 *                 so the dlopen will fail, and we will check there is no memleak for so libldso_debug_test_lib_6.so
 * @tc.level     : Level 0
 */
void ldso_memleak_check_0100(void)
{
    void* handle = dlopen(g_libPath_1, RTLD_NOW);
    if (handle != NULL) {
        t_error("dlopen(name=%s, mode=%d) should fail\n", g_libPath_1, RTLD_NOW);
    }
    if (strstr(dlerror(), "libldso_debug_test_lib_7.so") == NULL) {
        t_error("libldso_debug_test_lib_7.so should be not found, %s\n", dlerror());
    }
    if (check_loaded((char*)g_libPath_1)) {
        t_error("This so file should not exist in maps, %s\n", (char*)g_libPath_1);
    }
}

/**
 * @tc.name      : ldso_memleak_check_0200
 * @tc.desc      : dlopen so file libldso_debug_test_lib_8.so has DTNEEDED so file libldso_debug_test_lib_9.so.
 *                 libldso_debug_test_lib_9.so is dependent on libldso_debug_test_lib_10.so,
 *                 but the libldso_debug_test_lib_10.so is not in device, so the dlopen will fail,
 *                 and we will check there is no memleak for so libldso_debug_test_lib_8.so
 *                 and libldso_debug_test_lib_9.so
 * @tc.level     : Level 0
 */
void ldso_memleak_check_0200(void)
{
    void* handle = dlopen(g_libPath_2, RTLD_NOW);
    if (handle != NULL) {
        t_error("dlopen(name=%s, mode=%d) should fail\n", g_libPath_2, RTLD_NOW);
    }
    if (check_loaded((char*)g_libPath_2)) {
        t_error("This so file should not exist in maps, %s\n", (char*)g_libPath_2);
    }
    if (check_loaded((char*)g_libPath_3)) {
        t_error("This so file should not exist in maps, %s\n", (char*)g_libPath_3);
    }
}

int main(int argc, char* argv[])
{
    ldso_memleak_check_0100();
    ldso_memleak_check_0200();
    return t_status;
}