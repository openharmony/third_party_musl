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

#include <stdio.h>
#include <ctype.h>
#include <wctype.h>
#include <dlfcn.h>
#include <functionalext.h>
#include "test.h"


#ifndef __LITEOS__
#define ICU_UC_SO "libhmicuuc.z.so"
#define UCASE_TOUPPER "ucase_toupper"
#define ICU_GET_VERSION_NUM_SYMBOL "GetIcuVersion"
static void* g_hmicu_handle = NULL;
static char* g_hmicu_version = NULL;
static char* (*f_hmicu_version)(void);
static char valid_icu_symbol[64];

static void get_hmicu_handle(void)
{
	if (!g_hmicu_handle) {
		g_hmicu_handle = dlopen(ICU_UC_SO, RTLD_LOCAL);
	}
}

static void get_icu_version_num(void) {
	get_hmicu_handle();
	if (g_hmicu_handle) {
		f_hmicu_version = dlsym(g_hmicu_handle, ICU_GET_VERSION_NUM_SYMBOL);
	}

	if (f_hmicu_version) {
		g_hmicu_version = f_hmicu_version();
	}
}

static void* find_hmicu_symbol(const char* symbol_name) {
	get_icu_version_num();
	if (g_hmicu_version) {
		snprintf(valid_icu_symbol, sizeof(valid_icu_symbol), "%s_%s", symbol_name, g_hmicu_version);
		return dlsym(g_hmicu_handle, valid_icu_symbol);
	}
 	return NULL;
}
#endif

/**
 * @tc.name      : towupper_0100
 * @tc.desc      : get icu symbol
 * @tc.level     : Level 1
 */
void towupper_0100(void) 
{
#ifndef __LITEOS__
    get_hmicu_handle();
    if (!g_hmicu_handle) {
        return;
    }
    void* handle = find_hmicu_symbol(UCASE_TOUPPER);
    if (!handle) {
        t_error("towupper_0100 failed! g_hmicu_handle: %p, valid_icu_symbol: %s", g_hmicu_handle, valid_icu_symbol);
    }
#endif
}

/**
 * @tc.name      : towupper_0200
 * @tc.desc      : test towupper
 * @tc.level     : Level 1
 */
void towupper_0200(void) 
{
    wint_t c1 = towupper(L'a');
    EXPECT_EQ("towupper_0200_c1", c1, L'A');
    wint_t c2 = towupper(L'b');
    EXPECT_EQ("towupper_0200_c2", c2, L'B');
    wint_t c3 = towupper(L'c');
    EXPECT_EQ("towupper_0200_c3", c3, L'C');
    wint_t c4 = towupper(L'd'); 
    EXPECT_EQ("towupper_0200_c4", c4, L'D');
}

int main(int argc, char *argv[])
{
    towupper_0100();
    towupper_0200();
    return t_status;
}