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
#include "testdlopen.h"

static const char *fileName = "libadlt_base_dlopen_cpp_lib.so";

static void DlopenCPPTests_TestGetStorage() {
    EXPECT_EQ(__func__, 42, testdlopen::testCtor(fileName, 42));
}

static void DlopenCPPTests_TestGetVer() {
    char *p = const_cast<char *>(testdlopen::testVer(fileName).c_str());
    EXPECT_STREQ(__func__, "1.1", p);
}

static void DlopenCPPTests_TestPrintf() {
    EXPECT_EQ(__func__, 0, testdlopen::testPrintf(fileName, "some text\n"));
}

static void DlopenCPPTests_TestMax() {
    EXPECT_EQ(__func__, 2, testdlopen::testMax(fileName, 1, 2));
}

int main(int argc, char **argv) {
    DlopenCPPTests_TestGetStorage();
    DlopenCPPTests_TestGetVer();
    DlopenCPPTests_TestPrintf();
    DlopenCPPTests_TestMax();
    return t_status;
}
