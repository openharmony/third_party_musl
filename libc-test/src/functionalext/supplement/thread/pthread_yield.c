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
#include <pthread.h>
#include "functionalext.h"

#ifdef MUSL_EXTERNAL_FUNCTION
#define YIELD_THREAD_NUM (4)
#define YIELD_LOOP_NUM (1000)

static void *pthread_yield_thread(void *arg)
{
    (void)arg;
    int ret = pthread_yield();
    EXPECT_EQ("pthread_yield_0200", ret, 0);
    return NULL;
}
#endif

#ifndef MUSL_EXTERNAL_FUNCTION
/**
 * @tc.name      : pthread_yield_0100
 * @tc.desc      : Verify pthread_yield returns ENOSYS when MUSL_EXTERNAL_FUNCTION is disabled
 * @tc.level     : Level 0
 */
void pthread_yield_0100(void)
{
    int ret = pthread_yield();
    EXPECT_EQ("pthread_yield_0100", ret, ENOSYS);
}
#else
/**
 * @tc.name      : pthread_yield_0100
 * @tc.desc      : Verify pthread_yield returns 0 on a single call
 * @tc.level     : Level 0
 */
void pthread_yield_0100(void)
{
    int ret = pthread_yield();
    EXPECT_EQ("pthread_yield_0100", ret, 0);
}

/**
 * @tc.name      : pthread_yield_0200
 * @tc.desc      : Verify pthread_yield can be called from multiple threads
 *                 and that all threads exit normally
 * @tc.level     : Level 0
 */
void pthread_yield_0200(void)
{
    pthread_t tid[YIELD_THREAD_NUM];
    int created = 0;

    for (int i = 0; i < YIELD_THREAD_NUM; i++) {
        int ret = pthread_create(&tid[i], NULL, pthread_yield_thread, NULL);
        EXPECT_EQ("pthread_yield_0200_create", ret, 0);
        if (ret != 0) {
            break;
        }
        created++;
    }

    for (int i = 0; i < created; i++) {
        int ret = pthread_join(tid[i], NULL);
        EXPECT_EQ("pthread_yield_0200_join", ret, 0);
    }
}

/**
 * @tc.name      : pthread_yield_0300
 * @tc.desc      : Verify repeated consecutive pthread_yield calls all return 0
 * @tc.level     : Level 0
 */
void pthread_yield_0300(void)
{
    for (int i = 0; i < YIELD_LOOP_NUM; i++) {
        int ret = pthread_yield();
        EXPECT_EQ("pthread_yield_0300", ret, 0);
    }
}
#endif

int main(void)
{
#ifndef MUSL_EXTERNAL_FUNCTION
    pthread_yield_0100();
#else
    pthread_yield_0100();
    pthread_yield_0200();
    pthread_yield_0300();
#endif
    return t_status;
}
