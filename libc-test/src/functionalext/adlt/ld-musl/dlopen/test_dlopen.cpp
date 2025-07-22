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
#include "functionalext.h"
#include "constant.h"
#include "sampledata.h"
#include "errexit.h"

static int (*get_constant)() = nullptr;
static size_t (*get_buflen)() = nullptr;
static void (*set_buf)(void*, size_t) = nullptr;
static int (*cmp_buf)(void*) = nullptr;

#define RESOLVE(handle, symbol)			 \
    symbol = reinterpret_cast<decltype(symbol)>(dlsym(handle, #symbol)); \
    if ((symbol) == 0)                       \
    { \
	    t_error("%s: dlsym() failed for %s with %s\n", __func__, #symbol, dlerror()); \
    }

static void LdMuslDlOpenTests_Test0() {
    const char *libname = def_libname;
    void *lib;
    size_t len;

    lib = dlopen(libname, RTLD_LAZY);
    if (!lib) {
	    t_error("%s: dlopen() failed for %s with %s\n", libname, dlerror());
	    return;
    }

    RESOLVE(lib, get_constant);
    RESOLVE(lib, set_buf);
    RESOLVE(lib, get_buflen);
    RESOLVE(lib, cmp_buf);

    if (!get_constant || !set_buf || !get_buflen || !cmp_buf) {
        goto err;
    }

    EXPECT_EQ(__func__, CONSTANT, get_constant());
    set_buf(get_sample_data(), get_sample_data_len());
    len = get_buflen();
    EXPECT_EQ(__func__, len, get_sample_data_len());
    EXPECT_EQ(__func__, 0, cmp_buf(get_sample_data()));

err:
    dlclose(lib);    
}

int main(int argc, char **argv)
{    
    LdMuslDlOpenTests_Test0();
    return t_status;
}

