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
// pthread_cancel_testcase3_disabled_cancel.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <atomic.h>
#include <unistd.h>

static volatile int thread_ready = 0; // 标志：子线程已完成取消状态设置

void* thread_func(void* arg) {
    // 1. 禁用取消
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    // 2. 通知主线程：我已准备好，可安全取消（但会被忽略）
    a_cas(&thread_ready, 0, 1);

    // 3. 执行一段可取消的操作（如 sleep）
    for (int i = 0; i < 2000; i++) {
        usleep(1000); // 1ms
    }

    return (void*)0x1234; // 正常退出值
}

int main()
{
    set_pthread_extended_function_policy(1);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    // 主线程等待：直到子线程明确表示“已禁用取消”
    while (a_cas(&thread_ready, 1, 0) != 1) {
        usleep(1000); // 1ms
    }

    // 此刻可确定：子线程已禁用取消，且即将进入 sleep
    int r = pthread_cancel(tid);
    if (r != 0) {
        printf("FAIL: pthread_cancel returned error %d\n", r);
        return 1;
    }

    void* result;
    pthread_join(tid, &result);

    if (result == (void*)0x1234) {
        printf("PASS: Cancellation correctly ignored when disabled.\n");
        return 0;
    } else if (result == PTHREAD_CANCELED) {
        printf("FAIL: Thread was canceled despite cancellation being disabled!\n");
        return 1;
    } else {
        printf("FAIL: Unexpected thread return value: %p\n", result);
        return 1;
    }
}
#endif