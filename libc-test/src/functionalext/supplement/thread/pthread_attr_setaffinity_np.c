/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

static int get_cpu_count() {
    int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    EXPECT_GT("get_cpu_count", nprocs, 0);
    return nprocs;
}

void *threadfunc(void *arg)
{
    return arg;
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0100
 * @tc.desc      : Basic cpu affinity settings and retrieval
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0100(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset, cpuset_get;
    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0100", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", ret, 0);
    CPU_ZERO(&cpuset_get);
    ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0100", CPU_EQUAL(&cpuset, &cpuset_get));
    EXPECT_TRUE("pthread_attr_setaffinity_np_0100", CPU_ISSET(0, &cpuset_get));

    for (int i = 1; i < ncpus; i++) {
        EXPECT_TRUE("pthread_attr_setaffinity_np_0100", !CPU_ISSET(i, &cpuset_get));
    }
    pthread_attr_destroy(&attr);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0200
 * @tc.desc      : Multi CPU affinity
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0200(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset, cpuset_get;
    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0200", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0200", ret, 0);

    int use_cpus = (ncpus < 4) ? ncpus : 4;
    CPU_ZERO(&cpuset);
    for (int i = 0; i < use_cpus; i++) {
        CPU_SET(i, &cpuset);
    }

    ret  = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0200", ret, 0);
    CPU_ZERO(&cpuset_get);
    ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0200", ret, 0);

    for (int i = 0; i < use_cpus; i++) {
        EXPECT_TRUE("pthread_attr_setaffinity_np_0200", CPU_ISSET(i, &cpuset_get));
    }
    for (int i = use_cpus; i < ncpus; i++) {
        EXPECT_TRUE("pthread_attr_setaffinity_np_0200", !CPU_ISSET(i, &cpuset_get));
    }
    pthread_attr_destroy(&attr);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0300
 * @tc.desc      : Error handling - Invalid CPU (out of range)
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0300(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset, cpuset_get;
    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0300", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0300", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(ncpus + 10, &cpuset);  // invalid CPU ID

    ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0300", ret, 0);

    // Empty affinity set
    CPU_ZERO(&cpuset_get);
    ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0300", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0300", CPU_ISSET(ncpus + 10, &cpuset_get));

    pthread_attr_destroy(&attr);

    // Test signal 11 handling when using destroyed attribute
    pid_t pid = fork();
    if (pid < 0) {
        t_error("pthread_attr_setaffinity_np_0300: fork failed\n");
    } else if (pid == 0) {
        // Child process: intentionally trigger signal 11
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        // If we reach here, signal 11 was not triggered (unexpected)
        exit(1);
    } else {
        // Parent process: wait and check for signal 11
        int status;
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            if (sig == SIGSEGV) {
                // Expected behavior: signal 11 was caught
                EXPECT_TRUE("pthread_attr_setaffinity_np_0300", 1);
            } else {
                t_error("pthread_attr_setaffinity_np_0300: unexpected signal %d\n", sig);
            }
        } else if (WIFEXITED(status)) {
            t_error("pthread_attr_setaffinity_np_0300: child exited normally (signal 11 not triggered)\n");
        }
    }
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0400
 * @tc.desc      : Error handling - null pointer/zero size
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0400(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset;
    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0200", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    ret = pthread_attr_setaffinity_np(NULL, sizeof(cpu_set_t), &cpuset);
    EXPECT_NE("pthread_attr_setaffinity_np_0400", ret, 0);
    EXPECT_EQ("pthread_attr_setaffinity_np_0400", ret, EINVAL);
    EXPECT_NE("pthread_attr_setaffinity_np_0400", pthread_attr_getaffinity_np(NULL, sizeof(cpu_set_t), &cpuset), 0);
    EXPECT_NE("pthread_attr_setaffinity_np_0400", pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), NULL), 0);
    EXPECT_NE("pthread_attr_setaffinity_np_0400", pthread_attr_getaffinity_np(&attr, 0, &cpuset), 0);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0500
 * @tc.desc      : Test the setting of CPU affinity attributes in thread attributes and verify that the created thread
 *                 has the corresponding CPU affinity
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0500(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset, cpuset_get;
    pthread_t ph;

    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0500", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0500", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(3, &cpuset);

    ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0500", ret, 0);

    ret = pthread_create(&ph, &attr, threadfunc, NULL);
    EXPECT_EQ("pthread_attr_setaffinity_np_0500", ret, 0);

    CPU_ZERO(&cpuset_get);
    ret = pthread_getaffinity_np(ph, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0500", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0500", CPU_EQUAL(&cpuset, &cpuset_get));
    EXPECT_TRUE("pthread_attr_setaffinity_np_0500", CPU_ISSET(3, &cpuset_get));
    pthread_attr_destroy(&attr);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0600
 * @tc.desc      : Test the setting of CPU affinity attributes with related functions in thread attributes
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0600(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr, attr1;
    cpu_set_t cpuset, cpuset_get;
    pthread_t ph;

    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0600", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);
    ret = pthread_attr_init(&attr1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);

    ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);

    ret = pthread_create(&ph, &attr, threadfunc, NULL);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);

    ret = pthread_getattr_np(ph, &attr1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);
    CPU_ZERO(&cpuset_get);

    pthread_attr_getaffinity_np(&attr1, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0600", CPU_EQUAL(&cpuset, &cpuset_get));
    EXPECT_TRUE("pthread_attr_setaffinity_np_0600", CPU_ISSET(2, &cpuset_get));

    CPU_ZERO(&cpuset_get);
    pthread_attr_getaffinity_np(&attr1, sizeof(cpu_set_t) - 1, &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0600", ret, 0);
    pthread_attr_destroy(&attr);
    pthread_attr_destroy(&attr1);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0700
 * @tc.desc      : Test pthread_attr_getaffinity_np function when using pthread_setaffinity_np function to set thread
 *                 CPU affinity
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0700(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr, attr1;
    cpu_set_t cpuset, cpuset_get;
    pthread_t ph;

    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0700", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);
    ret = pthread_attr_init(&attr1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);
    ret = pthread_create(&ph, NULL, threadfunc, NULL);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);

    ret = pthread_setaffinity_np(ph, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);

    ret = pthread_getattr_np(ph, &attr1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);

    CPU_ZERO(&cpuset_get);
    pthread_attr_getaffinity_np(&attr1, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0700", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0700", CPU_EQUAL(&cpuset, &cpuset_get));
    EXPECT_TRUE("pthread_attr_setaffinity_np_0700", CPU_ISSET(2, &cpuset_get));
    pthread_attr_destroy(&attr);
    pthread_attr_destroy(&attr1);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0800
 * @tc.desc      : Test related interface calls when musl_pc_module = 0
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0800(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(0);
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 0);
    pthread_attr_t attr;
    cpu_set_t cpuset;
    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_0800", ncpus, 0);

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setaffinity_np_0800", ret, 0);

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_NE("pthread_attr_setaffinity_np_0800", ret, 0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_0900
 * @tc.desc      : Multi threaded independent CPU affinity binding test
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_0900(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr1, attr2;
    pthread_t thread1, thread2;
    cpu_set_t cpuset1, cpuset2;
    int ret;
    int cpu_count = get_cpu_count();
    int cpu1 = 0;
    int cpu2 = (cpu_count > 1) ? 1 : 0;

    // Initialize thread attribute 1
    ret = pthread_attr_init(&attr1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);

    CPU_ZERO(&cpuset1);
    CPU_SET(cpu1, &cpuset1);
    ret = pthread_attr_setaffinity_np(&attr1, sizeof(cpu_set_t), &cpuset1);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    // Initialize thread attribute 2
    ret = pthread_attr_init(&attr2);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    CPU_ZERO(&cpuset2);
    CPU_SET(cpu2, &cpuset2);
    ret = pthread_attr_setaffinity_np(&attr2, sizeof(cpu_set_t), &cpuset2);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    // Create two threads with different CPU affinity attributes
    ret = pthread_create(&thread1, &attr1, threadfunc, (void *)(long)3);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    ret = pthread_create(&thread2, &attr2, threadfunc, (void *)(long)4);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    // verify that the two threads have the correct CPU affinity
    cpu_set_t get_cpuset;
    CPU_ZERO(&get_cpuset);
    ret = pthread_getaffinity_np(thread1, sizeof(cpu_set_t), &get_cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0900", CPU_ISSET(cpu1, &get_cpuset));
    CPU_ZERO(&get_cpuset);
    ret = pthread_getaffinity_np(thread2, sizeof(cpu_set_t), &get_cpuset);
    EXPECT_EQ("pthread_attr_setaffinity_np_0900", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0900", CPU_ISSET(cpu2, &get_cpuset));

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

/**
 * @tc.name      : pthread_attr_getaffinity_np_0100
 * @tc.desc      : Test pthread_attr_getaffinity_np function for branch coverage
 *                 CPU affinity
 * @tc.level     : Level 0
 */
void pthread_attr_getaffinity_np_0100(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_getaffinity_np_0100", get_pthread_extended_function_policy(), 1);

    pthread_attr_t attr;
    cpu_set_t cpuset;
    int ret;

    ret = pthread_attr_getaffinity_np(NULL, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_getaffinity_np_0100", ret, EINVAL);
    ret = pthread_attr_getaffinity_np(NULL, 0, &cpuset);
    EXPECT_EQ("pthread_attr_getaffinity_np_0100", ret, EINVAL);
    ret = pthread_attr_getaffinity_np(NULL, sizeof(cpu_set_t), NULL);
    EXPECT_EQ("pthread_attr_getaffinity_np_0100", ret, EINVAL);

    // attr->_a_cpusetaddr = NULL case
    pthread_attr_init(&attr);
    ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    EXPECT_EQ("pthread_attr_getaffinity_np_0100", ret, 0);
    int ncpus = get_cpu_count();
    for (int i = 0; i < ncpus; i++) {
        EXPECT_TRUE("pthread_attr_getaffinity_np_0100", CPU_ISSET(i, &cpuset));
    }
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

#define NUM_THREADS 4
#define ITERATIONS 100

struct thread_data {
    int thread_id;
    int cpu_id;
};

void *concurrent_affinity_test(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;
    pthread_attr_t attr;
    cpu_set_t cpuset, cpuset_get;
    int ret;

    for (int i = 0; i < ITERATIONS; i++) {
        ret = pthread_attr_init(&attr);
        if (ret != 0) {
            continue;
        }

        CPU_ZERO(&cpuset);
        CPU_SET(data->cpu_id, &cpuset);

        ret = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
        if (ret != 0) {
            pthread_attr_destroy(&attr);
            continue;
        }

        CPU_ZERO(&cpuset_get);
        ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset_get);
        if (ret != 0) {
            pthread_attr_destroy(&attr);
            continue;
        }

        if (!CPU_EQUAL(&cpuset, &cpuset_get)) {
            pthread_attr_destroy(&attr);
            continue;
        }

        pthread_attr_destroy(&attr);
    }

    return NULL;
}

/**
 * @tc.name      : pthread_attr_setaffinity_np_1000
 * @tc.desc      : Multi-thread concurrent stability test for pthread_attr_setaffinity_np and
 *                 pthread_attr_getaffinity_np
 * @tc.level     : Level 0
 */
void pthread_attr_setaffinity_np_1000(void)
{
    #ifdef MUSL_EXTERNAL_FUNCTION
    #ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    EXPECT_EQ("pthread_attr_setaffinity_np_1000", get_pthread_extended_function_policy(), 1);

    int ncpus = get_cpu_count();
    EXPECT_GT("pthread_attr_setaffinity_np_1000", ncpus, 0);

    pthread_t threads[NUM_THREADS];
    struct thread_data data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].thread_id = i;
        data[i].cpu_id = i % ncpus;

        int ret = pthread_create(&threads[i], NULL, concurrent_affinity_test, &data[i]);
        EXPECT_EQ("pthread_attr_setaffinity_np_1000", ret, 0);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int ret = pthread_join(threads[i], NULL);
        EXPECT_EQ("pthread_attr_setaffinity_np_1000", ret, 0);
    }
    set_pthread_extended_function_policy(0);
    #endif
    #endif
}

int main(int argc, char *argv[])
{
    pthread_attr_setaffinity_np_0100();
    pthread_attr_setaffinity_np_0200();
    pthread_attr_setaffinity_np_0300();
    pthread_attr_setaffinity_np_0400();
    pthread_attr_setaffinity_np_0500();
    pthread_attr_setaffinity_np_0600();
    pthread_attr_setaffinity_np_0700();
    pthread_attr_setaffinity_np_0800();
    pthread_attr_setaffinity_np_0900();
    pthread_attr_setaffinity_np_1000();
    pthread_attr_getaffinity_np_0100();
    return t_status;
}
