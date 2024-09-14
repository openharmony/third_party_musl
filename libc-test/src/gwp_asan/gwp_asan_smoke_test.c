/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <malloc.h>
#include <stdint.h>
#include <sys/wait.h>
#include "gwp_asan_test.h"
#include "test.h"

#define NUM_OF_MALLOC_FOR_OOM 10000
#define SIZE_OF_SINGLE_MALLOC 20

void test_gwp_asan_oom()
{
    if (libc_gwp_asan_has_free_mem()) {
        for (size_t i = 0; i < NUM_OF_MALLOC_FOR_OOM; i++) {
            void *ptr = malloc(SIZE_OF_SINGLE_MALLOC);
            if (!ptr) {
                t_error("FAIL malloc failed!\n");
            }
        }
    } else {
        t_error("FAIL can't use gwp_asan!\n");
    }
    if (!libc_gwp_asan_has_free_mem()) {
        // test we can call default malloc when gwp_asan doesn't have free mem.
        void *ptr = malloc(SIZE_OF_SINGLE_MALLOC);
        if (!ptr) {
            t_error("FAIL malloc failed!\n");
        }
        if (libc_gwp_asan_ptr_is_mine(ptr)) {
            t_error("FAIL this addr(%p) shouldn't in gwp_asan area!\n", ptr);
        } else {
            printf("It's ok call default malloc\n");
        }
    } else {
        t_error("FAIL gwp_asan doesn't oom!\n");
    }
}

int main()
{
    config_gwp_asan_environment(false);
    
    void *ptr = malloc(2);
    if (!ptr) {
        t_error("malloc failed.");
        cancel_gwp_asan_environment(false);
        return t_status;
    }
    if (!libc_gwp_asan_ptr_is_mine(ptr)) {
        t_error("Memory is not allocated by gwp_asan.");
        cancel_gwp_asan_environment(false);
        return t_status;
    }
    int size = malloc_usable_size(ptr);
    void *new_addr = realloc(ptr, 40);
    if (!new_addr) {
        t_error("realloc failed.");
        cancel_gwp_asan_environment(false);
        return t_status;
    }
    char c = *(char *)new_addr;
    printf("c:%c size:%d.\n", c, size);

    void *calloc_ptr = calloc(5, 4);
    if (!calloc_ptr) {
        t_error("calloc failed.");
        cancel_gwp_asan_environment(false);
        return t_status;
    }
    int value = *(int *)calloc_ptr;
    if (value != 0) {
        t_error("FAIL the memory value of gwp_asan calloc isn't 0 get %d", value);
    }

    free(new_addr);

    pid_t pid = fork();
    if (pid < 0) {
        t_error("FAIL fork failed.");
    } else if (pid == 0) { // child process
        test_gwp_asan_oom();
    } else { // parent process
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            t_error("gwp_asan_smoke_test waitpid failed.");
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            t_error("gwp_asan_smoke_test failed.");
        }

        cancel_gwp_asan_environment(false);
    }

    return t_status;
}