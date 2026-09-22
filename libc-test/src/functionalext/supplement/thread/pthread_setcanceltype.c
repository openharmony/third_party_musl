/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include "functionalext.h"

#ifdef MUSL_EXTERNAL_FUNCTION
#define SETCANCEL_LOOP_NUM (1000)
#define SETCANCEL_THREAD_NUM (4)
#define SETCANCEL_WAIT_NUM (100000)

static void *pthread_setcanceltype_thread(void *arg)
{
    (void)arg;
    int old = -1;
    int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    EXPECT_EQ("pthread_setcanceltype_0400_set_async", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0400_old_deferred", old, PTHREAD_CANCEL_DEFERRED);

    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
    EXPECT_EQ("pthread_setcanceltype_0400_set_deferred", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0400_old_async", old, PTHREAD_CANCEL_ASYNCHRONOUS);
    return NULL;
}

static volatile int g_cancelAsyncReady = 0;
static volatile int g_cancelAsyncGo = 0;

static void *pthread_setcanceltype_cancel_async_thread(void *arg)
{
    (void)arg;
    g_cancelAsyncReady = 1;
    while (g_cancelAsyncGo == 0) {
        (void)sched_yield();
    }
    (void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    return (void *)1;
}

static volatile int g_asyncCancelReady = 0;

static void *pthread_setcanceltype_async_cancel_thread(void *arg)
{
    (void)arg;
    (void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    g_asyncCancelReady = 1;
    for (int i = 0; i < SETCANCEL_WAIT_NUM; i++) {
        (void)sched_yield();
    }
    return (void *)1;
}

static volatile int g_deferredReady = 0;
static volatile int g_deferredGo = 0;
static volatile int g_deferredReachedTestcancel = 0;

static void *pthread_setcanceltype_deferred_thread(void *arg)
{
    (void)arg;
    g_deferredReady = 1;
    while (g_deferredGo == 0) {
        (void)sched_yield();
    }
    g_deferredReachedTestcancel = 1;
    pthread_testcancel();
    return (void *)1;
}
#endif

#ifndef MUSL_EXTERNAL_FUNCTION
/*
 * @tc.name      : pthread_setcanceltype_0100
 * @tc.desc      : Verify pthread_setcanceltype returns ENOSYS when MUSL_EXTERNAL_FUNCTION is disabled
 * @tc.level     : Level 0
 */
void pthread_setcanceltype_0100(void)
{
    int old = -1;
    int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    EXPECT_EQ("pthread_setcanceltype_0100", ret, ENOSYS);
}
#else
/*
 * @tc.name      : pthread_setcanceltype_0100
 * @tc.desc      : Verify setting deferred/async type returns old value
 * @tc.level     : Level 0
 */
void pthread_setcanceltype_0100(void)
{
    int old = -1;
    int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    EXPECT_EQ("pthread_setcanceltype_0100_set_async", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0100_old_deferred", old, PTHREAD_CANCEL_DEFERRED);

    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
    EXPECT_EQ("pthread_setcanceltype_0100_set_deferred", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0100_old_async", old, PTHREAD_CANCEL_ASYNCHRONOUS);
}

/*
 * @tc.name      : pthread_setcanceltype_0200
 * @tc.desc      : Verify invalid type returns EINVAL and leaves oldtype
 *                 and current cancel type unchanged
 * @tc.level     : Level 0
 */
void pthread_setcanceltype_0200(void)
{
    int old = -1;
    int ret = pthread_setcanceltype(2, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_einval", ret, EINVAL);
    EXPECT_EQ("pthread_setcanceltype_0200_old_unchanged", old, -1);

    ret = pthread_setcanceltype(-1, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_neg_einval", ret, EINVAL);
    EXPECT_EQ("pthread_setcanceltype_0200_neg_old_unchanged", old, -1);

    ret = pthread_setcanceltype(INT_MIN, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_intmin_einval", ret, EINVAL);
    EXPECT_EQ("pthread_setcanceltype_0200_intmin_old_unchanged", old, -1);

    ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_set_async", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0200_old_deferred", old, PTHREAD_CANCEL_DEFERRED);

    ret = pthread_setcanceltype(2, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_einval_after_async", ret, EINVAL);

    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
    EXPECT_EQ("pthread_setcanceltype_0200_restore_deferred", ret, 0);
    EXPECT_EQ("pthread_setcanceltype_0200_old_still_async", old, PTHREAD_CANCEL_ASYNCHRONOUS);
}

/*
 * @tc.name      : pthread_setcanceltype_0300
 * @tc.desc      : Verify NULL oldtype is accepted
 * @tc.level     : Level 1
 */
void pthread_setcanceltype_0300(void)
{
    int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    EXPECT_EQ("pthread_setcanceltype_0300_null_oldtype", ret, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
}

/*
 * @tc.name      : pthread_setcanceltype_0400
 * @tc.desc      : Verify pthread_setcanceltype can be called from multiple
 *                 threads and each thread keeps its own cancel type
 * @tc.level     : Level 0
 */
void pthread_setcanceltype_0400(void)
{
    pthread_t tid[SETCANCEL_THREAD_NUM];
    int created = 0;

    for (int i = 0; i < SETCANCEL_THREAD_NUM; i++) {
        int ret = pthread_create(&tid[i], NULL, pthread_setcanceltype_thread, NULL);
        EXPECT_EQ("pthread_setcanceltype_0400_create", ret, 0);
        if (ret != 0) {
            break;
        }
        created++;
    }

    for (int i = 0; i < created; i++) {
        int ret = pthread_join(tid[i], NULL);
        EXPECT_EQ("pthread_setcanceltype_0400_join", ret, 0);
    }
}

/*
 * @tc.name      : pthread_setcanceltype_0500
 * @tc.desc      : Verify repeated consecutive pthread_setcanceltype calls
 *                 return the previous type correctly
 * @tc.level     : Level 0
 */
void pthread_setcanceltype_0500(void)
{
    int old = -1;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (int i = 0; i < SETCANCEL_LOOP_NUM; i++) {
        int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
        EXPECT_EQ("pthread_setcanceltype_0500_set_async", ret, 0);
        EXPECT_EQ("pthread_setcanceltype_0500_old_deferred", old, PTHREAD_CANCEL_DEFERRED);

        ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
        EXPECT_EQ("pthread_setcanceltype_0500_set_deferred", ret, 0);
        EXPECT_EQ("pthread_setcanceltype_0500_old_async", old, PTHREAD_CANCEL_ASYNCHRONOUS);
    }
}

/*
 * @tc.name      : pthread_setcanceltype_0600
 * @tc.desc      : Verify a pending cancellation request is acted upon
 *                 immediately when switching to PTHREAD_CANCEL_ASYNCHRONOUS
 * @tc.level     : Level 1
 */
void pthread_setcanceltype_0600(void)
{
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_setcanceltype_0600_policy", get_pthread_extended_function_policy(), 1);

    g_cancelAsyncReady = 0;
    g_cancelAsyncGo = 0;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, pthread_setcanceltype_cancel_async_thread, NULL);
    EXPECT_EQ("pthread_setcanceltype_0600_create", ret, 0);
    if (ret != 0) {
        set_pthread_extended_function_policy(0);
        return;
    }

    for (int i = 0; i < SETCANCEL_LOOP_NUM && g_cancelAsyncReady == 0; i++) {
        (void)sched_yield();
    }
    EXPECT_EQ("pthread_setcanceltype_0600_ready", g_cancelAsyncReady, 1);

    ret = pthread_cancel(tid);
    EXPECT_EQ("pthread_setcanceltype_0600_cancel", ret, 0);

    g_cancelAsyncGo = 1;

    void *result = NULL;
    ret = pthread_join(tid, &result);
    EXPECT_EQ("pthread_setcanceltype_0600_join", ret, 0);
    EXPECT_PTREQ("pthread_setcanceltype_0600_canceled", result, PTHREAD_CANCELED);

    set_pthread_extended_function_policy(0);
}

/*
 * @tc.name      : pthread_setcanceltype_0700
 * @tc.desc      : Verify an already-asynchronous thread is canceled
 *                 immediately without reaching a cancellation point
 * @tc.level     : Level 1
 */
void pthread_setcanceltype_0700(void)
{
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_setcanceltype_0700_policy", get_pthread_extended_function_policy(), 1);

    g_asyncCancelReady = 0;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, pthread_setcanceltype_async_cancel_thread, NULL);
    EXPECT_EQ("pthread_setcanceltype_0700_create", ret, 0);
    if (ret != 0) {
        set_pthread_extended_function_policy(0);
        return;
    }

    for (int i = 0; i < SETCANCEL_WAIT_NUM && g_asyncCancelReady == 0; i++) {
        (void)sched_yield();
    }
    EXPECT_EQ("pthread_setcanceltype_0700_ready", g_asyncCancelReady, 1);

    ret = pthread_cancel(tid);
    EXPECT_EQ("pthread_setcanceltype_0700_cancel", ret, 0);

    void *result = NULL;
    ret = pthread_join(tid, &result);
    EXPECT_EQ("pthread_setcanceltype_0700_join", ret, 0);
    EXPECT_PTREQ("pthread_setcanceltype_0700_canceled", result, PTHREAD_CANCELED);

    set_pthread_extended_function_policy(0);
}

/*
 * @tc.name      : pthread_setcanceltype_0800
 * @tc.desc      : Verify deferred cancellation is held until an explicit
 *                 cancellation point (pthread_testcancel)
 * @tc.level     : Level 1
 */
void pthread_setcanceltype_0800(void)
{
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_setcanceltype_0800_policy", get_pthread_extended_function_policy(), 1);

    g_deferredReady = 0;
    g_deferredGo = 0;
    g_deferredReachedTestcancel = 0;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, pthread_setcanceltype_deferred_thread, NULL);
    EXPECT_EQ("pthread_setcanceltype_0800_create", ret, 0);
    if (ret != 0) {
        set_pthread_extended_function_policy(0);
        return;
    }

    for (int i = 0; i < SETCANCEL_LOOP_NUM && g_deferredReady == 0; i++) {
        (void)sched_yield();
    }
    EXPECT_EQ("pthread_setcanceltype_0800_ready", g_deferredReady, 1);

    ret = pthread_cancel(tid);
    EXPECT_EQ("pthread_setcanceltype_0800_cancel", ret, 0);

    g_deferredGo = 1;

    void *result = NULL;
    ret = pthread_join(tid, &result);
    EXPECT_EQ("pthread_setcanceltype_0800_join", ret, 0);
    EXPECT_PTREQ("pthread_setcanceltype_0800_canceled", result, PTHREAD_CANCELED);
    EXPECT_EQ("pthread_setcanceltype_0800_reached_testcancel", g_deferredReachedTestcancel, 1);

    set_pthread_extended_function_policy(0);
}
#endif

int main(int argc, char *argv[])
{
#ifndef MUSL_EXTERNAL_FUNCTION
    pthread_setcanceltype_0100();
#else
    pthread_setcanceltype_0100();
    pthread_setcanceltype_0200();
    pthread_setcanceltype_0300();
    pthread_setcanceltype_0400();
    pthread_setcanceltype_0500();
    pthread_setcanceltype_0600();
    pthread_setcanceltype_0700();
    pthread_setcanceltype_0800();
#endif
    return t_status;
}