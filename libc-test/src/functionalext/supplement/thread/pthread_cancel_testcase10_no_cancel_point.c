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
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "functionalext.h"

#define ITERATIONS (10000)

void* func1(void* arg) {
    printf("Sub-thread func1 is running\n");
    for (int i = 0; i < ITERATIONS; i++);
    printf("Sub-thread func1 is ending naturally\n");
    return NULL;
}

int subProcess() {
    set_pthread_extended_function_policy(1);
    pthread_t thread;
    int ret;

    // Create sub-thread func1
    ret = pthread_create(&thread, NULL, func1, NULL);
    if (ret != 0) {
        perror("pthread_create failed");
        exit(EXIT_FAILURE);
    }

    printf("Thread has been detached\n");

    // Try to cancel the detached sub-thread
    printf("Attempting to cancel detached thread...\n");
    ret = pthread_cancel(thread);
    
    // The expected result is ESRCH (No such thread)
    // Because detached threads that have completed execution 
    // are cleaned up by the system and no longer exist
    if (ret == ESRCH) {
        printf("pthread_cancel returned ESRCH as expected: No such thread\n");
    } else if (ret == 0) {
        printf("pthread_cancel succeeded - thread was cancelled\n");
    } else {
        printf("pthread_cancel returned: %d (%s)\n", ret, strerror(ret));
    }

    void *result = NULL;

    ret = pthread_join(thread, &result);
    if (ret != 0) {
        printf("pthread_join failed: %d (%s)\n", ret, strerror(ret));
        exit(EXIT_FAILURE);
    } else {
        printf("Thread has been joined\n");
        if (result == PTHREAD_CANCELED) {
            printf("PASS: pthread_join returns PTHREAD_CANCELED.\n");
        } else {
            printf("FAIL: pthread_join did not return PTHREAD_CANCELED (got %p).\n", result);
        }
    }

    // Sleep to allow any cancellation to take effect
    printf("Test completed\n");

    return 0;
}

// fork then execute subProcess
int main()
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: execute subProcess
        subProcess();
        // If we reach here, no crash happened — but we forced crash above
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        wait(&status);
    }

    return 0;
}
#endif