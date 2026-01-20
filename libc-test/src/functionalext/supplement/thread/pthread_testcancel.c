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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "functionalext.h"

static void* worker_thread(void* arg)
{
    printf("Worker thread started...\n");

    for (int i = 0; i < 1000; ++i) {
        printf("Worker thread: loop %d\n", i);
        usleep(3000);  // 3 ms
        pthread_testcancel();  // explicit cancellation point
    }

    printf("Worker thread finished normally (not canceled).\n");
    return NULL;
}

int main() {
    pthread_t tid;
    set_pthread_extended_function_policy(1);
    if (pthread_create(&tid, NULL, worker_thread, NULL) != 0) {
        t_error("pthread_create failed errno=%d\n", errno);
        return t_status;
    }

    printf("Main thread: sending cancellation request...\n");
    pthread_cancel(tid);

    void* result;
    if (pthread_join(tid, &result) != 0) {
        t_error("pthread_join failed errno=%d\n", errno);
        return t_status;
    }

    if (result == PTHREAD_CANCELED) {
        printf("Worker thread was successfully canceled!\n");
    } else {
        printf("Worker thread was NOT canceled.\n");
        t_error("Expected PTHREAD_CANCELED but got %p\n", result);
    }

    return t_status;
}