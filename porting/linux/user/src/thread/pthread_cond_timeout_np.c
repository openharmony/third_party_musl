/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "pthread_impl.h"

int pthread_cond_timeout_np(pthread_cond_t *cond, pthread_mutex_t *mutex, unsigned int ms)
{
    struct timespec ts;
    __timespec_from_ms(&ts, ms);
    struct timespec abs_timeout;
    __absolute_timespec_from_timespec(&abs_timeout, &ts, CLOCK_MONOTONIC);
    return pthread_cond_timedwait_monotonic_np(cond, mutex, &abs_timeout);
}
