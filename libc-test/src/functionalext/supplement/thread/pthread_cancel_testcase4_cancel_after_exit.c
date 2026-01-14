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
// pthread_cancel_testcase4_cancel_after_exit.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* arg) {
    return NULL;
}

int main()
{
    set_pthread_extended_function_policy(1);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    usleep(200000); // ensure thread has exited

    int r = pthread_cancel(tid);
    // POSIX: "If the thread has already terminated, pthread_cancel() has no effect."
    // But return value is unspecified. Most return 0 or ESRCH.
    printf("pthread_cancel after exit returned: %d\n", r);
    // We don't assert failure — spec says "no effect", not error.
    printf("PASS: Behavior after thread exit is acceptable (no crash).\n");
    return 0;
}
#endif