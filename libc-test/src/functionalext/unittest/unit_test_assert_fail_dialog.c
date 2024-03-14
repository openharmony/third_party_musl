/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <assert.h>
#include <info/fatal_message.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#undef assert
#define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))

Assert_Status CallbackFunctionAbort(AssertFailureInfo assert_fail)
{
    printf("\nFatal message: %s\n", assert_fail.expression);
    printf("Fatal message: %s\n", assert_fail.file);
    printf("Fatal message: %s\n", assert_fail.function);
    printf("Fatal message: %d\n", assert_fail.line);
    Assert_Status res = ASSERT_ABORT;
    return res;
}

Assert_Status CallbackFunctionIgnore(AssertFailureInfo assert_fail)
{
    printf("\nFatal message: %s\n", assert_fail.expression);
    printf("Fatal message: %s\n", assert_fail.file);
    printf("Fatal message: %s\n", assert_fail.function);
    printf("Fatal message: %d\n", assert_fail.line);
    Assert_Status res = ASSERT_IGNORE;
    return res;
}

Assert_Status CallbackFunctionRetry(AssertFailureInfo assert_fail)
{
    printf("\nFatal message: %s\n", assert_fail.expression);
    printf("Fatal message: %s\n", assert_fail.file);
    printf("Fatal message: %s\n", assert_fail.function);
    printf("Fatal message: %d\n", assert_fail.line);
    Assert_Status res = ASSERT_RETRY;
    return res;
}

void ProcessAbort()
{
    set_assert_callback(CallbackFunctionAbort);
    assert(0);
}

void ProcessIgnore()
{
    set_assert_callback(CallbackFunctionIgnore);
    assert(0);
}

void ProcessRetry()
{
    set_assert_callback(CallbackFunctionRetry);
    assert(0);
}

int main(void)
{
    pid_t pid1, pid2, pid3;
    int status;

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process One\n");
        return 1;
    } else if (pid1 == 0) {
        // Child process one
        ProcessAbort(); // Specific operations for Child Process One can be added here
        return 0;
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process Two\n");
        return 1;
    } else if (pid2 == 0) {
        // Child process two
        ProcessIgnore(); // Specific operations for Child Process Two can be added here
        return 0;
    }

    // Create the third child process
    pid3 = fork();
    if (pid3 < 0) {
        // Failed to create the process
        fprintf(stderr, "Fork failed for Process Three\n");
        return 1;
    } else if (pid3 == 0) {
        // Child process three
        ProcessRetry(); // Specific operations for Child Process Three can be added here
        return 0;
    }

    // Wait for all child processes to finish
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);

    printf("All processes finished.\n");

    return 0;
}