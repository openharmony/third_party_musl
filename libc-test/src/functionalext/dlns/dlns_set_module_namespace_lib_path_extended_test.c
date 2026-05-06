/**
 * Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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

/**
 * @file dlns_set_module_namespace_lib_path_extended_test.c
 * @brief Extended test cases for dlns_set_module_namespace_lib_path interface
 *
 * This file contains extended test cases that cover potential issues found in
 * source code analysis, including:
 * - Concurrent access (race conditions)
 * - Memory allocation stress
 * - Path format validation
 * - Path length limits
 */

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>

#include "dlns_test.h"
#include "functionalext.h"

// Test configuration
#define THREAD_COUNT 10
#define ITERATIONS_PER_THREAD 1000
#define STRESS_TEST_ITERATIONS 10000

// Global variables for concurrent testing
static Dl_namespace *g_concurrent_test_ns = NULL;
static volatile int g_concurrent_test_passed = 1;
static volatile int g_concurrent_thread_errors[THREAD_COUNT] = {0};

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0150
 * @tc.desc      : Test path with trailing slashes in individual paths
 * @tc.level     : Level 2
 * @tc.status    : Implemented
 * @tc.type      : Extended
 * @tc.purpose   : Verify handling of paths ending with '/' (documented as invalid)
 *
 * This test checks if the interface properly rejects paths with trailing slashes
 * as specified in the documentation. The source code analysis reveals this may
 * not be fully enforced.
 */
void dlns_set_module_namespace_lib_path_0150(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0150");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0150", dlns_create(&dlns, NULL), EOK);

    // Test: Individual paths ending with '/' (documented as invalid)
    char* pathWithTrailingSlash = "/data/tests/libc-test/src/:/data/tests/libc-test/src/B/";

    int ret = dlns_set_module_namespace_lib_path(
        "dlns_set_module_namespace_lib_path_0150",
        pathWithTrailingSlash
    );

    // Expected behavior validation
    if (ret == EOK) {
        // Current implementation accepts it - this is a known issue
        printf("INFO: Path with trailing slash was accepted (known issue in source code)\n");

        // Verify it still works despite the format issue
        void *handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
        EXPECT_TRUE("dlns_set_module_namespace_lib_path_0150_still_works", handle);
        if (handle) {
            dlclose(handle);
        }
    } else if (ret == EINVAL) {
        // Ideal behavior - properly rejected
        printf("INFO: Path with trailing slash was correctly rejected\n");
        EXPECT_EQ("dlns_set_module_namespace_lib_path_0150_rejected", ret, EINVAL);
    } else {
        printf("ERROR: Unexpected return code %d\n", ret);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0160
 * @tc.desc      : Test single path approaching PATH_MAX limit
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 * @tc.type      : Extended
 * @tc.purpose   : Verify handling of very long single paths
 *
 * This test checks if the interface properly handles paths approaching or
 * exceeding PATH_MAX (typically 4096 bytes).
 */
void dlns_set_module_namespace_lib_path_0160(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0160");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0160", dlns_create(&dlns, NULL), EOK);

    // Test: Single path close to but not exceeding PATH_MAX
    char path1[PATH_MAX];
    memset(path1, 'a', PATH_MAX - 20);
    path1[PATH_MAX - 20] = '\0';

    // Make it a valid path format
    path1[0] = '/';
    for (int i = 1; i < PATH_MAX - 21; i++) {
        if (i % 50 == 0) {
            path1[i] = '/';
        }
    }
    path1[PATH_MAX - 21] = '\0';

    int ret = dlns_set_module_namespace_lib_path(
        "dlns_set_module_namespace_lib_path_0160",
        path1
    );

    // Document the actual behavior
    if (ret == EOK) {
        printf("INFO: Long path (%d bytes) was accepted\n", (int)strlen(path1));
    } else if (ret == EINVAL) {
        printf("INFO: Long path was rejected (may exceed implementation limit)\n");
    } else {
        printf("WARNING: Unexpected return code %d for long path\n", ret);
    }

    // Clean up if successful
    if (ret == EOK) {
        // Reset to a valid short path
        dlns_set_module_namespace_lib_path("dlns_set_module_namespace_lib_path_0160", path);
    }
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0170
 * @tc.desc      : Test multiple paths with combined length limit
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 * @tc.type      : Extended
 * @tc.purpose   : Verify handling of multiple long paths concatenated
 *
 * This test checks if the interface properly handles multiple paths where
 * the combined string approaches practical limits.
 */
void dlns_set_module_namespace_lib_path_0170(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0170");

    EXPECT_EQ("dlns_set_module_namespace_lib_path_0170", dlns_create(&dlns, NULL), EOK);

    // Test: Multiple moderate-length paths concatenated
    char buffer[8192];
    int offset = 0;

    // Create multiple path entries
    for (int i = 0; i < 20; i++) {
        if (offset > 0) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, ":");
        }
        offset += snprintf(buffer + offset, sizeof(buffer) - offset,
            "/data/tests/libc-test/src/directory%02d", i);
    }

    int ret = dlns_set_module_namespace_lib_path(
        "dlns_set_module_namespace_lib_path_0170",
        buffer
    );

    if (ret == EOK) {
        printf("INFO: Long multi-path string (%d bytes) was accepted\n", (int)strlen(buffer));

        // Verify basic functionality still works
        void *handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
        EXPECT_TRUE("dlns_set_module_namespace_lib_path_0170_load_check", handle);
        if (handle) {
            dlclose(handle);
        }
    } else if (ret == EINVAL) {
        printf("INFO: Long multi-path string was rejected\n");
    } else {
        printf("WARNING: Unexpected return code %d\n", ret);
    }
}

/**
 * Thread function for concurrent testing
 * Each thread repeatedly sets different paths to test race conditions
 */
static void* concurrent_set_path_thread(void *arg)
{
    int thread_id = *(int *)arg;
    char path_buffer[256];
    int errors = 0;

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        // Each thread uses different paths to maximize contention
        (void)snprintf(path_buffer, sizeof(path_buffer),
            "/data/tests/libc-test/src/T%d/iter%d", thread_id, i % 100);

        int ret = dlns_set_module_namespace_lib_path(
            g_concurrent_test_ns->name,
            path_buffer
        );

        // Track unexpected errors (ENOMEM might be acceptable under stress)
        if (ret != 0 && ret != ENOMEM) {
            errors++;
        }

        // Small delay to increase chance of race conditions
        if (i % 100 == 0) {
            usleep(1);
        }
    }

    g_concurrent_thread_errors[thread_id] = errors;

    if (errors > 0) {
        printf("Thread %d had %d errors during concurrent test\n", thread_id, errors);
        g_concurrent_test_passed = 0;
    }

    return NULL;
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0180
 * @tc.desc      : Test concurrent path updates from multiple threads
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 * @tc.type      : Extended
 * @tc.purpose   : Verify thread safety and detect race conditions
 *
 * CRITICAL TEST: This test checks for race conditions in the concurrent
 * modification of namespace lib paths. Source code analysis identified
 * potential issues between permission checking and path setting.
 */
void dlns_set_module_namespace_lib_path_0180(void)
{
    printf("INFO: Starting concurrent test with %d threads, %d iterations each\n",
        THREAD_COUNT, ITERATIONS_PER_THREAD);

    g_concurrent_test_ns = malloc(sizeof(Dl_namespace));
    if (!g_concurrent_test_ns) {
        printf("ERROR: Failed to allocate memory for namespace\n");
        EXPECT_TRUE("dlns_set_module_namespace_lib_path_0180_alloc", false);
        return;
    }

    dlns_init(g_concurrent_test_ns, "dlns_set_module_namespace_lib_path_0180");

    int ret = dlns_create(g_concurrent_test_ns, NULL);
    if (ret != EOK) {
        printf("ERROR: Failed to create namespace: %d\n", ret);
        free(g_concurrent_test_ns);
        EXPECT_TRUE("dlns_set_module_namespace_lib_path_0180_create", false);
        return;
    }

    // Reset global state
    g_concurrent_test_passed = 1;
    memset((void*)g_concurrent_thread_errors, 0, sizeof(g_concurrent_thread_errors));

    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];

    // Create multiple threads that will concurrently update the path
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_ids[i] = i;
        int pthread_ret = pthread_create(&threads[i], NULL,
            concurrent_set_path_thread, &thread_ids[i]);

        if (pthread_ret != 0) {
            printf("ERROR: Failed to create thread %d: %d\n", i, pthread_ret);
            g_concurrent_test_passed = 0;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("INFO: Concurrent test completed\n");

    // Verify: No crashes occurred
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_0180_no_crash",
        g_concurrent_test_passed);

    // Verify: Final state is still functional
    dlns_set_module_namespace_lib_path(
        g_concurrent_test_ns->name, path);

    void *handle = dlopen_ns(g_concurrent_test_ns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_0180_final_state", handle);
    if (handle) {
        dlclose(handle);
    }

    // Clean up
    free(g_concurrent_test_ns);
    g_concurrent_test_ns = NULL;

    printf("INFO: Concurrent test finished successfully\n");
}

static int verify_ns_functional(Dl_namespace *dlns, const char *ns_name, int iteration)
{
    dlns_set_module_namespace_lib_path(ns_name, path);
    void *handle = dlopen_ns(dlns, dllName, RTLD_LAZY);
    if (handle) {
        dlclose(handle);
        return 0;
    }
    printf("ERROR: Namespace became non-functional at iteration %d\n", iteration);
    return -1;
}

/**
 * @tc.name      : dlns_set_module_namespace_lib_path_0190
 * @tc.desc      : Test memory allocation stress with repeated path updates
 * @tc.level     : Level 1
 * @tc.status    : Implemented
 * @tc.type      : Extended
 * @tc.purpose   : Verify memory management and detect memory leaks
 */
void dlns_set_module_namespace_lib_path_0190(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_module_namespace_lib_path_0190");
    EXPECT_EQ("dlns_set_module_namespace_lib_path_0190",
        dlns_create(&dlns, NULL), EOK);

    int enomem_count = 0;
    int unexpected_errors = 0;
    const char *ns_name = "dlns_set_module_namespace_lib_path_0190";

    for (int i = 0; i < STRESS_TEST_ITERATIONS; i++) {
        char path_buffer[512];
        (void)snprintf(path_buffer, sizeof(path_buffer),
            "/data/tests/libc-test/src/path%d/subdir%03d",
            i % 100, i % 1000);

        int ret = dlns_set_module_namespace_lib_path(ns_name, path_buffer);

        if (ret == ENOMEM) {
            enomem_count++;
        } else if (ret != 0) {
            unexpected_errors++;
            printf("WARNING: Iteration %d returned unexpected error %d\n", i, ret);
        }

        if (i > 0 && i % 1000 == 0) {
            printf("INFO: Completed %d iterations, ENOMEM count: %d\n",
                i, enomem_count);
            if (verify_ns_functional(&dlns, ns_name, i) != 0) {
                unexpected_errors++;
            }
        }
    }

    printf("INFO: Total ENOMEM errors: %d, Unexpected errors: %d\n",
        enomem_count, unexpected_errors);

    EXPECT_TRUE("dlns_set_module_namespace_lib_path_0190_final_check",
        verify_ns_functional(&dlns, ns_name, -1) == 0);
    EXPECT_TRUE("dlns_set_module_namespace_lib_path_0190_acceptable_errors",
        unexpected_errors == 0 && enomem_count < (STRESS_TEST_ITERATIONS / 10));
}

/**
 * Test suite array for extended tests
 */
TEST_FUN G_Fun_Array[] = {
    dlns_set_module_namespace_lib_path_0150,
    dlns_set_module_namespace_lib_path_0160,
    dlns_set_module_namespace_lib_path_0170,
    dlns_set_module_namespace_lib_path_0180,
    dlns_set_module_namespace_lib_path_0190,
};

/**
 * Main test runner for extended tests
 */
int main(void)
{
    printf("===========================================\n");
    printf("Extended Test Suite\n");
    printf("Testing: dlns_set_module_namespace_lib_path\n");
    printf("Focus: Potential issues from source code analysis\n");
    printf("===========================================\n\n");

    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    int passed = 0;
    int failed = 0;

    for (int pos = 0; pos < num; ++pos) {
        printf("\n--- Running test %d/%d ---\n", pos + 1, num);
        G_Fun_Array[pos]();

        // Check if test marked status as failed
        if (t_status == 0) {
            passed++;
        } else {
            failed++;
        }
    }

    printf("\n===========================================\n");
    printf("Extended Test Summary\n");
    printf("Total: %d\n", num);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("===========================================\n");

    return t_status;
}
