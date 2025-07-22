/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <dlfcn.h>

#include "functionalext.h"
#include "constant.h"

#include "errexit.h"
#include "sampledata.h"
#include "foo.h"

static int (*s_get_constant)() = nullptr;
static size_t (*s_get_buflen)() = nullptr;
static void (*s_set_buf)(void*, size_t) = nullptr;
static int (*s_cmp_buf)(void*) = nullptr;

#define RESOLVE(handle, var, symbol)		     \
    var = reinterpret_cast<decltype(var)>(dlsym(handle, #symbol)); \
    if ((var) == 0)                       \
    { \
	    t_error("%s: dlsym() failed for %s with %s\n", __func__, #symbol, dlerror()); \
    }

static void LdMuslDlOpenDoubleMapTests_Test0() {
    void *lib;
    const char *libname = def_libname;
    
    /* Check direct mapping */
    EXPECT_EQ(__func__, CONSTANT, get_constant());
    set_buf(get_sample_data(), get_sample_data_len());
    EXPECT_EQ(__func__, get_buflen(), get_sample_data_len());
    EXPECT_EQ(__func__, 0, cmp_buf(get_sample_data()));


    /* Resolve and use same symbols but via explicit dlopen */
    lib = dlopen(libname, RTLD_LAZY);
    if (!lib)
	{
	    t_error("%s: dlopen() failed for %s with %s\n", __func__, libname, dlerror());
	    return;
	}

    RESOLVE(lib, s_get_constant, get_constant);
    RESOLVE(lib, s_set_buf, set_buf);
    RESOLVE(lib, s_get_buflen, get_buflen);
    RESOLVE(lib, s_cmp_buf, cmp_buf);

    if (!s_get_constant || !s_set_buf || !s_get_buflen || !s_cmp_buf) {
        goto err;
    }

    EXPECT_EQ(__func__, CONSTANT, s_get_constant());

    s_set_buf(get_sample_data(), get_sample_data_len());
    EXPECT_EQ(__func__, s_get_buflen(), get_sample_data_len());
    EXPECT_EQ(__func__, 0, s_cmp_buf(get_sample_data()));
    EXPECT_TRUE(__func__, cmp_buf == s_cmp_buf);

err:
    dlclose(lib);
}

int main(int argc, char **argv)
{    
    LdMuslDlOpenDoubleMapTests_Test0();
    return t_status;
}

