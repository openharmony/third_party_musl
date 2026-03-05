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
#include "musl_malloc_opt.h"
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

void default_malloc_func(void *temp, size_t bytes)
{
    return;
}

static char *__musl_opt_hook_shared_lib = "lib_kccllockopt.so";
long long __ohos_musl_opt_hook_shared_library = NULL;
bool musl_malloc_opt_enable = false;
typedef bool (*initialize_func_type)();
initialize_func_type initialize_lock_func = default_initialize_func;
initialize_func_type initialize_malloc_func = default_initialize_func;
lock_func_type lock_func = default_lock_func;
malloc_func_type malloc_opt_func = default_malloc_func;

static bool init_musl_opt_hook_shared_library(void *shared_library_handle)
{
    initialize_func_type pfunc_lock_initialize = (initialize_func_type)dlsym(shared_library_handle, "musl_opt_initialize");
    initialize_func_type pfunc_malloc_initialize = (initialize_func_type)dlsym(shared_library_handle, "musl_malloc_opt_initialize");

    if (pfunc_lock_initialize == NULL || pfunc_malloc_initialize == NULL) {
        MUSL_LOGE("dlsym musl_opt_initialize or musl_malloc_opt_initialize failed!");
        return false;
    }
    initialize_lock_func = pfunc_lock_initialize;
    initialize_malloc_func = pfunc_malloc_initialize;

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

static void init_musl_lock_opt_hook(void *shared_library_handle)
{
    bool enable_lock_opt = initialize_lock_func();
    if (enable_lock_opt) {
        lock_func_type pfunc_lock = 
            (lock_func_type)dlsym(shared_library_handle, "musl_opt_lock");
        if (pfunc_lock == NULL) {
            MUSL_LOGE("lock node enable but load symbol musl_opt_lock failed!");
            return;
        }
        lock_func = pfunc_lock;
    }
}

static void init_musl_malloc_opt_hook(void *shared_library_handle)
{
    bool enable_malloc_opt = initialize_malloc_func();
    if (enable_malloc_opt) {
        malloc_func_type pfunc_malloc = 
            (malloc_func_type)dlsym(shared_library_handle, "musl_opt_malloc");
        if (pfunc_malloc == NULL) {
            MUSL_LOGE("malloc node enable but load symbol musl_opt_malloc failed!");
            return;
        }
        malloc_opt_func = pfunc_malloc;
        musl_malloc_opt_enable = true;
    }
}

__attribute__((constructor())) static void __musl_opt_initialize()
{
    void *shared_library_handle = (void *)__ohos_musl_opt_hook_shared_library;
    if (shared_library_handle != NULL && shared_library_handle != (void *)-1) {
        return;
    }
    shared_library_handle = load_musl_opt_hook_shared_library();
    if (shared_library_handle != NULL) {
        init_musl_lock_opt_hook(shared_library_handle);
        init_musl_malloc_opt_hook(shared_library_handle);
    }

    __ohos_musl_opt_hook_shared_library = (long long)shared_library_handle;
}

#endif