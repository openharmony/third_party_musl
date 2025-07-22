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

static void CFICallCastTests_TestCall() {
    fprintf(stderr, "cfi call test\n");
    test_call();
}

static void CFICallCastTests_TestCast() {
    fprintf(stderr, "cfi cast test\n");
    test_cast();
}

int main(int argc, char **argv) {    
    CFICallCastTests_TestCall();
    CFICallCastTests_TestCast();
    return t_status;
}
