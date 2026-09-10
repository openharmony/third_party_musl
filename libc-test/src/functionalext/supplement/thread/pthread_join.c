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

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "test.h"
#include "functionalext.h"

pthread_t pid;
int flag = 0;

void *threadfunc(void *arg)
{
    sleep(2);
    flag++;
    return 0;
}

static sem_t g_detachBlockSem;
static sem_t g_detachDoneSem;

void *threadfunc_detach_block(void *arg)
{
    sem_wait(&g_detachBlockSem);
    sem_post(&g_detachDoneSem);
    return 0;
}

/**
 * @tc.name      : pthread_join_0100
 * @tc.desc      : The parameters are valid, verify that the main thread does not wait for other threads to end the
 * scene
 * @tc.level     : Level 0
 */
void pthread_join_0100(void)
{
    flag = 0;
    pthread_create(&pid, NULL, threadfunc, NULL);
    int result = pthread_join(pid, NULL);
    if (result != 0) {
        t_error("%s pthread_join error get result is %d are not want 0\n", __func__, result);
    }
    if (flag != 1) {
        t_error("%s pthread_join error get flag is %d are not want 1\n", __func__, flag);
    }
}

/**
 * @tc.name      : pthread_join_0200
 * @tc.desc      : The input parameter pthread_t is NULL, pthread_join() returns errno code ESRCH
 * @tc.level     : Level 0
 */
void pthread_join_0200(void)
{
    pthread_t t = NULL;
    int result = pthread_join(t, NULL);
    if (result != ESRCH) {
        t_error("%s pthread_join error get result is %d are not want ESRCH\n", __func__, result);
    }
}

/**
 * @tc.name      : pthread_join_0300
 * @tc.desc      : A detached thread is passed to pthread_join(), which returns EINVAL
 * @tc.level     : Level 0
 */
void pthread_join_0300(void)
{
    sem_init(&g_detachBlockSem, 0, 0);
    sem_init(&g_detachDoneSem, 0, 0);
    pthread_t t;
    int createResult = pthread_create(&t, NULL, threadfunc_detach_block, NULL);
    if (createResult != 0) {
        t_error("%s pthread_create error get result is %d are not want 0\n", __func__, createResult);
        sem_destroy(&g_detachBlockSem);
        sem_destroy(&g_detachDoneSem);
        return;
    }
    int detachResult = pthread_detach(t);
    if (detachResult != 0) {
        t_error("%s pthread_detach error get result is %d are not want 0\n", __func__, detachResult);
        sem_post(&g_detachBlockSem);
        pthread_join(t, NULL);
        sem_destroy(&g_detachBlockSem);
        sem_destroy(&g_detachDoneSem);
        return;
    }
    int result = pthread_join(t, NULL);
    if (result != EINVAL) {
        t_error("%s pthread_join error get result is %d are not want EINVAL\n", __func__, result);
    }
    sem_post(&g_detachBlockSem);
    sem_wait(&g_detachDoneSem);
    sem_destroy(&g_detachBlockSem);
    sem_destroy(&g_detachDoneSem);
}

/**
 * @tc.name      : pthread_join_0400
 * @tc.desc      : A thread created with detached attribute is passed to pthread_join(), which returns EINVAL
 * @tc.level     : Level 0
 */
void pthread_join_0400(void)
{
    sem_init(&g_detachBlockSem, 0, 0);
    sem_init(&g_detachDoneSem, 0, 0);
    pthread_attr_t attr;
    pthread_t t;
    pthread_attr_init(&attr);
    int setResult = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (setResult != 0) {
        t_error("%s pthread_attr_setdetachstate error get result is %d are not want 0\n", __func__, setResult);
    }
    int createResult = pthread_create(&t, &attr, threadfunc_detach_block, NULL);
    if (createResult != 0) {
        t_error("%s pthread_create error get result is %d are not want 0\n", __func__, createResult);
        pthread_attr_destroy(&attr);
        sem_destroy(&g_detachBlockSem);
        sem_destroy(&g_detachDoneSem);
        return;
    }
    pthread_attr_destroy(&attr);
    int result = pthread_join(t, NULL);
    if (result != EINVAL) {
        t_error("%s pthread_join error get result is %d are not want EINVAL\n", __func__, result);
    }
    sem_post(&g_detachBlockSem);
    sem_wait(&g_detachDoneSem);
    sem_destroy(&g_detachBlockSem);
    sem_destroy(&g_detachDoneSem);
}

int main(int argc, char *argv[])
{
    pthread_join_0100();
    pthread_join_0200();
    pthread_join_0300();
    pthread_join_0400();
    return t_status;
}