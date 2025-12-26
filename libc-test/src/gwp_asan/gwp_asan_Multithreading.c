/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "gwp_asan_test.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define SIZE 100
const int ARRAY_SIZE = 10000;
const int THREAD_NUM = 10;//Number of child threads
int** shared_array = NULL;
int sum = 0;
typedef struct {
    char vars[SIZE];
} thread_data;
__thread thread_data threads;

void use_after_free_handler()
{
    find_and_check_file(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "Use After Free at", false);
    find_and_check_file(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "#2", false);
    find_and_check_file(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "#0.*?gwp_asan", true);
    check_no_consecutive_errors(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, "*** GWP-ASan detected a memory error ***");//Check for duplicates and confusion in the logs.
    cancel_gwp_asan_environment(true);
    _exit(t_status);
}
static void install_sigv_handler()
{
    struct sigaction sigv = {
        .sa_handler = use_after_free_handler,
        .sa_flags = 0,
    };
    sigaction(SIGSEGV, &sigv, NULL);
}

void* read_only_access_array(void* thread_id) {//Child thread accessing memory
    int id = *(int*)thread_id;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        sum += *shared_array[i];
    }
    return NULL;
}

void Multithreading_error() {
    config_gwp_asan_environment(true);
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    install_sigv_handler();

    int** temp_array = (int**)malloc(ARRAY_SIZE * sizeof(int*));
    if (temp_array == NULL) {
        return;
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {//Allocate memory, fill data
        temp_array[i] = (int*)malloc(sizeof(int));
        *temp_array[i] = 1;
    }

    shared_array = temp_array;

    for (int i = ARRAY_SIZE - 1; i >= 0; --i) {//deallocate memory
        free(temp_array[i]);
    }

    pthread_t threads[THREAD_NUM];
    int thread_ids[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; ++i) {//Child thread accessing memory
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, read_only_access_array, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

}

int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        Multithreading_error();
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            t_error("gwp_asan_invalid_free_right_test waitpid failed.");
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("gwp_asan_invalid_free_right_test failed.");
        }
        cancel_gwp_asan_environment(true);
    }
    clear_log(GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG);
    return t_status;
}
