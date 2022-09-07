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

#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include "test.h"

static thrd_t thr;
static int count = 0;

int threadfuncA(void *arg)
{
    count++;
    thrd_t id = thrd_current();

    if (!(thrd_equal(id, thr))) {
        t_error("%s thrd_current failed", __func__);
    }

    thrd_exit(thrd_success);
}

int threadfuncB(void *arg)
{
    count++;

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : thrd_current_0100
 * @tc.desc      : Returns the identifier of the calling thread
 * @tc.level     : Level 0
 */
void thrd_current_0100(void)
{
    int result;

    result = thrd_create(&thr, threadfuncA, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(thr, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

/**
 * @tc.name      : thrd_current_0200
 * @tc.desc      : Create two threads to execute the same func
 * @tc.level     : Level 1
 */
void thrd_current_0200(void)
{
    thrd_t thr1, thr2;
    int result;

    result = thrd_create(&thr1, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_create(&thr2, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    if (thrd_equal(thr1, thr2)) {
        t_error("%s equal", __func__);
    }

    if (thrd_equal(thr1, thrd_current())) {
        t_error("%s equal", __func__);
    }

    if (thrd_equal(thr2, thrd_current())) {
        t_error("%s equal", __func__);
    }

    result = thrd_join(thr1, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    result = thrd_join(thr2, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 2) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

int main(int argc, char *argv[])
{
    thrd_current_0100();
    thrd_current_0200();
    return t_status;
}