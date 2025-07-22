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

static int  theVar = V_PREINIT;

int getvar() { return theVar; }

void printvar(const char *caller, const char *tag) {
    const char *defcaller="common";
    const char *deftag="implicit";
    if (!caller || !*caller) {
	caller = defcaller;
	tag = deftag;
    }
    msg(("printvar (%s:%s): %d\n", caller, tag, theVar));
}

 __attribute__((constructor)) void mkinit()
{
    theVar += V_ADD; 
}

 __attribute__((destructor)) void mkfin()
{
    theVar -= V_ADD; 
}
