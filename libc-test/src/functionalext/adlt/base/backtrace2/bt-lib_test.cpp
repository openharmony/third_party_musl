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

#include "bt-test.h"

#define STRING_MAX_LEN  256

namespace testbt {
typedef const char* (*clibGetVer_t)();
typedef int (*clibMaxMin_t)(int, int);

// close library
void closeLib(void *handle) {
    printf("\tClosing library\n");
    dlclose(handle);
}

// open library
void *openLib(const char *fileName) {
    printf("\tLoading library %s\n", fileName);
    void *handle = dlopen(fileName, RTLD_LAZY);
    if (!handle) {
        printf("Error loading library!\n");
        dlerror();
    }

    return handle;
}

std::string getSymbol(void *saddr) {
    Dl_info info = { 0 };
    dladdr(saddr, &info);

    char *str_buffer = static_cast<char *>(malloc(STRING_MAX_LEN * sizeof(char)));

    if (info.dli_sname && info.dli_saddr && info.dli_fbase && info.dli_fname) {
        int ret = snprintf_s(str_buffer, STRING_MAX_LEN, STRING_MAX_LEN - 1,
            "<%s+%#lx>[%#lx] -> %s", info.dli_sname,
            reinterpret_cast<uintptr_t>(saddr) - reinterpret_cast<uintptr_t>(info.dli_saddr),
            reinterpret_cast<uintptr_t>(saddr) - reinterpret_cast<uintptr_t>(info.dli_fbase), info.dli_fname);
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

// test if functions without parameters are loading and working properly
std::string TestVer::test() {
    printf("Testing symbol lookup\n");
    void *handleA = openLib(fileNameA);
    if (!handleA) {
        printf("Exiting!\n");
        return "test failure\n";
    }
    void *handleB = openLib(fileNameB);
    if (!handleB) {
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tLookup symbols %s, %s\n", symbolNameA, symbolNameB);
    clibGetVer_t getA = reinterpret_cast<clibGetVer_t>(dlsym(handleA, symbolNameA));
    if (!getA) {
        printf("Error lookup symbol %s!\n", symbolNameA);
        printf("Exiting!\n");
        return "test failure\n";
    }
    clibGetVer_t getB = reinterpret_cast<clibGetVer_t>(dlsym(handleB, symbolNameB));
    if (!getB) {
        printf("Error lookup symbol %s!\n", symbolNameB);
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tCalling %s, %s\n", symbolNameA, symbolNameB);
    verA = getA();
    verB = getB();

    closeLib(handleA);
    closeLib(handleB);

    printf("Exiting symbol lookup test\n");
    return "test not failed";
}

// test if functions without parameters symbol can be backteaced properly after
// loading
std::string TestVerBT::test() {
    printf("Testing symbol backtrace\n");
    void *handleA = openLib(fileNameA);
    if (!handleA) {
        printf("Exiting!\n");
        return "test failure\n";
    }
    void *handleB = openLib(fileNameB);
    if (!handleB) {
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tLookup symbols %s, %s\n", symbolNameA, symbolNameB);
    clibGetVer_t getA = reinterpret_cast<clibGetVer_t>(dlsym(handleA, symbolNameA));
    if (!getA) {
        printf("Error lookup symbol %s!\n", symbolNameA);
        printf("Exiting!\n");
        return "test failure\n";
    }
    clibGetVer_t getB = reinterpret_cast<clibGetVer_t>(dlsym(handleB, symbolNameB));
    if (!getB) {
        printf("Error lookup symbol %s!\n", symbolNameB);
        printf("Exiting!\n");
        return "test failure\n";
    }

    btA = getSymbol(reinterpret_cast<void *>(getA));
    btB = getSymbol(reinterpret_cast<void *>(getB));
    printf("\tFrom dladdr for %s: %s\n", symbolNameA, btA.c_str());
    printf("\tFrom dladdr for %s: %s\n", symbolNameB, btB.c_str());

    closeLib(handleA);
    closeLib(handleB);

    printf("Exiting symbol backtrace test\n");
    return "test not failed";
}

// test if functions with 2 parameters are loading and working properly
std::string TestMaxMin::test() {
    printf("Testing symbol lookup\n");
    void *handleA = openLib(fileNameA);
    if (!handleA) {
        printf("Exiting!\n");
        return "test failure\n";
    }
    void *handleB = openLib(fileNameB);
    if (!handleB) {
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tLookup symbols %s, %s\n", symbolNameA, symbolNameB);
    clibMaxMin_t getA = reinterpret_cast<clibMaxMin_t>(dlsym(handleA, symbolNameA));
    if (!getA) {
        printf("Error lookup symbol %s!\n", symbolNameA);
        printf("Exiting!\n");
        return "test failure\n";
    }
    clibMaxMin_t getB = reinterpret_cast<clibMaxMin_t>(dlsym(handleB, symbolNameB));
    if (!getB) {
        printf("Error lookup symbol %s!\n", symbolNameB);
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tCalling %s, %s\n", symbolNameA, symbolNameB);
    valA = getA(aa, ab);
    valB = getB(ba, bb);

    closeLib(handleA);
    closeLib(handleB);

    printf("Exiting symbol lookup test\n");
    return "test not failed";
}

// test if functions with 2 parameters symbol can be backteaced properly after
// loading
std::string TestMaxMinBT::test() {
    printf("Testing symbol backtrace\n");
    void *handleA = openLib(fileNameA);
    if (!handleA) {
        printf("Exiting!\n");
        return "test failure\n";
    }
    void *handleB = openLib(fileNameB);
    if (!handleB) {
        printf("Exiting!\n");
        return "test failure\n";
    }

    printf("\tLookup symbols %s, %s\n", symbolNameA, symbolNameB);
    clibMaxMin_t getA = reinterpret_cast<clibMaxMin_t>(dlsym(handleA, symbolNameA));
    if (!getA) {
        printf("Error lookup symbol %s!\n", symbolNameA);
        printf("Exiting!\n");
        return "test failure\n";
    }
    clibMaxMin_t getB = reinterpret_cast<clibMaxMin_t>(dlsym(handleB, symbolNameB));
    if (!getB) {
        printf("Error lookup symbol %s!\n", symbolNameB);
        printf("Exiting!\n");
        return "test failure\n";
    }

    btA = getSymbol(reinterpret_cast<void *>(getA));
    btB = getSymbol(reinterpret_cast<void *>(getB));
    printf("\tFrom dladdr for %s: %s\n", symbolNameA, btA.c_str());
    printf("\tFrom dladdr for %s: %s\n", symbolNameB, btB.c_str());

    closeLib(handleA);
    closeLib(handleB);

    printf("Exiting symbol backtrace test\n");
    return "test not failed";
}
} // namespace testbt