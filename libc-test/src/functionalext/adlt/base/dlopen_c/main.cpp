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
#include <string>

#include "functionalext.h"
#include "testdlopen.h"

static const char *fileName = "libadlt_base_dlopen_c_lib.so";

static void DlopenCTests_TestGetVer() {
    EXPECT_EQ(__func__, 1, testdlopen::testVer(fileName));
}

static void DlopenCTests_TestPrintf() {
    EXPECT_EQ(__func__, 0, testdlopen::testPrintf(fileName, "some text\n"));
}

static void DlopenCTests_TestMax() {
    EXPECT_EQ(__func__, 2, testdlopen::testMax(fileName, 1, 2));
}

int main(int argc, char **argv) {
    DlopenCTests_TestGetVer();
    DlopenCTests_TestPrintf();
    DlopenCTests_TestMax();
    return t_status;
}

