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

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>

#define LIB_NAME "liblarge_size.so"
#define SYMBOL_NAME "CreateObj"

struct Test {
    virtual void f1();
    void f2();
};

int main()
{
    void *h = dlopen(LIB_NAME, RTLD_LAZY);
    if (!h) {
        printf("dlopen %s failed.\n", LIB_NAME);
        exit(1);
    }

    void *(*createObj)() = (void *(*)())dlsym(h, SYMBOL_NAME);
    if (!createObj) {
        printf("dlsym %s failed.\n", SYMBOL_NAME);
        exit(1);
    }
    void *p = createObj();
    if (!p) {
        printf("create obj failed.\n");
        exit(1);
    }
    Test *obj = (Test*)p;
    obj->f1();

    dlclose(h);

    return 0;
}