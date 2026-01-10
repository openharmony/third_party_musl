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
#include <unistd.h>
#include <stdbool.h>

// 全局变量，用于记录测试结果
volatile bool test_result = true;
// 互斥锁，用于保护全局变量
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

// B线程函数
void* b_thread_func(void* arg) {
    while (1) {
        pthread_testcancel();
        // 添加微小延迟避免过度占用CPU
        usleep(1000);
    }
    return NULL;
}

// A线程函数
void* a_thread_func(void* arg) {
    pthread_t b_thread;
    int ret;
    
    // 创建B线程
    ret = pthread_create(&b_thread, NULL, b_thread_func, NULL);
    if (ret != 0) {
        test_result = false;
        return NULL;
    }
    
    // 取消B线程
    ret = pthread_cancel(b_thread);
    if (ret != 0) {
        test_result = false;
        // 尝试join以清理资源
        pthread_join(b_thread, NULL);
        return NULL;
    }
    
    // 等待B线程结束
    void* thread_result;
    ret = pthread_join(b_thread, &thread_result);
    if (ret != 0) {
        test_result = false;
        return NULL;
    }
    
    // 检查B线程是否因取消而正常结束
    if (thread_result != PTHREAD_CANCELED) {
        test_result = false;
    }
    
    return NULL;
}

int main() {
    set_pthread_extended_function_policy(1);
    pthread_t a_threads[5];
    int ret;
    test_result = true;
    // 创建5个A线程
    for (int i = 0; i < 5; i++) {
        ret = pthread_create(&a_threads[i], NULL, a_thread_func, NULL);
        if (ret != 0) {
            fprintf(stderr, "Failed to create A thread %d\n", i);
            return 0;
        }
    }
    
    // 等待所有A线程结束
    for (int i = 0; i < 5; i++) {
        pthread_join(a_threads[i], NULL);
    }
    
    // 输出测试结果
    if (test_result) {
        printf("All threads completed successfully.\n");
        return 0;
    } else {
        printf("Test failed.\n");
        return 1;
    }
}
#endif