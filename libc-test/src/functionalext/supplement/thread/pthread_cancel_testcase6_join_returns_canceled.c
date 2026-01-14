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
// pthread_cancel_testcase6_join_returns_canceled.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* thread_func(void* arg) {
    while (1) {
        usleep(10000);
        pthread_testcancel();
    }
    return NULL;
}

int main()
{
    set_pthread_extended_function_policy(1);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    usleep(50000);
    pthread_cancel(tid);

    void* result;
    pthread_join(tid, &result);
    if (result == PTHREAD_CANCELED) {
        printf("PASS: pthread_join returns PTHREAD_CANCELED.\n");
        return 0;
    } else {
        printf("FAIL: pthread_join did not return PTHREAD_CANCELED (got %p).\n", result);
        return 1;
    }
}
#endif