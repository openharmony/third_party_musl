/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <malloc.h>
#include <pthread.h>
#include <sys/wait.h>
#include "gwp_asan_test.h"
#include "test.h"

#define REPS 50
static int a = 0;

void* fork_task(void *arg)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        a++;
    } else { // parent process
        a++;
        return NULL;
    }
    return NULL;
}

void thread_fork_test()
{
    config_gwp_asan_environment(false);
    for (int i = 0; i < REPS; i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, fork_task, NULL);
        pthread_join(tid, NULL);
    }
}

// Test whether it's ok to open gwp_asan in the sub-thread fork scenario.
int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        thread_fork_test();
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            t_error("gwp_asan_thread_fork_test waitpid failed.");
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("gwp_asan_thread_fork_test failed.");
        }

        cancel_gwp_asan_environment(false);
    }
    
    return t_status;
}