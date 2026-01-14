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
// testcase_cancel_invalid_tid_robust.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* dummy(void* arg) {
    return NULL;
}

int main() {
    pthread_t tid;

    // Step 1: Create and join a thread → tid becomes "invalid" per POSIX
    if (pthread_create(&tid, NULL, dummy, NULL) != 0) {
        fprintf(stderr, "pthread_create failed\n");
        return 1;
    }
    if (pthread_join(tid, NULL) != 0) {
        fprintf(stderr, "pthread_join failed\n");
        return 1;
    }

    // Step 2: Now tid is a valid object (stack variable) containing
    //         a value that no longer refers to any thread.
    //         This is the ONLY portable way to get an "invalid but safe" tid.

    // Step 3: Call pthread_cancel — behavior is undefined per POSIX,
    //         but a robust implementation should not crash.
    int r = pthread_cancel(tid);

    // We do NOT assert on return value (it's undefined).
    // We only care that the program is still running.

    printf("PASS: pthread_cancel on joined tid did not crash (r=%d).\n", r);
    return 0;
}
#endif
