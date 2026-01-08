/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/*
* @tc.name      : pthread_attr_setaffinity_np_stress_test
* @tc.desc      : During compilation, the following configuration needs to be added to the file of
*                 libc-test/src/functionalext/supplement/BUILD.gn:
*                 if (musl_extended_function) {
*                    defines = [ "MUSL_EXTERNAL_FUNCTION" ]
*                    sources += [ "thread/thread_gtest/pthread_attr_setaffinity_np_test.cpp" ]
*                 }
*/
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <gtest/gtest.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#ifdef MUSL_EXTERNAL_FUNCTION
#ifndef __LITEOS_A__
using namespace testing::ext;

// Global statistics (requires lock protection)
typedef struct {
    long long total_calls;          // Total number of calls
    long long success_calls;        // Number of successful calls
    long long failed_calls;         // Number of failed calls
    pthread_mutex_t mutex;          // Statistics lock
} TestStats;

class ThreadPthreadAffinityTest : public testing::Test {
protected:
    TestStats stats;

    void SetUp() override {
        pthread_mutex_init(&stats.mutex, nullptr);
        stats.total_calls = 0;
        stats.success_calls = 0;
        stats.failed_calls = 0;
    }

    void TearDown() override {
        pthread_mutex_destroy(&stats.mutex);
    }

    void transform(const int index);
};


// Test configuration (can be adjusted as needed)
#define CONCURRENT_THREADS  32      // Number of concurrent test threads
#define CALLS_PER_THREAD    1000    // Number of interface calls per test thread
#define TARGET_CPU_CORE     1       // Target CPU core for testing (recommended to select existing core)

static int get_cpu_count() {
    return get_nprocs_conf();
}

static void init_stats(TestStats* stats) {
    pthread_mutex_init(&stats->mutex, nullptr);
    stats->total_calls = 0;
    stats->success_calls = 0;
    stats->failed_calls = 0;
}

static void update_stats(TestStats* stats, int is_success) {
    pthread_mutex_lock(&stats->mutex);
    stats->total_calls++;
    if (is_success) {
        stats->success_calls++;
    } else {
        stats->failed_calls++;
    }
    pthread_mutex_unlock(&stats->mutex);
}

void *threadfunc(void *arg)
{
    // Get temporary thread attributes (verify pthread_getattr_np)
    pthread_attr_t get_attr;
    int ret = 0;
    ret = pthread_attr_init(&get_attr);
    if (ret == 0) {
        ret = pthread_getattr_np(pthread_self(), &get_attr);
        if (ret != 0) {
            fprintf(stderr, "pthread_getattr_np failed: %s (errno=%d)\n", std::strerror(ret), ret);
        }
        pthread_attr_destroy(&get_attr);
}
    return nullptr;
}

static int verify_affinity_settings(pthread_attr_t* attr, size_t cpusetsize) {
    cpu_set_t get_cpuset;
    CPU_ZERO(&get_cpuset);
    int ret = pthread_attr_getaffinity_np(attr, cpusetsize, &get_cpuset);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_getaffinity_np failed: %s (errno=%d)\n", std::strerror(ret), ret);
        return ret;
    }

    if (!CPU_ISSET(TARGET_CPU_CORE, &get_cpuset)) {
        fprintf(stderr, "Affinity verification failed: set CPU %d, but could not get it\n", TARGET_CPU_CORE);
        return -1;
    }
    return 0;
}

static int create_and_join_test_thread(pthread_attr_t* attr) {
    pthread_t temp_thread;
    int ret = pthread_create(&temp_thread, attr, threadfunc, nullptr);
    if (ret != 0) {
        fprintf(stderr, "pthread_create failed: %s (errno=%d)\n", std::strerror(ret), ret);
        return ret;
    }

    pthread_join(temp_thread, nullptr);
    return 0;
}

static int execute_single_affinity_test(TestStats* stats) {
    int ret = 0;
    pthread_attr_t attr;
    cpu_set_t cpuset;
    size_t cpusetsize = sizeof(cpu_set_t);

    ret = pthread_attr_init(&attr);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_init failed: %s (errno=%d)\n", std::strerror(ret), ret);
        return ret;
    }

    CPU_ZERO(&cpuset);
    CPU_SET(TARGET_CPU_CORE, &cpuset);
    ret = pthread_attr_setaffinity_np(&attr, cpusetsize, &cpuset);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_setaffinity_np failed: %s (errno=%d)\n", std::strerror(ret), ret);
        pthread_attr_destroy(&attr);
        return ret;
    }

    ret = verify_affinity_settings(&attr, cpusetsize);
    if (ret != 0) {
        pthread_attr_destroy(&attr);
        return ret;
    }

    ret = create_and_join_test_thread(&attr);
    if (ret != 0) {
        pthread_attr_destroy(&attr);
        return ret;
    }

    ret = pthread_attr_destroy(&attr);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_destroy failed: %s (errno=%d)\n", std::strerror(ret), ret);
        return ret;
    }

    return 0;
}

// Test thread function: core logic - loop calling pthread affinity interfaces
static void* affinity_test_thread(void* arg) {
    TestStats* stats = static_cast<TestStats*>(arg);
    int cpu_count = get_cpu_count();
    if (cpu_count <= TARGET_CPU_CORE) {
        fprintf(stderr, "Target CPU core %d exceeds system range (max %d)\n", TARGET_CPU_CORE, cpu_count-1);
        return nullptr;
    }

    for (int i = 0; i < CALLS_PER_THREAD; i++) {
        int ret = execute_single_affinity_test(stats);
        update_stats(stats, ret == 0);
    }

    return nullptr;
}

void ThreadPthreadAffinityTest::transform(const int index)
{
    int cpu_count = get_cpu_count();
    init_stats(&stats);
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create concurrent test threads
    pthread_t test_threads[CONCURRENT_THREADS];
    for (int i = 0; i < CONCURRENT_THREADS; i++) {
        int ret = pthread_create(&test_threads[i], nullptr, affinity_test_thread, &stats);
        if (ret != 0) {
            fprintf(stderr, "Loop %d, failed to create test thread %d: %s (errno=%d)\n", index, i, std::strerror(ret),
                    ret);
            if (ret != 0) {
                FAIL() << "Failed to create test thread";
            }
        }
    }

    // Wait for all test threads to exit
    for (int i = 0; i < CONCURRENT_THREADS; i++) {
        pthread_join(test_threads[i], nullptr);
    }

    // Record end time and calculate elapsed time
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;

    pthread_mutex_destroy(&stats.mutex);
    EXPECT_GT(stats.total_calls, 0) << "Total calls should be greater than 0";
    EXPECT_GT(stats.success_calls, 0) << "Successful calls should be greater than 0";
    double success_rate = static_cast<double>(stats.success_calls) / stats.total_calls * 100;
    EXPECT_GE(success_rate, 90.0) << "Success rate should reach 90% or higher";
}

/**
 * @tc.name: pthread_attr_setaffinity_np_001
 * @tc.desc: After adding thread CPU affinity code, perform thread safety stress testing.
 * @tc.type: FUNC
 * */
HWTEST_F(ThreadPthreadAffinityTest, pthread_attr_setaffinity_np_001, TestSize.Level1)
{
    int loopNum = 100;
    for (int i = 1; i <= loopNum; i++) {
        transform(i);
    }
}
#endif
#endif