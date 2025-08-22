/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <link.h>

#include "test.h"
#include "global.h"

#define SO_FOR_WEAKLINK_A "/data/local/tmp/libc-test-lib/libdlopen_weaklink_a.so"
#define SO_FOR_WEAKLINK_B "/data/local/tmp/libc-test-lib/libdlopen_weaklink_b.so"
#define SO_FOR_WEAKLINK_C "/data/local/tmp/libc-test-lib/libdlopen_weaklink_c.so"
#define SO_FOR_WEAKLINK_D "/data/local/tmp/libc-test-lib/libdlopen_weaklink_d.so"
#define FOO_SO "/data/local/tmp/libc-test-lib/libfoo.so"

static int test_value = 0;

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
    if (strcmp(info->dlpi_name, FOO_SO) != 0) {
        return 0;
    }
    test_value++;
    return 0;
}

static int callback_b(struct dl_phdr_info *info, size_t size, void *data)
{
    if (strcmp(info->dlpi_name, SO_FOR_WEAKLINK_B) != 0) {
        return 0;
    }
    test_value++;
    return 0;
}

static bool check_foo_so()
{
    test_value = 0;
    dl_iterate_phdr(callback, NULL);

    if (test_value != 0) {
        return true;
    }

    return false;
}

static bool check_weaklink_b_so()
{
    test_value = 0;
    dl_iterate_phdr(callback_b, NULL);

    if (test_value != 0) {
        return true;
    }

    return false;
}

/**
 * @tc.name     : dlopen_weaklink_001
 * @tc.desc     : Test dlopen libdlopen_weaklink_a.so and foo.so is loaded
 *                when weaklink foo.so exists.
 *                A weaklink foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_001()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_A, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_A, RTLD_NOW, dlerror());
        return;
    }

    bool res = check_foo_so();

    if (!res) {
        t_error("libfoo.so not load \n");
    }

    dlclose(handle);
}

/**
 * @tc.name     : dlopen_weaklink_002
 * @tc.desc     : Test dlopen libdlopen_weaklink_a.so and foo.so is not loaded
 *                when weaklink foo.so not exists.
 *                A weaklink foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_002()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_A, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_A, RTLD_NOW, dlerror());
        return;
    }

    bool res = check_foo_so();

    if (res) {
        t_error("libfoo.so is load \n");
    }

    dlclose(handle);
}

/**
 * @tc.name     : dlopen_weaklink_003
 * @tc.desc     : Test dlopen libdlopen_weaklink_c.so and foo.so is loaded
 *                when weaklink foo.so exists.
 *                C link A weaklink foo
 *                C link B link foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_003()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_B, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_B, RTLD_NOW, dlerror());
        return;
    }

    bool res = check_foo_so();

    if (!res) {
        t_error("libfoo.so not load \n");
    }

    dlclose(handle);

    void* handle2 = dlopen(SO_FOR_WEAKLINK_C, RTLD_NOW);
    if (!handle2) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_C, RTLD_NOW, dlerror());
        return;
    }

    res = check_foo_so();

    if (!res) {
        t_error("libfoo.so not load \n");
    }

    dlclose(handle2);
}

/**
 * @tc.name     : dlopen_weaklink_004
 * @tc.desc     : Test dlopen libdlopen_weaklink_c.so when foo.so not exists.
 *                C link A weaklink foo
 *                C link B link foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_004()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_C, RTLD_NOW);
    if (handle) {
        t_error("dlopen(name=%s,mode=%d) succeed\n", SO_FOR_WEAKLINK_C, RTLD_NOW);
        dlclose(handle);
    }
}

/**
 * @tc.name     : dlopen_weaklink_005
 * @tc.desc     : Test dlopen libdlopen_weaklink_d.so when foo.so and libdlopen_weaklink_b exists.
 *                D weaklink B link foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_005()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_D, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_D, RTLD_NOW, dlerror());
        return;
    }

    bool res = check_foo_so();

    if (!res) {
        t_error("libfoo.so not load \n");
    }
    dlclose(handle);
}

/**
 * @tc.name     : dlopen_weaklink_006
 * @tc.desc     : Test dlopen libdlopen_weaklink_d.so when foo.so not exists.
 *                D weaklink B link foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_006()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_D, RTLD_NOW);
    if (handle) {
        t_error("dlopen(name=%s,mode=%d) succeed\n", SO_FOR_WEAKLINK_D, RTLD_NOW);
        dlclose(handle);
    }
}

/**
 * @tc.name     : dlopen_weaklink_007
 * @tc.desc     : Test dlopen libdlopen_weaklink_d.so when libdlopen_weaklink_b.so not exists.
 *                D weaklink B link foo
 * @tc.level    : Level 0
 */

void dlopen_weaklink_007()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_D, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_D, RTLD_NOW, dlerror());
        return;
    }

    bool res = check_weaklink_b_so();

    if (res) {
        t_error("weaklink_b_so is load \n");
    }
    dlclose(handle);
}

int main(int argc, char *argv[])
{
    dlopen_weaklink_001();
    dlopen_weaklink_003();
    dlopen_weaklink_005();
    if (remove(FOO_SO) != 0) {
        t_error("remove FOO_SO failed \n");
    }

    dlopen_weaklink_002();
    dlopen_weaklink_004();
    dlopen_weaklink_006();
    if (remove(SO_FOR_WEAKLINK_B) != 0) {
        t_error("remove SO_FOR_WEAKLINK_B failed \n");
    }
    dlopen_weaklink_007();

    return t_status;
}