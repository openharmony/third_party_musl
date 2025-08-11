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
#include "common.h"

static void VersionsTest_testCase1() {
    // get versioned symbol from each lib
    EXPECT_EQ(__func__, 1, get_valA_fromA());
    EXPECT_EQ(__func__, 2, get_valB_fromB());
    // cross getting val
    EXPECT_EQ(__func__, 1, get_valA_fromB());
    EXPECT_EQ(__func__, 2, get_valB_fromA());
}

static void VersionsTest_testCase2() {
    // different names, same versions
    EXPECT_EQ(__func__, 3, duplicate_a);
    EXPECT_EQ(__func__, 4, duplicate_b);
    // same names, different versions
    EXPECT_EQ(__func__, 5, duplicate_1);
    // same names, same versions
    EXPECT_EQ(__func__, 7, duplicate_2);
}

int main(int argc, char **argv) {
    VersionsTest_testCase1();
    VersionsTest_testCase2();
    return t_status;
}
