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
#include "caller.h"
#include <dlfcn.h>
#include <stdio.h>

/*
 * Reproduces the audio_server bug scenario:
 * This nDSO (simulating libasymc_stack) calls dlsym(RTLD_DEFAULT, ...)
 * looking for a symbol that belongs to another nDSO (simulating libuv)
 * within the same ADLT-merged SO.
 *
 * Without the is_adlt_dso_sym fix:
 *   gnu_lookup finds the symbol in the merged symbol table → returned (BUG)
 *
 * With the is_adlt_dso_sym fix:
 *   gnu_lookup finds the symbol → is_adlt_dso_sym says it doesn't belong
 *   to this nDSO → sym = NULL → adlt_lookup_unique_sym can't find the
 *   postfix variant for current nDSO → symbol not found (CORRECT)
 */
int caller_try_find_provider_sym(void)
{
    void *sym = dlsym(RTLD_DEFAULT, "ProviderSetCallbackFunc");
    return (sym != NULL) ? 1 : 0;
}

int caller_get_value(void)
{
    return 100;
}
