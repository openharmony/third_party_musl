/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dynlink.h"
#include "musl_log.h"
#include "pthread_impl.h"

extern void add_dso_handle_node(void *dso_handle);
extern void remove_dso_handle_node(void *dso_handle);

int __cxa_thread_atexit_impl(void (*func)(void*), void *arg, void *dso_handle)
{
    struct thread_local_dtor* dtor = __libc_malloc(sizeof(*dtor));
    if (!dtor) {
        MUSL_LOGE("[cxa_thread] Alloc thread_local_dtor failed.\n");
        return 0;
    }
    dtor->func = func;
    dtor->arg = arg;
    dtor->dso_handle = dso_handle;
    pthread_t thr = __pthread_self();
    dtor->next = thr->thread_local_dtors;
    thr->thread_local_dtors = dtor;
    add_dso_handle_node(dso_handle);
    return 0;
}

void __cxa_thread_finalize()
{
    pthread_t thr = __pthread_self();
    while (thr->thread_local_dtors != NULL) {
        struct thread_local_dtor* cur = thr->thread_local_dtors;
        thr->thread_local_dtors= cur->next;
        cur->func(cur->arg);
        remove_dso_handle_node(cur->dso_handle);
    }
    return;
}