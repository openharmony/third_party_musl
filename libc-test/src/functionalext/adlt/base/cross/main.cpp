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


static void CrossTests_CheckSources() {
    EXPECT_STREQ(__func__, "a.c", strA);
    EXPECT_STREQ(__func__, "b.c", strB);
}

static void CrossTests_CheckMessages() {
    EXPECT_STREQ(__func__, "message from lib A!", msgA);
    EXPECT_STREQ(__func__, "message from lib B!", msgB);
}

static void CrossTests_TestSumF1F2() {
    int sum = 0;
    const int countRuns = 1000000;
    for (size_t i = 0; i < countRuns; i++)
        sum += f1();
    EXPECT_EQ(__func__, 3000000, sum);

    for (size_t i = 0; i < countRuns; i++)
        sum += f2();
    EXPECT_EQ(__func__, 7000000, sum);
}

int main(int argc, char **argv) {
    printf("Hello! This test contains libs built from sources: ");
    const char *sources[] = {strA, strB};
    const int countSources = sizeof(sources) / sizeof(sources[0]);
    for (size_t i = 0; i < countSources; i++)
        printf("%s ", sources[i]);
    printf("\n");

    printf("Test print calls in libs: \n");
    testPrintA();
    testPrintB();

    testPrintMessageAfromA();
    testPrintMessageBfromB();

    testPrintMessageAfromB();
    testPrintMessageBfromA();

    CrossTests_CheckSources();
    CrossTests_CheckMessages();
    CrossTests_TestSumF1F2();
    return t_status;
}