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
// pthread_cancel_testcase1_basic_cancel.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

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
    int res = pthread_create(&tid, NULL, thread_func, NULL);
    if (res != 0) {
        printf("FAIL: pthread_create returned %d errno=%d\n", res, errno);
        return 1;
    }
    int r = pthread_cancel(tid);
    if (r != 0) {
        printf("FAIL: pthread_cancel returned %d\n", r);
        return 1;
    }

    void* result;
    pthread_join(tid, &result);
    if (result == PTHREAD_CANCELED) {
        printf("PASS: Basic deferred cancellation works.\n");
        return 0;
    } else {
        printf("FAIL: Thread not canceled.\n");
        return 1;
    }
}
#endif