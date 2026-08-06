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
#include "caller.h"
#include <dlfcn.h>
#include <stdio.h>

/*
 * Test for bugfix: dlsym(RTLD_DEFAULT) must NOT find symbols from an nDSO
 * that the main executable has no direct dependency on, even though that
 * nDSO is merged into the same ADLT SO.
 *
 * Reproduces the audio_server bug:
 *   - libasymc_stack.so called dlsym(RTLD_DEFAULT, "LibuvSetReleaseAsyncContextFunc")
 *   - libuv.so was NOT loaded by audio_server, but was in the same ADLT-merged SO
 *   - Without is_adlt_dso_sym filter, gnu_lookup found the symbol in the
 *     merged symbol table and incorrectly returned it
 *   - With is_adlt_dso_sym filter, the symbol is rejected because it
 *     doesn't belong to the current nDSO, then adlt_lookup_unique_sym
 *     can't find the postfix variant for the current nDSO either
 *
 * Test structure:
 *   ADLT-merged SO contains 2 independent nDSOs:
 *     - caller: simulates libasymc_stack (main exe depends on this)
 *     - provider: simulates libuv (main exe does NOT depend on this)
 *   The main executable only links against the caller nDSO.
 */

static void test_dlsym_rtld_default_cross_ndso_not_found()
{
    /*
     * ProviderSetCallbackFunc belongs to the "provider" nDSO.
     * Since the main exe only depends on the "caller" nDSO,
     * this symbol should NOT be found via RTLD_DEFAULT.
     */
    printf("[dlsym_rtld_default_cross_ndso] test: %s\n", __func__);
    int found = caller_try_find_provider_sym();
    printf("[dlsym_rtld_default_cross_ndso]   dlsym(RTLD_DEFAULT, \"ProviderSetCallbackFunc\") from caller nDSO: %s\n",
           found ? "FOUND (BUG)" : "not found (PASS)");
    EXPECT_EQ(__func__, 0, found);
}

static void test_dlsym_rtld_default_caller_sym_found()
{
    /*
     * Verify that dlsym(RTLD_DEFAULT) can still find symbols from
     * the caller nDSO, which the main exe does depend on.
     */
    printf("[dlsym_rtld_default_cross_ndso] test: %s\n", __func__);
    void *sym = dlsym(RTLD_DEFAULT, "caller_get_value");
    printf("[dlsym_rtld_default_cross_ndso]   dlsym(RTLD_DEFAULT, \"caller_get_value\"): %s\n",
           sym ? "found (PASS)" : "NOT FOUND (BUG)");
    EXPECT_PTRNE(__func__, nullptr, sym);
}

static void test_dlopen_provider_then_dlsym_found()
{
    /*
     * Positive test: after explicitly dlopen'ing the provider SO,
     * its symbols SHOULD be found via dlsym with the returned handle.
     * This verifies the is_adlt_dso_sym filter doesn't break
     * legitimate dlopen-based symbol resolution.
     */
    printf("[dlsym_rtld_default_cross_ndso] test: %s\n", __func__);
    void *handle = dlopen("libadlt_bugfix_dlsym_default_provider.so", RTLD_NOW);
    EXPECT_PTRNE(__func__, nullptr, handle);
    if (!handle) {
        t_error("%s: dlopen failed: %s\n", __func__, dlerror());
        printf("[dlsym_rtld_default_cross_ndso]   dlopen failed: %s\n", dlerror());
        return;
    }

    typedef void (*ProviderFunc)(void);
    ProviderFunc func = reinterpret_cast<ProviderFunc>(dlsym(handle, "ProviderSetCallbackFunc"));
    printf("[dlsym_rtld_default_cross_ndso]   dlsym(handle, \"ProviderSetCallbackFunc\"): %s\n",
           func ? "found (PASS)" : "NOT FOUND (BUG)");
    EXPECT_PTRNE(__func__, nullptr, func);
    if (!func) {
        t_error("%s: dlsym failed: %s\n", __func__, dlerror());
    }

    dlclose(handle);
}

int main(int argc, char **argv)
{
    test_dlsym_rtld_default_cross_ndso_not_found();
    test_dlsym_rtld_default_caller_sym_found();
    test_dlopen_provider_then_dlsym_found();
    return t_status;
}
