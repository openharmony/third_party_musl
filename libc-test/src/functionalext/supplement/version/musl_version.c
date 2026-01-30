/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#include <string.h>

#include "functionalext.h"
#include "musl_version.h"

/**
 * @tc.name      : get_musl_version_0100
 * @tc.desc      : Test basic functionality of get_musl_version
 * @tc.level     : Level 0
 */
void get_musl_version_0100(void)
{
    const char *version = get_musl_version();
    EXPECT_PTRNE("get_musl_version_0100", version, NULL);
    EXPECT_MT("get_musl_version_0100", strlen(version), 1);
}

/**
 * @tc.name      : get_musl_version_0200
 * @tc.desc      : Verify that the version string conforms to the standard format (X.Y.Z or X.Y)
 * @tc.level     : Level 0
 */
void get_musl_version_0200(void)
{
    const char *version = get_musl_version();
    EXPECT_PTRNE("get_musl_version_0200", version, NULL);
    int major = 0, minor = 0, patch = 0;
    int count = 0;
    count = sscanf(version, "%d.%d.%d", &major, &minor, &patch);
    EXPECT_MT("get_musl_version_0200", count, 0);
    EXPECT_MT("get_musl_version_0200", major, 0);
    EXPECT_MT("get_musl_version_0200", minor, 0);
    if (count == 2) {
        EXPECT_TRUE("get_musl_version_0200", patch == 0);
    } else {
        EXPECT_MT("get_musl_version_0200", patch, 0);
    }
}

/**
 * @tc.name      : get_musl_version_0300
 * @tc.desc      : Verify that multiple calls return the same string address and content
 * @tc.level     : Level 0
 */
void get_musl_version_0300(void)
{
    const char *version1 = get_musl_version();
    EXPECT_PTRNE("get_musl_version_0300", version1, NULL);
    const char *version2 = get_musl_version();
    EXPECT_PTRNE("get_musl_version_0300", version2, NULL);
    EXPECT_TRUE("get_musl_version_0300", strcmp(version1, version2) == 0);
}

#define NUM_THREADS 4
#define ITERATIONS_PER_THREAD 100

struct thread_arg {
    int thread_id;
    int *result;
};

static void *thread_func(void *arg) {
    struct thread_arg *targ = (struct thread_arg *)arg;
    int success_count = 0;

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        const char *version = get_musl_version();
        if (version != NULL && strlen(version) > 0) {
            success_count++;
        }
    }

    *(targ->result) = success_count;
    return NULL;
}

/**
 * @tc.name      : get_musl_version_0400
 * @tc.desc      : Multi-thread concurrent get_musl_version operations test (thread safety)
 * @tc.level     : Level 1
 */
void get_musl_version_0400(void)
{
    pthread_t threads[NUM_THREADS];
    int thread_results[NUM_THREADS] = {0};

    struct thread_arg args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_id = i;
        args[i].result = &thread_results[i];

        if (pthread_create(&threads[i], NULL, thread_func, &args[i]) != 0) {
            t_error("get_musl_version_0400: Failed to create thread %d\n", i);
            thread_results[i] = -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_results[i] != -1) {
            pthread_join(threads[i], NULL);
        }
    }

    int total_success = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_results[i] > 0) {
            total_success += thread_results[i];
        }
    }

    int expected_min_success = NUM_THREADS * ITERATIONS_PER_THREAD;
    EXPECT_GTE("get_musl_version_0400", total_success, expected_min_success);
}

int main(void)
{
    get_musl_version_0100();
    get_musl_version_0200();
    get_musl_version_0300();
    get_musl_version_0400();
    return t_status;
}
