/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "memory_trace.h"
#include "errno.h"
#ifdef HOOK_ENABLE
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include "common_def.h"
#include "musl_preinit_common.h"

struct ResTraceTls {
    uint32_t traceType;
    uint64_t traceId;
};
static pthread_key_t g_resTraceKey;
static pthread_once_t g_resTraceOnce = PTHREAD_ONCE_INIT;
static atomic_bool g_resTraceKeyReady = ATOMIC_VAR_INIT(false);

static void initResTraceKey(void)
{
    int result = pthread_key_create(&g_resTraceKey, NULL);
    if (result == 0) {
        atomic_store_explicit(&g_resTraceKeyReady, true, memory_order_release);
        (void)pthread_setspecific(g_resTraceKey, NULL);
    } else {
        atomic_store_explicit(&g_resTraceKeyReady, false, memory_order_release);
    }
}

static struct ResTraceTls* getOrCreateResTraceTls(void)
{
    (void)pthread_once(&g_resTraceOnce, initResTraceKey);
    
    if (!atomic_load_explicit(&g_resTraceKeyReady, memory_order_acquire)) {
        return NULL;
    }
    
    struct ResTraceTls* tls = (struct ResTraceTls*)pthread_getspecific(g_resTraceKey);
    if (tls == NULL) {
        // 使用calloc确保内存清零
        tls = (struct ResTraceTls*)calloc(1, sizeof(struct ResTraceTls));
        if (tls == NULL) {
            return NULL;
        }
		tls->traceType = 0;
        tls->traceId = 0;
        (void)pthread_setspecific(g_resTraceKey, tls);
    }
    return tls;
}
#endif

void setResTraceId(uint32_t newTraceType, uint64_t newTraceID, uint32_t* pOldTraceType, uint64_t* pOldTraceID)
{
#ifdef HOOK_ENABLE
    if (!pOldTraceType || !pOldTraceID) {
        return;
    }
    struct ResTraceTls* tls = getOrCreateResTraceTls();
    if (tls == NULL) {
        *pOldTraceType = 0;
        *pOldTraceID = 0;
        return;
    }
    *pOldTraceType = tls->traceType;
    *pOldTraceID = tls->traceId;

    tls->traceType = newTraceType;
    tls->traceId = newTraceID;
#endif
	errno = ENOSYS;
	return;
}

bool getResTraceId(uint32_t* pTraceType, uint64_t* pTraceID)
{
#ifdef HOOK_ENABLE
    if (!atomic_load_explicit(&g_resTraceKeyReady, memory_order_acquire)) {
        return false;
    }
    if (!pTraceType || !pTraceID) {
        return false;
    }
    // Don't create key/tls on Get.
    struct ResTraceTls* tls = (struct ResTraceTls*)pthread_getspecific(g_resTraceKey);
    if (tls == NULL) {
        return false;
    }
    *pTraceType = tls->traceType;
    *pTraceID = tls->traceId;
    return true;
#endif
	errno = ENOSYS;
	return false;
}

void memtrace(void* addr, size_t size, const char* tag, bool is_using)
{
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (__get_memleak_hook_flag()) {
			return;
		}
		if (!__get_global_hook_flag()) {
			return;
		}
		else if (!__get_hook_flag()) {
			return;
		}
		return dispatch_table->memtrace(addr, size, tag, is_using);
	}
#endif
    return;
}

void restrace(unsigned long long mask, void* addr, size_t size, const char* tag, bool is_using)
{
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (!__get_global_hook_flag()) {
			return;
		}
		else if (!__get_hook_flag()) {
			return;
		}
		return dispatch_table->restrace(mask, addr, size, tag, is_using);
	}
    return;
#endif
    errno = ENOSYS;
}

void resTraceMove(unsigned long long mask, void* oldAddr, void* newAddr, size_t newSize){
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (!__get_global_hook_flag()) {
			return;
		}
		else if (!__get_hook_flag()) {
			return;
		}
		return dispatch_table->resTraceMove(mask, oldAddr, newAddr, newSize);
	}
	return;
#endif
	errno = ENOSYS;
}

void resTraceFreeRegion(unsigned long long mask, void* addr, size_t size){
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (!__get_global_hook_flag()) {
			return;
		}
		else if (!__get_hook_flag()) {
			return;
		}
		return dispatch_table->resTraceFreeRegion(mask, addr, size);
	}
	return;
#endif
	errno = ENOSYS;
}
