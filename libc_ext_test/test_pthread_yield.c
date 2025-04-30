/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdatomic.h>
#include <unistd.h>
extern int pthread_yield(void);
// Test 1: Basic functionality (returns 0)
static void test_return_value() {
    int ret = pthread_yield();
    assert(ret == 0);
    printf("Test 1: Basic return value passed\n");
}

// Test 2: Thread coordination with yield
static atomic_int flag = 0;

static void* thread_worker(void *arg) {
    // Simulate work
    for (volatile int i = 0; i < 100000; i++);
    flag = 1;
    return NULL;
}

static void* thread_waiter(void *arg) {
    while (flag == 0) {
        pthread_yield(); // Yield until flag is set
    }
    return NULL;
}

static void test_thread_coordination() {
    pthread_t worker, waiter_tid;
    pthread_create(&worker, NULL, thread_worker, NULL);
    pthread_create(&waiter_tid, NULL, thread_waiter, NULL);

    pthread_join(worker, NULL);
    pthread_join(waiter_tid, NULL);

    assert(flag == 1);
    printf("Test 2: Thread coordination passed\n");
}

// Test 3: Ensure both threads run (best-effort check)
static atomic_int counter1 = 0;
static atomic_int counter2 = 0;

static void* thread_counter1(void *arg) {
    for (int i = 0; i < 1000; i++) {
        counter1++;
        pthread_yield();
    }
    return NULL;
}

static void* thread_counter2(void *arg) {
    for (int i = 0; i < 1000; i++) {
        counter2++;
        pthread_yield();
    }
    return NULL;
}

static void test_concurrent_counters() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_counter1, NULL);
    pthread_create(&t2, NULL, thread_counter2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(counter1 == 1000 && counter2 == 1000);
    printf("Test 3: Concurrent counters passed\n");
}

static atomic_int thread1_count = 0;
static atomic_int thread2_count = 0;

static void* thread1(void *arg) {
    for (int i = 0; i < 1000; i++) {
        thread1_count++;
        pthread_yield();
    }
    return NULL;
}

static void* thread2(void *arg) {
    for (int i = 0; i < 1000; i++) {
        thread2_count++;
        pthread_yield();
    }
    return NULL;
}

static void test_tight_loop_progress() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(thread1_count == 1000);
    assert(thread2_count == 1000);
    printf("Test 4: Tight loop progress passed\n");
}

static void test_main_thread_yield() {
    int ret = pthread_yield(); // Main thread yields
    assert(ret == 0);
    printf("Test 5: Main thread yield passed\n");
}

#define NUM_THREADS 5
static atomic_int shared_counter = 0;

static void* shared_counter_thread(void *arg) {
    for (int i = 0; i < 100; i++) {
        shared_counter++;
        pthread_yield();
    }
    return NULL;
}

static void test_multiple_threads_shared_resource() {
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, shared_counter_thread, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    assert(shared_counter == NUM_THREADS * 100);
    printf("Test 6: Multiple threads with shared resource passed\n");
}

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static atomic_int mutex_counter = 0;

static void* mutex_thread(void *arg) {
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&mutex);
        mutex_counter++;
        pthread_mutex_unlock(&mutex);
        pthread_yield();
    }
    return NULL;
}

static void test_yield_with_mutex() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, mutex_thread, NULL);
    pthread_create(&t2, NULL, mutex_thread, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(mutex_counter == 200);
    printf("Test 7: Yield with mutex passed\n");
}

static atomic_int low_priority_done = 0;

static void* high_priority_thread(void *arg) {
    for (int i = 0; i < 1000; i++) {
        pthread_yield(); // Yield aggressively
    }
    return NULL;
}

static void* low_priority_thread(void *arg) {
    low_priority_done = 1;
    return NULL;
}

static void test_starvation_avoidance() {
    pthread_t t_high, t_low;
    pthread_create(&t_high, NULL, high_priority_thread, NULL);
    pthread_create(&t_low, NULL, low_priority_thread, NULL);

    pthread_join(t_high, NULL);
    pthread_join(t_low, NULL);

    assert(low_priority_done == 1);
    printf("Test 8: Starvation avoidance passed\n");
}

static void* exiting_thread(void *arg) {
    pthread_yield();
    return NULL;
}

static void test_yield_in_exiting_thread() {
    pthread_t t;
    pthread_create(&t, NULL, exiting_thread, NULL);
    pthread_join(t, NULL);
    printf("Test 9: Yield in exiting thread passed\n");
}

#define STRESS_THREADS 50
static atomic_int stress_counter = 0;

static void* stress_thread(void *arg) {
    for (int i = 0; i < 20; i++) {
        stress_counter++;
        pthread_yield();
    }
    return NULL;
}

static void test_stress_many_threads() {
    pthread_t threads[STRESS_THREADS];
    for (int i = 0; i < STRESS_THREADS; i++) {
        pthread_create(&threads[i], NULL, stress_thread, NULL);
    }
    for (int i = 0; i < STRESS_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    assert(stress_counter == STRESS_THREADS * 20);
    printf("Test 10: Stress test with many threads passed\n");
}

int main() {
    test_return_value();
    test_thread_coordination();
    test_concurrent_counters();
    test_tight_loop_progress();
    test_main_thread_yield();
    test_multiple_threads_shared_resource();
    test_yield_with_mutex();
    test_starvation_avoidance();
    test_yield_in_exiting_thread();
    test_stress_many_threads();
    printf("All tests passed!\n");
    return 0;
}