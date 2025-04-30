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

#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>

void *async_task(void *arg)
{
    int i = 1000;
    while (i-- > 0) {
    };
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_t pt;
    int i;
    for (int i = 0; i < 100; i++) {
        void *hdl = dlopen("/data/tests/libc-test/ola_test/libtest_tls.so", RTLD_NOW | RTLD_GLOBAL);
        void (*print_tls)(void) = dlsym(hdl, "print_tls");
        if (print_tls == NULL) {
            dlclose(hdl);
            return 1;
        }
        print_tls();
        dlclose(hdl);
        pthread_create(&pt, &attr, async_task, NULL);
        pthread_join(pt, NULL);
    }
}