/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_THREAD_SLEEP 3
#ifdef FEATURE_PTHREAD_CANCEL
#undef FEATURE_PTHREAD_CANCEL
#endif

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;
static int gprepare_flag = 0;
static int gparent_flag = 0;
static int gchild_flag = 0;
static int prepare_count = 0;
static int parent_count = 0;
static int child_count = 0;
static int parent_order_step = 0;
static int child_order_step = 0;
static int stable_prepare = 0;
static int stable_parent = 0;
static int stable_child = 0;
static int prepare_order[2] = {0};
static int parent_order[2] = {0};
static int child_order[2] = {0};

void reset_data(void)
{
    gprepare_flag = 0;
    gparent_flag = 0;
    gchild_flag = 0;
}

void reset_count_data(void)
{
    prepare_count = 0;
    parent_count = 0;
    child_count = 0;
}

void reset_order_data(void)
{
    parent_order_step = 0;
    child_order_step = 0;
    prepare_order[0] = prepare_order[1] = 0;
    parent_order[0] = parent_order[1] = 0;
    child_order[0] = child_order[1] = 0;
}

static void prepare(void)
{
    int ret = pthread_mutex_lock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gprepare_flag++;
}

static void parent(void)
{
    int ret = pthread_mutex_unlock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gparent_flag++;
}

static void child(void)
{
    int ret = pthread_mutex_unlock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gchild_flag++;
}

static void prepare_count_cb(void)
{
    prepare_count++;
}

static void parent_count_cb(void)
{
    parent_count++;
}

static void child_count_cb(void)
{
    child_count++;
}

static void stable_prepare_cb(void) 
{ 
    stable_prepare++; 
}

static void stable_parent_cb(void)
{
    stable_parent++;
}

static void stable_child_cb(void)
{
    stable_child++;
}

static void prepare_order_cb1(void)
{
    prepare_order[0] = ++parent_order_step;
}

static void prepare_order_cb2(void)
{
    prepare_order[1] = ++parent_order_step;
}

static void parent_order_cb1(void)
{
    parent_order[0] = ++parent_order_step;
}

static void parent_order_cb2(void)
{
    parent_order[1] = ++parent_order_step;
}

static void child_order_cb1(void)
{
    child_order[0] = ++child_order_step;
}

static void child_order_cb2(void)
{
    child_order[1] = ++child_order_step;
}

static void *thread_fun(void *arg)
{
#ifdef FEATURE_PTHREAD_CANCEL
    while (1) {
        pthread_testcancel();
        pthread_mutex_lock(&glock);
        sleep(TEST_THREAD_SLEEP);
        pthread_mutex_unlock(&glock);
        sleep(1);
    }
#else
    pthread_mutex_lock(&glock);
    sleep(TEST_THREAD_SLEEP);
    pthread_mutex_unlock(&glock);
    sleep(1);
#endif
    return NULL;
}

/**
 * @tc.name      : __register_atfork_0100
 * @tc.desc      : Provide a callback to test the relationship between __register_atfork and fork
 * @tc.level     : Level 1
 */
void __register_atfork_0100(void)
{
    reset_data();
#ifdef FEATURE_PTHREAD_CANCEL
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    EXPECT_EQ("__register_atfork_0100", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    ret = __register_atfork(prepare, parent, child, NULL);
    EXPECT_EQ("__register_atfork_0100", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }
    sleep(1);

    pid_t pid = fork();
    if (pid == 0) {
        pthread_mutex_lock(&glock);
        sleep(1);
        pthread_mutex_unlock(&glock);
        EXPECT_EQ("__register_atfork_0100", gchild_flag, 1);
        _exit(0);
    } else if (pid > 0) {
#ifdef FEATURE_PTHREAD_CANCEL
        wait(NULL);
        pthread_cancel(tid);
#endif
        pthread_join(tid, NULL);

        EXPECT_EQ("__register_atfork_0100", gprepare_flag, 1);
        EXPECT_EQ("__register_atfork_0100", gparent_flag, 1);
    } else {
        EXPECT_FALSE("__register_atfork_0100", 1);
    }
}

/**
 * @tc.name      : __register_atfork_0200
 * @tc.desc      : No callback is provided, test the relationship between __register_atfork and fork
 * @tc.level     : Level 2
 */
void __register_atfork_0200(void)
{
    reset_data();
    int ret = __register_atfork(NULL, NULL, NULL, NULL);
    EXPECT_EQ("__register_atfork_0200", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        EXPECT_EQ("__register_atfork_0200", gchild_flag, 0);
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);

        EXPECT_EQ("__register_atfork_0200", gprepare_flag, 0);
        EXPECT_EQ("__register_atfork_0200", gparent_flag, 0);
    } else {
        EXPECT_FALSE("__register_atfork_0200", 1);
    }
}

/**
 * @tc.name      : __register_atfork_0300
 * @tc.desc      : Register callbacks repeatedly, verify the cumulative execution count of callbacks
 * @tc.level     : Level 1
 */
void __register_atfork_0300(void)
{
    reset_count_data();
    int ret;

    ret = __register_atfork(prepare_count_cb, parent_count_cb, child_count_cb, NULL);
    EXPECT_EQ("__register_atfork_0300", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    ret = __register_atfork(prepare_count_cb, parent_count_cb, child_count_cb, NULL);
    EXPECT_EQ("__register_atfork_0300", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        EXPECT_EQ("__register_atfork_0300", child_count, 2);
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);
        EXPECT_EQ("__register_atfork_0300", prepare_count, 2);
        EXPECT_EQ("__register_atfork_0300", parent_count, 2);
    } else {
        EXPECT_FALSE("__register_atfork_0300", 1);
    }
}

/**
 * @tc.name      : __register_atfork_0400
 * @tc.desc      : Register multiple groups of callbacks, verify the execution order
 * @tc.level     : Level 1
 */
void __register_atfork_0400(void)
{
    reset_order_data();
    int ret;

    ret = __register_atfork(prepare_order_cb1, parent_order_cb1, child_order_cb1, NULL);
    EXPECT_EQ("__register_atfork_0400", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    ret = __register_atfork(prepare_order_cb2, parent_order_cb2, child_order_cb2, NULL);
    EXPECT_EQ("__register_atfork_0400", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        EXPECT_EQ("__register_atfork_0400", child_order[0], 1);
        EXPECT_EQ("__register_atfork_0400", child_order[1], 2);
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);
        EXPECT_EQ("__register_atfork_0400", prepare_order[0], 2);
        EXPECT_EQ("__register_atfork_0400", prepare_order[1], 1);
        EXPECT_EQ("__register_atfork_0400", parent_order[0], 3);
        EXPECT_EQ("__register_atfork_0400", parent_order[1], 4);
    } else {
        EXPECT_FALSE("__register_atfork_0400", 1);
    }
}

/**
 * @tc.name      : __register_atfork_0500
 * @tc.desc      : Register dangerous callback, verify registration behavior
 * @tc.level     : Level 1
 */
void __register_atfork_0500(void)
{
    void (*invalid_cb)(void) = NULL;
    int ret = __register_atfork(invalid_cb, NULL, NULL, NULL);
    EXPECT_EQ("__register_atfork_0500", ret, CMPFLAG);
}

/**
 * @tc.name      : __register_atfork_0600
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 1
 */
void __register_atfork_0600(void) {
    stable_prepare = 0;
    stable_parent = 0;
    stable_child = 0;
    const int LOOP_NUM = 10;
    int ret;

    ret = __register_atfork(stable_prepare_cb, stable_parent_cb, stable_child_cb, NULL);
    EXPECT_EQ("__register_atfork_0600_reg", ret, CMPFLAG);
    if (ret != 0) {
        EXPECT_FALSE("__register_atfork_0600", 1);
        return;
    }

    for (int i = 0; i < LOOP_NUM; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            EXPECT_FALSE("__register_atfork_0600_fork", 1);
            return;
        }

        if (pid == 0) {
            _exit(0);
        } else {
            wait(NULL);
            EXPECT_EQ("__register_atfork_0600_prepare", stable_prepare, i + 1);
            EXPECT_EQ("__register_atfork_0600_parent", stable_parent, i + 1);
        }
    }

    EXPECT_EQ("__register_atfork_0600_total", stable_prepare, LOOP_NUM);
    EXPECT_EQ("__register_atfork_0600_total", stable_parent, LOOP_NUM);
}

int main(void)
{
    __register_atfork_0200();
    __register_atfork_0100();
    __register_atfork_0300();
    __register_atfork_0400();
    __register_atfork_0500();
    __register_atfork_0600();
    return t_status;
}