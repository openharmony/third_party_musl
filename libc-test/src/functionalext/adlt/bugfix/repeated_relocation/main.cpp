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
#include <string>
#include <iostream>

#include <dlfcn.h>
#include "adlt_common.h"

#include "liba.h"

static void test_bugfix_repeated_relocation_verify_symbol_resolution() {
    // call funca firstly(using func1 in liba1)
    EXPECT_EXIT(__func__, callLibACommonFunction(), 0, "Calling from liba: This implementation comes from liba1\n");

    // dynamic load libb.so and call funcb
    void *handle_b = dlopen("libadlt_bugfix_repeated_relocation_libb.so", RTLD_LAZY | RTLD_LOCAL);
    EXPECT_TRUE(__func__, handle_b);
    if(!handle_b) {
        t_error("%s: Failed to load libb: %s\n", __func__,  dlerror());
        return;
    }

    // get addr of funcb
    using FuncBPtr = void(*)();
    auto callLibBCommonFunction = reinterpret_cast<FuncBPtr>(dlsym(handle_b, "callLibBCommonFunction"));
    EXPECT_TRUE(__func__, callLibBCommonFunction);
    if (!callLibBCommonFunction) {
        t_error("%s: Symbol funcb not found: %s\n", __func__, dlerror());
        goto out;
    }

    // call funcb (using func1 of libb1, expecting to call liba1 by load sequences)
    EXPECT_EXIT(__func__, callLibBCommonFunction(), 0, "Calling from libb: This implementation comes from liba1\n");

    // call funcb (using func1 of libb1, expecting to call liba1 by load sequences)
    EXPECT_EXIT(__func__, callLibACommonFunction(), 0, "Calling from liba: This implementation comes from liba1\n");

out:
    // clear resource
    dlclose(handle_b);
}

int main(int argc, char **argv) {
    test_bugfix_repeated_relocation_verify_symbol_resolution();
    return t_status;
}