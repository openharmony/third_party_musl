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
#include <stdlib.h>
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
    printf("pthread_attr_setaffinity_np_0100 start\n");
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
    (void)ret;

    CPU_ZERO(&cpuset_get);
    ret = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpuset_get);
    EXPECT_EQ("pthread_attr_setaffinity_np_0300", ret, 0);
    EXPECT_TRUE("pthread_attr_setaffinity_np_0300", CPU_ISSET(ncpus + 10, &cpuset_get));

    pthread_attr_destroy(&attr);
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
    pthread_attr_destroy(&attr);
    pthread_attr_destroy(&attr1);
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
    cpu_set_t cpuset, cpuset_get;
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
    EXPECT_EQ("pthread_attr_setaffinity_np_0100", get_pthread_extended_function_policy(), 1);

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
        EXPECT_FALSE("pthread_attr_getaffinity_np_0100", CPU_ISSET(i, &cpuset));
    }
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
    pthread_attr_getaffinity_np_0100();
    return t_status;
}
