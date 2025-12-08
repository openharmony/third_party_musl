/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "functionalext.h"
#include "ldso_dlopen_config_abs_path.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    if (!CheckPrerequisites()) {
        printf("CheckPrerequisites failed\n");
        return t_status;
    }

    dlopen_config_abs_path_policy_func(true);
    dlopen_config_abs_path_policy_func(false);
    if (!OpenSo(false, true)) {
        printf("OpenSo with abs path failed\n");
        t_status = 1;
    }
    
    return t_status;
}