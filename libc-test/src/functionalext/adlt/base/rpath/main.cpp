/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "adlt_common.h"

const char *libname = "libadlt_base_rpath_a.so";

class DynLib
{
public:
    DynLib(): lib(nullptr), testPrintMessagefromA(nullptr)
    {
        lib = dlopen(libname, RTLD_LAZY);
        printf("try to dlopen lib %s\n", libname);
        testPrintMessagefromA = nullptr;
        if (lib) {
            testPrintMessagefromA = reinterpret_cast<void(*)()>(dlsym(lib, "testPrintMessagefromA"));
        }
    }
    ~DynLib()
    {
        if (lib) {
            dlclose(lib);
        }
    }
    void *lib;
    void (*testPrintMessagefromA)();
};

static void TestRpath_test1(int testno) {
    DynLib *dynLib = new DynLib();
    switch (testno) {
        case 0:
            // without -rpath, without LD_LIBRARY_PATH
            printf("without -rpath, without LD_LIBRARY_PATH - lib is unaccessible\n");
            EXPECT_PTREQ(__func__, dynLib->lib, nullptr);
            break;
        case 1:
            // without -rpath, with LD_LIBRARY_PATH
            printf("without -rpath, with LD_LIBRARY_PATH - check that lib is loaded,\n");
            EXPECT_PTRNE(__func__, dynLib->lib, nullptr);
            printf("load and call testPrintMessagefromA():\n");
            EXPECT_PTRNE(__func__, dynLib->testPrintMessagefromA, nullptr);
            dynLib->testPrintMessagefromA();
            break;
        case 2:
            // with -rpath, without LD_LIBRARY_PATH
            printf("with -rpath, without LD_LIBRARY_PATH - check that lib is loaded,\n");
            EXPECT_PTRNE(__func__, dynLib->lib, nullptr);
            printf("load and call testPrintMessagefromA():\n");
            EXPECT_PTRNE(__func__, dynLib->testPrintMessagefromA, nullptr);
            dynLib->testPrintMessagefromA();
            break;
        default:
            EXPECT_TRUE("unknown test case", 0);
        }
    delete dynLib;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        int testno = atoi(argv[1]);
        TestRpath_test1(testno);
        return t_status;
    }

    int ret = 0;
#ifdef WITHOUT_RPATH
    ret = ret || run_self_command("0");
    ret = ret || run_self_command("1", "LD_LIBRARY_PATH=/data/local/tmp/libc-test-lib"); 
#else
    ret = run_self_command("2");
#endif

    return ret;
}
