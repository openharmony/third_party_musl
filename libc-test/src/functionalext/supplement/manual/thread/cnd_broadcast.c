/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "functionalext.h"

mtx_t mtx;
cnd_t cndPut;
void cnd_timedwaitfirst(void *arg)
{
    int res = -1;
    struct timespec ts;
    mtx_lock(&mtx);
    res = cnd_timedwait(&cndPut, &mtx, &ts);
    mtx_unlock(&mtx);
}

/**
 * @tc.name      : cnd_broadcast_0100
 * @tc.desc      : Validation wakes up all threads waiting for the specified condition variable
 * @tc.level     : Level 0
 */
void cnd_broadcast_0100(void)
{
    thrd_t id1;
    int ret;
    int result = -1;
    cnd_init(&cndPut);
    mtx_init(&mtx, 0);
    ret = thrd_create(&id1, (thrd_start_t)cnd_timedwaitfirst, (void *)1);
    if (0 != ret) {
        printf("thread 1 create failed!\n");
    }
    sleep(1);
    mtx_lock(&mtx);
    result = cnd_broadcast(&cndPut);
    EXPECT_EQ("cnd_broadcast_0100", result, thrd_success);
    mtx_unlock(&mtx);
    thrd_join(id1, NULL);
    cnd_destroy(&cndPut);
    mtx_destroy(&mtx);
}

int main(int argc, char *argv[])
{
    cnd_broadcast_0100();
    return t_status;
}