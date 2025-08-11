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
#ifndef _ADLT_TEST_BACKTRACE2_BT_TEST_H_
#define _ADLT_TEST_BACKTRACE2_BT_TEST_H_

#include <string>

namespace testbt {
struct TestVer {
    TestVer(std::string &verA, std::string &verB) :
                    verA(verA), verB(verB) {}

    const char *symbolNameA;
    const char *fileNameA;
    std::string &verA;
    const char *symbolNameB;
    const char *fileNameB;
    std::string &verB;

    std::string test();
};

struct TestVerBT {
    TestVerBT(std::string &btA, std::string &btB) :
                        btA(btA), btB(btB) {}

    const char *symbolNameA;
    const char *fileNameA;
    std::string &btA;
    const char *symbolNameB;
    const char *fileNameB;
    std::string &btB;

    std::string test();
};

struct TestMaxMin {
    TestMaxMin(int &valA, int &valB) :
                        valA(valA), valB(valB) {}

    const char *symbolNameA;
    const char *fileNameA;
    int aa;
    int ab;
    int &valA;
    const char *symbolNameB;
    const char *fileNameB;
    int ba;
    int bb;
    int &valB;

    std::string test();
};

struct TestMaxMinBT {
    TestMaxMinBT(std::string &btA, std::string &btB) :
                            btA(btA), btB(btB) {}

    const char *symbolNameA;
    const char *fileNameA;
    std::string &btA;
    const char *symbolNameB;
    const char *fileNameB;
    std::string &btB;

    std::string test();
};
} // namespace testbt

#endif