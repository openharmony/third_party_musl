/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#ifdef MUSL_EXTERNAL_FUNCTION
// pthread_cancel_testcase5_multiple_cancel.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "functionalext.h"

void* thread_func(void* arg) {
    for (int i = 0; i < 2000; i++) {
        usleep(1000); // 1ms
    }
    return NULL;
}

int main()
{
    set_pthread_extended_function_policy(1);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    pthread_cancel(tid); // first
    pthread_cancel(tid); // second
    pthread_cancel(tid); // third

    void* result;
    pthread_join(tid, &result);
    // It should return PTHREAD_CANCELED
    EXPECT_EQ("pthread_cancel_testcase5_multiple_cancel", result, PTHREAD_CANCELED);
    return t_status;
}
#endif