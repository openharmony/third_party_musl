/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifdef ONO_CURRENT_INTERFACE
#include <benchmark/benchmark.h>
#include "pthread.h"
#include "semaphore.h"
#include "signal.h"
#include "threads.h"
#include "unistd.h"
#include "util.h"

static void Bm_function_pthread_mutexattr_settype(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL));
    }
    pthread_mutexattr_destroy(&mutex_attr);
}

static void Bm_function_pthread_mutex_trylock_fast(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_errchk(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_rec(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_fast(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_errorcheck(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_trylock_pi_rec(benchmark::State &state)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_trylock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_fast(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_errchk(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_rec(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_fast(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_errchk(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_mutex_timedlock_pi_rec(benchmark::State &state)
{
    struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    while (state.KeepRunning()) {
        pthread_mutex_timedlock(&mutex, &ts);
        pthread_mutex_unlock(&mutex);
    }
}

static void Bm_function_pthread_rwlock_tryrdlock(benchmark::State &state)
{
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, nullptr);
    while (state.KeepRunning()) {
        pthread_rwlock_tryrdlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
    }
    pthread_rwlock_destroy(&rwlock);
}

static void Bm_function_pthread_mutex_init(benchmark::State &state)
{
    pthread_mutex_t mutex;

    for (auto _ : state) {
        pthread_mutex_init(&mutex, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void BM_pthread_rwlock_tryread(benchmark::State& state)
{
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_tryrdlock(&lock);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void BM_pthread_rwlock_trywrite(benchmark::State& state)
{
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_trywrlock(&lock);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_tss_get(benchmark::State &state)
{
    tss_t key;
    tss_create(&key, nullptr);

    while (state.KeepRunning()) {
        tss_get(key);
    }

    tss_delete(key);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_timedrdlock(benchmark::State &state)
{
    struct timespec tout;
    time(&tout.tv_sec);
    tout.tv_nsec = 0;
    tout.tv_sec += 1;
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_timedrdlock(&lock, &tout);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_rwlock_timedwrlock(benchmark::State &state)
{
    struct timespec tout;
    time(&tout.tv_sec);
    tout.tv_nsec = 0;
    tout.tv_sec += 1;
    pthread_rwlock_t lock;
    pthread_rwlock_init(&lock, nullptr);

    while (state.KeepRunning()) {
        pthread_rwlock_timedwrlock(&lock, &tout);
        pthread_rwlock_unlock(&lock);
    }

    pthread_rwlock_destroy(&lock);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_pthread_cond_timedwait(benchmark::State &state)
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 1;
    pthread_mutex_lock(&mutex);
    while (state.KeepRunning()) {
        pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    state.SetBytesProcessed(state.iterations());
}

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
void* thread_task1(void* arg)
{
    pthread_mutex_lock(&mutex1);
    pthread_cond_wait(&cond, &mutex1);
    sleep(1);
    pthread_mutex_unlock(&mutex1);
    return nullptr;
}

void* thread_task2(void* arg)
{
    pthread_mutex_lock(&mutex2);
    pthread_cond_wait(&cond, &mutex2);
    sleep(1);
    pthread_mutex_unlock(&mutex2);
    return nullptr;
}

void* broadcastNotifyMutex(void* arg)
{
    benchmark::State *statePtr = (benchmark::State *)arg;
    statePtr->ResumeTiming();
    benchmark::DoNotOptimize(pthread_cond_broadcast(&cond));
    statePtr->PauseTiming();
    return nullptr;
}

static void Bm_function_pthread_cond_broadcast(benchmark::State &state)
{
    while (state.KeepRunning()) {
        state.PauseTiming();
        pthread_t thread_1, thread_2, thread_3;
        pthread_create(&thread_1, nullptr, thread_task1, nullptr);
        pthread_create(&thread_2, nullptr, thread_task2, nullptr);
        sleep(3);
        pthread_create(&thread_3, nullptr, broadcastNotifyMutex, &state);
        pthread_join(thread_1, nullptr);
        pthread_join(thread_2, nullptr);
        pthread_join(thread_3, nullptr);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sem_timewait(benchmark::State &state)
{
    sem_t semp;
    sem_init(&semp, 0, 1);
    struct timespec spec;
    spec.tv_sec = 0;
    spec.tv_nsec = 5;
    while (state.KeepRunning()) {
        sem_timedwait(&semp, &spec);
    }
    sem_destroy(&semp);
}

static void Bm_function_Sem_post_wait(benchmark::State &state)
{
    sem_t semp;
    sem_init(&semp, 0, 0);
    struct timespec spec;
    spec.tv_sec = 0;
    spec.tv_nsec = 5;
    while (state.KeepRunning()) {
        sem_post(&semp);
        sem_wait(&semp);
    }
    sem_destroy(&semp);
}

static void Bm_function_pthread_cond_signal(benchmark::State &state)
{
    pthread_cond_t cond;
    pthread_cond_init(&cond, NULL);
    while (state.KeepRunning())
    {
        pthread_cond_signal(&cond);
    }
    pthread_cond_destroy(&cond);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sigaddset(benchmark::State &state)
{
    sigset_t set;
    sigemptyset(&set);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sigaddset(&set, SIGUSR1));
    }
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Sigaddset);
MUSL_BENCHMARK(Bm_function_pthread_cond_signal);
MUSL_BENCHMARK(Bm_function_pthread_mutexattr_settype);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_errorcheck);
MUSL_BENCHMARK(Bm_function_pthread_mutex_trylock_pi_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_rec);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_fast);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_errchk);
MUSL_BENCHMARK(Bm_function_pthread_mutex_timedlock_pi_rec);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_tryrdlock);
MUSL_BENCHMARK(Bm_function_pthread_mutex_init);
MUSL_BENCHMARK(BM_pthread_rwlock_tryread);
MUSL_BENCHMARK(BM_pthread_rwlock_trywrite);
MUSL_BENCHMARK(Bm_function_tss_get);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_timedrdlock);
MUSL_BENCHMARK(Bm_function_pthread_rwlock_timedwrlock);
MUSL_BENCHMARK(Bm_function_pthread_cond_timedwait);
MUSL_BENCHMARK(Bm_function_pthread_cond_broadcast);
MUSL_BENCHMARK(Bm_function_Sem_timewait);
MUSL_BENCHMARK(Bm_function_Sem_post_wait);
#endif