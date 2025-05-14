/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef USE_MUTEX_WAIT_OPT
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <dlfcn.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "musl_log.h"
#include "musl_opt.h"
#include "atomic.h"

bool default_initialize_func()
{
    return false;
}

void default_lock_func(pthread_mutex_t *restrict m)
{
    int spins = 100;
    while (spins-- && m->_m_lock && !m->_m_waiters) a_spin();
}

static char *__musl_opt_hook_shared_lib = "lib_kccllockopt.so";
long long __ohos_musl_opt_hook_shared_library = NULL;
typedef bool (*initialize_func_type)();
initialize_func_type initialize_func = default_initialize_func;
lock_func_type lock_func = default_lock_func;

static bool init_musl_opt_hook_shared_library(void *shared_library_handle)
{
    initialize_func_type pfunc_initialize = (initialize_func_type)dlsym(shared_library_handle, "musl_opt_initialize");
    lock_func_type pfunc_lock = (lock_func_type)dlsym(shared_library_handle, "musl_opt_lock");
    if (pfunc_initialize == NULL || pfunc_lock == NULL) {
        return false;
    }
    initialize_func = pfunc_initialize;
    lock_func = pfunc_lock;
    return true;
}

static void* load_musl_opt_hook_shared_library()
{
    void* shared_library_handle = NULL;

    shared_library_handle = dlopen(__musl_opt_hook_shared_lib, RTLD_NOW | RTLD_LOCAL);

    if (shared_library_handle == NULL) {
        return NULL;
    }

    if (!init_musl_opt_hook_shared_library(shared_library_handle)) {
        dlclose(shared_library_handle);
        shared_library_handle = NULL;
    }
    return shared_library_handle;
}

static void init_musl_opt_hook()
{
    bool enable = initialize_func();
    if (!enable) {
        lock_func = default_lock_func;
    }
}

__attribute__((constructor())) static void __musl_opt_initialize()
{
    void *shared_library_handle = (void *)__ohos_musl_opt_hook_shared_library;
    if (shared_library_handle != NULL && shared_library_handle != (void *)-1) {
        return;
    }
    shared_library_handle = load_musl_opt_hook_shared_library();
    if (shared_library_handle == NULL) {
        initialize_func = default_initialize_func;
        lock_func = default_lock_func;
    }

    init_musl_opt_hook();
    __ohos_musl_opt_hook_shared_library = (long long)shared_library_handle;
}

#endif