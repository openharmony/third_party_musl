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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <link.h>
#include <string.h>
#include <errno.h>

#include "functionalext.h"

#define RTLD_DL_LINKMAP (2)
#define RTLD_DL_SYMENT  (1)

// ---------- 辅助定义 ----------
static void static_internal_func(void)
{
    volatile int x = 0xDEADBEEF;
    (void)x;
}

#define INVALID_ADDR ((void*)0x1)

static void* offset_plus_N(const void* base)
{
    return (char*)base + 8;
}

bool run_test(const void* addr, bool extra_info_valid, int flags, bool expected_success, bool need_extra_info_success)
{
    Dl_info info;
    ElfW(Sym) *extra_info = NULL;
    int ret = dladdr1(addr, &info, (void **)(extra_info_valid ? &extra_info : NULL), flags);
    bool actual_success = (ret != 0);
    actual_success = (actual_success == expected_success);
    if (!actual_success) {
        printf("dladdr1 actual_success != expected_success"
            " actual_success=%s expected_success=%s\n",
            (ret != 0) ? "true" : "false",
            expected_success ? "true" : "false");
        return false;
    }
    if (actual_success && need_extra_info_success) {
        bool actual_extra_info_success = (extra_info != NULL);
        actual_success = actual_success && (actual_extra_info_success == need_extra_info_success);
        if (!actual_success) {
            printf("extra_info actual_extra_info_success != need_extra_info_success"
                "actual_extra_info_success=%s need_extra_info_success=%s\n",
                actual_extra_info_success ? "true" : "false",
                need_extra_info_success ? "true" : "false");
            return false;
        }
        if (need_extra_info_success) {
            actual_success = actual_success && (extra_info->st_value != 0);
            if (!actual_success) {
                printf("extra_info->st_value == 0\n");
                return false;
            }
            actual_success = actual_success && (extra_info->st_size != 0);
            if (!actual_success) {
                printf("extra_info->st_size == 0\n");
                return false;
            }
            actual_success = actual_success && (extra_info->st_name != 0);
            if (!actual_success) {
                printf("extra_info->st_name != malloc extra_info->st_name=%s\n", extra_info->st_name);
                return false;
            }
        }
    }
    return actual_success;
}

// ---------- 24 个独立测试用例 ----------
static const void* get_addr(int type)
{
    switch (type) {
        case 1: return (void*)malloc;          // A1: 有效基准对外暴露
        case 2: return (void*)static_internal_func;          // A2: 有效基准内部函数
        case 3: return offset_plus_N(malloc);  // A3: 有效偏移对外暴露
        case 4: return offset_plus_N(static_internal_func);  // A4: 有效偏移内部函数
        case 5: return NULL;                                  // A5: 空指针
        case 6: return INVALID_ADDR;                          // A6: 无效非法地址
        default: return NULL;
    }
}

// 每个测试函数独立定义（按 6×2×2 顺序展开）
bool test_case_1(void)
{
    return run_test(get_addr(1), false, RTLD_DL_LINKMAP, true, false);
}
bool test_case_2(void)
{
    return run_test(get_addr(1), false, RTLD_DL_SYMENT,  true, false);
}
bool test_case_3(void)
{
    return run_test(get_addr(1), true,  RTLD_DL_LINKMAP, true, false);
}
bool test_case_4(void)
{
    return run_test(get_addr(1), true,  RTLD_DL_SYMENT,  true, true);
}
bool test_case_5(void)
{
    return run_test(get_addr(2), false, RTLD_DL_LINKMAP, true, false);
}
bool test_case_6(void)
{
    return run_test(get_addr(2), false, RTLD_DL_SYMENT,  true, false);
}
bool test_case_7(void)
{
    return run_test(get_addr(2), true,  RTLD_DL_LINKMAP, true, false);
}
bool test_case_8(void)
{
    return run_test(get_addr(2), true,  RTLD_DL_SYMENT,  true, false);
}
bool test_case_9(void)
{
    return run_test(get_addr(3), false, RTLD_DL_LINKMAP, true, false);
}
bool test_case_10(void)
{
    return run_test(get_addr(3), false, RTLD_DL_SYMENT,  true, false);
}
bool test_case_11(void)
{
    return run_test(get_addr(3), true,  RTLD_DL_LINKMAP, true, false);
}
bool test_case_12(void)
{
    return run_test(get_addr(3), true,  RTLD_DL_SYMENT,  true, true);
}
bool test_case_13(void)
{
    return run_test(get_addr(4), false, RTLD_DL_LINKMAP, true, false);
}
bool test_case_14(void)
{
    return run_test(get_addr(4), false, RTLD_DL_SYMENT,  true, false);
}
bool test_case_15(void)
{
    return run_test(get_addr(4), true,  RTLD_DL_LINKMAP, true, false);
}
bool test_case_16(void)
{
    return run_test(get_addr(4), true,  RTLD_DL_SYMENT,  true, false);
}
bool test_case_17(void)
{
    return run_test(get_addr(5), false, RTLD_DL_LINKMAP, false, false);
}
bool test_case_18(void)
{
    return run_test(get_addr(5), false, RTLD_DL_SYMENT,  false, false);
}
bool test_case_19(void)
{
    return run_test(get_addr(5), true,  RTLD_DL_LINKMAP, false, false);
}
bool test_case_20(void)
{
    return run_test(get_addr(5), true,  RTLD_DL_SYMENT,  false, false);
}
bool test_case_21(void)
{
    return run_test(get_addr(6), false, RTLD_DL_LINKMAP, false, false);
}
bool test_case_22(void)
{
    return run_test(get_addr(6), false, RTLD_DL_SYMENT,  false, false);
}
bool test_case_23(void)
{
    return run_test(get_addr(6), true,  RTLD_DL_LINKMAP, false, false);
}
bool test_case_24(void)
{
    return run_test(get_addr(6), true,  RTLD_DL_SYMENT,  false, false);
}

// 函数指针数组（索引 0 对应 test_case_1）
bool (*test_funcs[24])(void) = {
    test_case_1,  test_case_2,  test_case_3,  test_case_4,
    test_case_5,  test_case_6,  test_case_7,  test_case_8,
    test_case_9,  test_case_10, test_case_11, test_case_12,
    test_case_13, test_case_14, test_case_15, test_case_16,
    test_case_17, test_case_18, test_case_19, test_case_20,
    test_case_21, test_case_22, test_case_23, test_case_24
};

// ---------- 多线程测试（第25个）----------
void* mt_worker(void* arg)
{
    Dl_info info;
    dladdr1((void*)static_internal_func, &info, NULL, RTLD_DL_LINKMAP);
    return NULL;
}

bool test_multithread(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        if (pthread_create(&tids[i], NULL, mt_worker, NULL) != 0) {
            printf("pthread_create failed errno=%d\n", errno);
            return false;
        }
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
    return true; // 假设无崩溃即通过
}

int main(void)
{
    printf("Running 24 functional test cases...\n");
    int passed = 0;
    for (int i = 0; i < 24; i++) {
        bool ok = test_funcs[i]();
        printf("Test %02d: %s\n", i + 1, ok ? "PASS" : "FAIL");
        EXPECT_EQ("Test failed", ok, true);
    }

    printf("\nRunning multi-thread safety test (Test 25)...\n");
    bool mt_ok = test_multithread();
    printf("Test 25: %s\n", mt_ok ? "PASS" : "FAIL");
    EXPECT_EQ("Test failed", ok, true);

    printf("\n=== Summary: %d / 25 tests passed ===\n", passed);
    return t_status;
}