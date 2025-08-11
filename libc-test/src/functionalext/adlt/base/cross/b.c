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
#include <stdio.h>

#include "common.h"

const char *strB = "b.c";
const char *msgB = "message from lib B!";

void testPrintB(void) { printf("this is lib B!\n"); }

void testPrintMessageBfromB(void) { printf("%s\n", msgB); }

void testPrintMessageAfromB(void) {
    printf("print msg A from %s: %s\n", strB, msgA);
}

int f2(void) { return f4(); }

int f3(void) { return 3; }