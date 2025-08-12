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
#include <cstdio>
#include <dlfcn.h>
#include <execinfo.h>
#include <securec.h>

#include "testbacktrace.h"

#define STRING_MAX_LEN  256

namespace testbt {
    typedef const char* (*clibGetVer_t)();
    typedef int (*clibMax_t)(int, int);

    // close library
    void closeLib(void *handle) {
        printf("\tClosing library\n");
        dlclose(handle);
    }

    // open library
    void *openLib(const char *filename) {
        printf("\tLoading library %s\n", filename);
        void *handle = dlopen(filename, RTLD_LAZY);
        if (!handle) {
            printf("Error loading library!\n");
            dlerror();
        }

        return handle;
    }

    std::string getSymbol(void *func) {
        Dl_info info = { 0 };
        dladdr(func, &info);

        char *str_buffer = static_cast<char *>(malloc(STRING_MAX_LEN * sizeof(char)));

        if (info.dli_sname && info.dli_saddr && info.dli_fbase && info.dli_fname) {
            int ret = snprintf_s(str_buffer, STRING_MAX_LEN, STRING_MAX_LEN - 1,
            "    <%s+%#lx>[%#lx] -> %s\n", info.dli_sname,
            reinterpret_cast<uintptr_t>(func) - reinterpret_cast<uintptr_t>(info.dli_saddr),
            reinterpret_cast<uintptr_t>(func) - reinterpret_cast<uintptr_t>(info.dli_fbase), info.dli_fname);
            if (ret < 0) {
                free(str_buffer);
                return "Error: failed to format symbol data\n";
            }

            std::string str(str_buffer);
            free(str_buffer);
            return str;
        } else {
            free(str_buffer);
        }

        return "Error: could not resolve symbol data\n";
    }

    // test if clibGetVer is loading and working properly
    std::string testVer(const char *filename) {
        printf("Testing clibGetVer\n");
        void *handle = openLib(filename);
        if (!handle) {
            printf("Exiting!\n");
            return "test failure";
        }

        printf("\tLoading symbol clibGetVer\n");
        clibGetVer_t getVer = reinterpret_cast<clibGetVer_t>(dlsym(handle, "clibGetVer"));

        printf("\tCalling clibGetVer\n");
        const char *ver = getVer();

        closeLib(handle);

        printf("Exiting clibGetVer test\n");
        return ver;
    }

    // test if clibGetVer symbol can be backteaced properly after loading
    std::string testVerBT(const char *filename) {
        printf("Testing clibGetVer symbol backtrace\n");
        void *handle = openLib(filename);
        if (!handle) {
            printf("Exiting!\n");
            return "test failure";
        }

        printf("\tLoading symbol clibGetVer\n");
        clibGetVer_t getVer = reinterpret_cast<clibGetVer_t>(dlsym(handle, "clibGetVer"));

        auto ret = getSymbol(reinterpret_cast<void *>(getVer));
        printf("\tThis is what dladdr returned: %s\n", ret.c_str());

        closeLib(handle);

        printf("Exiting clibGetVer symbol backtrace test\n");
        return ret;
    }

    // test if clibmax is loading and working properly
    int testMax(const char *filename, int a, int b) {
        printf("Testing clibMax\n");
        void *handle = openLib(filename);
        if (!handle) {
            printf("Exiting!\n");
            return -1;
        }

        printf("\tLoading symbol clibMax\n");
        clibMax_t libMax = reinterpret_cast<clibMax_t>(dlsym(handle, "clibMax"));

        printf("\tCalling clibMax\n");
        int max = libMax(a, b);

        closeLib(handle);

        printf("Exiting clibMax test\n");
        return max;
    }

    // test if clibGetVer symbol can be backteaced properly after loading
    std::string testMaxBT(const char *filename) {
        printf("Testing clibMax symbol backtrace\n");
        void *handle = openLib(filename);
        if (!handle) {
            printf("Exiting!\n");
            return "test failure";
        }

        printf("\tLoading symbol clibMax\n");
        clibMax_t libMax = reinterpret_cast<clibMax_t>(dlsym(handle, "clibMax"));

        auto ret = getSymbol(reinterpret_cast<void *>(libMax));
        printf("\tThis is what dladdr returned: %s\n", ret.c_str());

        closeLib(handle);

        printf("Exiting clibMax symbol backtrace test\n");
        return ret;
    }
} // namespace testbt
