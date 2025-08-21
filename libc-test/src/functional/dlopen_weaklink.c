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
#include <sys/wait.h>
#include "test.h"
#include "global.h"

#define SO_FOR_WEAKLINK_A "/data/local/tmp/libc-test-lib/libdlopen_weaklink_a.so"
#define SO_FOR_WEAKLINK_B "/data/local/tmp/libc-test-lib/libdlopen_weaklink_b.so"
#define SO_FOR_WEAKLINK_C "/data/local/tmp/libc-test-lib/libdlopen_weaklink_c.so"
#define SO_FOR_WEAKLINK_D "/data/local/tmp/libc-test-lib/libdlopen_weaklink_d.so"

void dlopen_weaklink_001()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_A, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_A, RTLD_NOW, dlerror());
        return;
    }
    dlclose(handle);
}

void dlopen_weaklink_002()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_B, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_B, RTLD_NOW, dlerror());
        return;
    }
    dlclose(handle);

    void* handle2 = dlopen(SO_FOR_WEAKLINK_C, RTLD_NOW);
    if (!handle2) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_C, RTLD_NOW, dlerror());
        return;
    }
    dlclose(handle2);
}

void dlopen_weaklink_003()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_C, RTLD_NOW);
    if (handle) {
        t_error("dlopen(name=%s,mode=%d) succeed\n", SO_FOR_WEAKLINK_C, RTLD_NOW);
        dlclose(handle);
    }
}

void dlopen_weaklink_004()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_D, RTLD_NOW);
    if (!handle) {
        t_error("dlopen(name=%s,mode=%d) failed: %s\n", SO_FOR_WEAKLINK_D, RTLD_NOW, dlerror());
        return;
    }
    dlclose(handle);
}

void dlopen_weaklink_005()
{
    void* handle = dlopen(SO_FOR_WEAKLINK_D, RTLD_NOW);
    if (handle) {
        t_error("dlopen(name=%s,mode=%d) succeed\n", SO_FOR_WEAKLINK_D, RTLD_NOW);
        dlclose(handle);
    }
}

int main(int argc, char *argv[])
{
    dlopen_weaklink_001();
    dlopen_weaklink_002();
    dlopen_weaklink_004();
    if (remove("/data/local/tmp/libc-test-lib/libfoo.so") != 0) {
        t_error("remove /data/local/tmp/libc-test-lib/libfoo.so failed \n");
    }

    dlopen_weaklink_001();
    dlopen_weaklink_003();
    dlopen_weaklink_005();
    if (remove("SO_FOR_WEAKLINK_B") != 0) {
        t_error("remove SO_FOR_WEAKLINK_B failed \n");
    }
    dlopen_weaklink_004();

    return t_status;
}