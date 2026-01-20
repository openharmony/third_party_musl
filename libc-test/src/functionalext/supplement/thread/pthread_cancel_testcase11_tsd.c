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
#ifdef MUSL_EXTERNAL_FUNCTION
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_key_t tsd_key;
static bool flag = false;

void tsd_destructor(void *value)
{
    printf("TSD destructor called with value: %p\n", value);
    (void)free(value);
    flag = true;
}

void *worker(void *arg)
{
    int *tsd_value = malloc(sizeof(int));
    if (!tsd_value) {
        printf("malloc failed errno=%d\n", errno);
        return NULL;
    }
    *tsd_value = 42;
    (void)pthread_setspecific(tsd_key, tsd_value);
    printf("Thread TSD value set to: %d\n", *tsd_value);
    // Simulate some work
    for (int i = 0; i < 2000; i++) {
        pthread_testcancel();
        usleep(1000);
    }
    printf("Thread finished work\n");
    return NULL;
}

int main()
{
    set_pthread_extended_function_policy(1);
    pthread_t thread;
    int ret;

    // Create TSD key with destructor
    ret = pthread_key_create(&tsd_key, tsd_destructor);
    if (ret != 0) {
        perror("pthread_key_create failed");
        return 0;
    }

    // Create worker thread
    ret = pthread_create(&thread, NULL, worker, NULL);
    if (ret != 0) {
        perror("pthread_create failed");
        return 0;
    }

    // Cancel the worker thread
    printf("Main thread cancelling worker thread\n");
    ret = pthread_cancel(thread);
    if (ret != 0) {
        perror("pthread_cancel failed");
        return 1;
    }

    // Wait for the worker thread to finish
    ret = pthread_join(thread, NULL);
    if (ret != 0) {
        perror("pthread_join failed");
        return 0;
    }

    // Check if TSD destructor was called
    if (flag) {
        printf("PASS: TSD destructor was called upon thread cancellation.\n");
        return 0;
    } else {
        printf("FAIL: TSD destructor was not called.\n");
        return 1;
    }
}
#endif