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

__thread char      c1 = 1;
__thread char      xchar = 2;
__thread char      c2 = 3;
__thread short     xshort = 4;
__thread char      c3 = 5;
__thread int       xint = 6;
__thread char      c4 = 7;
__thread long long xllong = 8;

struct {
	char *name;
	unsigned size;
	unsigned align;
	unsigned long addr;
} t[4];

#define entry(i, x) \
	t[i].name = #x; \
	t[i].size = sizeof x; \
	t[i].align = __alignof__(x); \
	t[i].addr = (unsigned long)&x

__attribute__((constructor)) static void init(void)
{
	entry(0, xchar);
	entry(1, xshort);
	entry(2, xint);
	entry(3, xllong);
}