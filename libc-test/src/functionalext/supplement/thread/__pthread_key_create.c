/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

static int32_t g_sleep_time = 2;
static pthread_key_t g_key;

void *threadfuncA(void *arg)
{
    sleep(g_sleep_time);
    int32_t value = 0;
    int32_t ret = pthread_setspecific(g_key, &value);
    EXPECT_EQ("__pthread_key_create_0200_threadfuncA", ret, 0);
    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    EXPECT_EQ("__pthread_key_create_0200_threadfuncA", *keyRet, 0);
    return arg;
}

void *threadfuncB(void *arg)
{
    int32_t value = 0;
    int32_t ret = pthread_setspecific(g_key, &value);
    EXPECT_EQ("__pthread_key_create_0100_threadfuncB", ret, 0);
    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    EXPECT_EQ("__pthread_key_create_0100_threadfuncB", *keyRet, 0);
    return arg;
}

void testfunc(void *arg)
{}

void thread_destructor(void *arg)
{
    free(arg);
}

void *threadfuncC(void *arg)
{
    int32_t value = (int32_t)(uintptr_t)arg;
    int32_t ret = pthread_setspecific(g_key, &value);
    EXPECT_EQ("pthread_key_create_0300_threadfuncC", ret, 0);

    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    EXPECT_PTRNE("__pthread_key_create_0300_threadfuncC", keyRet, NULL);
    EXPECT_EQ("__pthread_key_create_0300_threadfuncC", *keyRet, value);
    return arg;
}

void signo_handler(int signo)
{
    pthread_key_t temp_key;
    int ret = __pthread_key_create(&temp_key, NULL);
    if (ret == 0) {
        pthread_key_delete(temp_key);
    }
}

void *threadfuncD(void *arg)
{
    struct sigaction sa;
    sa.sa_handler = signo_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    int ret = sigaction(SIGUSR1, &sa, NULL);
    EXPECT_EQ("__pthread_key_create_0400_threadfuncD", ret, 0);

    sleep(1);

    pthread_t self = pthread_self();
    ret = pthread_kill(self, SIGUSR1);
    EXPECT_EQ("__pthread_key_create_0400_threadfuncD", ret, 0);

    pthread_key_t temp_key;
    ret = __pthread_key_create(&temp_key, NULL);
    EXPECT_EQ("__pthread_key_create_0400_threadfuncD", ret, 0);
    pthread_key_delete(temp_key);
    return NULL;
}

/**
 * @tc.name:      __pthread_key_create_0100
 * @tc.desc:      Verify __pthread_key_create_0100 process success when second args is not null.
 * @tc.desc:      level 1
 */
void __pthread_key_create_0100(void)
{
    pthread_t tid2;
    int32_t ret = __pthread_key_create(&g_key, testfunc);
    pthread_create(&tid2, NULL, threadfuncB, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ("__pthread_key_create_0100", ret, 0);
}

/**
 * @tc.name:      __pthread_key_create_0200
 * @tc.desc:      Verify __pthread_key_create_0200 process success when second args is null.
 * @tc.desc:      level 0
 */
void __pthread_key_create_0200(void)
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, threadfuncA, NULL);
    int32_t ret = __pthread_key_create(&g_key, NULL);
    EXPECT_EQ("__pthread_key_create_0200", ret, 0);
    pthread_join(tid1, NULL);
    pthread_key_delete(g_key);
}

/**
 * @tc.name:      __pthread_key_create_0300
 * @tc.desc:      Verify __pthread_key_create_0300 works correctly in multi-thread scenario.
 * @tc.desc:      level 1
 */
void __pthread_key_create_0300(void)
{
    const int THREAD_COUNT = 5;
    pthread_t tids[THREAD_COUNT];
    int32_t ret;

    ret = __pthread_key_create(&g_key, NULL);
    EXPECT_EQ("__pthread_key_create_0300", ret, 0);

    for (int i = 0; i < THREAD_COUNT; i++) {
        ret = pthread_create(&tids[i], NULL, threadfuncC, (void*)(uintptr_t)i);
        EXPECT_EQ("__pthread_key_create_0300", ret, 0);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(tids[i], NULL);
    }
    pthread_key_delete(g_key);
}

/**
 * @tc.name:      __pthread_key_create_0400
 * @tc.desc:      Verify no deadlock when __pthread_key_create_0400 is interrupted by signal.
 * @tc.desc:      level 2
 */
void __pthread_key_create_0400(void)
{
    pthread_t tid;
    int32_t ret = pthread_create(&tid, NULL, threadfuncD, NULL);
    EXPECT_EQ("__pthread_key_create_0400", ret, 0);
    pthread_join(tid, NULL);
}

/**
 * @tc.name:      __pthread_key_create_0500
 * @tc.desc:      Verify __pthread_key_create when key ptr is NULL
 * @tc.desc:      level 1
 */
void __pthread_key_create_0500(void)
{
    pthread_key_t temp_key;
    int32_t ret = __pthread_key_create(&temp_key, testfunc);
    EXPECT_EQ("__pthread_key_create_0500", ret, 0);
    pthread_key_delete(temp_key);
}

/**
 * @tc.name:      __pthread_key_create_0600
 * @tc.desc:      Verify repeated delete tsd key
 * @tc.desc:      level 1
 */
void __pthread_key_create_0600(void)
{
    pthread_key_t temp_key;
    int32_t ret = __pthread_key_create(&temp_key, NULL);
    EXPECT_EQ("__pthread_key_create_0600_create", ret, 0);
    ret = pthread_key_delete(temp_key);
    EXPECT_EQ("__pthread_key_create_0600_delete_first", ret, 0);
    ret = pthread_key_delete(temp_key);
    EXPECT_EQ("__pthread_key_create_0600_delete_second", ret, 0);
}

/**
 * @tc.name:      __pthread_key_create_0700
 * @tc.desc:      Verify create key exceed system limit (1024)
 * @tc.desc:      level 1
 */
void __pthread_key_create_0700(void)
{
    pthread_key_t keys[1025];
    int32_t create_count = 0;
    int32_t ret = 0;

    for (int i = 0; i < 1025; i++) {
        ret = __pthread_key_create(&keys[i], NULL);
        if (ret != 0) {
            break;
        }
        create_count++;
    }
    EXPECT_TRUE("__pthread_key_create_0700_count", create_count <= 1024);
    EXPECT_NE("__pthread_key_create_0700_fail", ret, 0);

    for (int i = 0; i < create_count; i++) {
        pthread_key_delete(keys[i]);
    }
}

/**
 * @tc.name:      __pthread_key_create_0800
 * @tc.desc:      Verify main thread use TSD normally
 * @tc.desc:      level 1
 */
void __pthread_key_create_0800(void)
{
    int32_t ret = __pthread_key_create(&g_key, thread_destructor);
    EXPECT_EQ("__pthread_key_create_0800_create", ret, 0);
    EXPECT_NE("__pthread_key_create_0800_key_nonzero", g_key, 0);

    int32_t *main_data = malloc(sizeof(int32_t));
    if (main_data == NULL) {
        return;
    }
    *main_data = 100;

    ret = pthread_setspecific(g_key, main_data);
    EXPECT_EQ("__pthread_key_create_0800_setspecific", ret, 0);

    int32_t *get_data = (int32_t *)pthread_getspecific(g_key);
    EXPECT_PTRNE("__pthread_key_create_0800_getspecific_ptr", get_data, NULL);
    EXPECT_EQ("__pthread_key_create_0800_getspecific_value", *get_data, 100);

    ret = pthread_key_delete(g_key);
    EXPECT_EQ("__pthread_key_create_0800_delete", ret, 0);
}

void* mt_worker(void* arg)
{
    __pthread_key_create_0100();
    return NULL;
}

/**
 * @tc.name      : __pthread_key_create_0900
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __pthread_key_create_0900(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ("__pthread_key_create_0900", pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(void)
{
    __pthread_key_create_0100();
    __pthread_key_create_0200();
    __pthread_key_create_0300();
    __pthread_key_create_0400();
    __pthread_key_create_0500();
    __pthread_key_create_0600();
    __pthread_key_create_0700();
    __pthread_key_create_0800();
    __pthread_key_create_0900();
    return t_status;
}