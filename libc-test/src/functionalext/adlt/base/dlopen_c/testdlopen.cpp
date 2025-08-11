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
#include <string>

#include "testdlopen.h"
#include "c-lib.h"

namespace testdlopen {
typedef int (*clibGetVer_t)();
typedef void (*clibPrintf_t)(const char *);
typedef int (*clibMax_t)(int, int);

// open library
static void *openLib(const char *filename) {
    printf("\tLoading library %s\n", filename);
    void *handle = dlopen(filename, RTLD_LAZY);
    if (!handle) {
        printf("Error loading library!\n");
        dlerror();
    }

    return handle;
}

// close library
static void closeLib(void *handle) {
    printf("\tClosing library\n");
    dlclose(handle);
}

int testVer(const char *filename) {
    printf("Testing clibGetVer\n");
    void *handle = openLib(filename);
    if (!handle) {
        printf("Exiting!\n");
        return -1;
    }
    
    printf("\tLoading symbol clibGetVer\n");
    clibGetVer_t getVer = (clibGetVer_t) dlsym(handle, "clibGetVer");

    printf("\tCalling clibGetVer\n");
    int ver = getVer();

    closeLib(handle);

    printf("Exiting clibGetVer test\n");
    return ver;
}

int testPrintf(const char *filename, const char *text) {
    printf("Testing clibPrintf\n");
    void *handle = openLib(filename);
    if (!handle) {
        printf("Exiting!\n");
        return -1;
    }
    
    printf("\tLoading symbol clibPrintf\n");
    clibPrintf_t libPrintf = (clibPrintf_t) dlsym(handle, "clibPrintf");

    printf("\tCalling clibPrintf\n");
    libPrintf(text);

    closeLib(handle);

    printf("Exiting clibGetVet test\n");
    return 0;
}

int testMax(const char *filename, int a, int b) {
    printf("Testing clibMax\n");
    void *handle = openLib(filename);
    if (!handle) {
        printf("Exiting!\n");
        return -1;
    }
    
    printf("\tLoading symbol clibMax\n");
    clibMax_t libMax = (clibMax_t) dlsym(handle, "clibMax");

    printf("\tCalling clibMax\n");
    int max = libMax(a, b);

    closeLib(handle);

    printf("Exiting clibMax test\n");
    return max;
}
} // namespace testdlopen