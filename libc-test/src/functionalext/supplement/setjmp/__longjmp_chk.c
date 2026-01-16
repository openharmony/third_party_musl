/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/wait.h>
#include "functionalext.h"
#include "test.h"

const int RETURN_456 = 456;
static jmp_buf g_jmp_env;

void FAIL(void)
{
    t_error("%s failed, this function should be unreachable\n", __func__);
}

/**
 * @tc.name      : __longjmp_chk_0100
 * @tc.desc      : Restore the environment saved by the  most recent invocation of setjmp() in the same process, with
 *                 the corresponding jmp_buf argument
 * @tc.level     : Level 0
 */
void __longjmp_chk_0100(void)
{
    jmp_buf jb;
    int value = setjmp(jb);
    if (value == 0) {
        __longjmp_chk(jb, RETURN_456);
        FAIL();
    } else {
        if (value != RETURN_456) {
            t_error("%s _longjmp failed\n", __func__);
        }
    }
}

/**
 * @tc.name      : __longjmp_chk_0200
 * @tc.desc      : Multiple transcoding test
 * @tc.level     : Level 0
 */
void __longjmp_chk_0200(void)
{
    int ret_code = 0;
    int test_codes[] = {1, 2, 3, 10, 20};
    int code_count = sizeof(test_codes) / sizeof(int);

    for (int i = 0; i < code_count; i++) {
        ret_code = setjmp(g_jmp_env);
        if (ret_code == 0) {
            __longjmp_chk(g_jmp_env, test_codes[i]);
            break;
        } else {
            EXPECT_EQ(__FUNCTION__, test_codes[i], ret_code);
        }
    }
}

/**
 * @tc.name      : __longjmp_chk_0300
 * @tc.desc      : Security verification test
 * @tc.level     : Level 0
 */
void __longjmp_chk_0300(void)
{
    int ret_code = 0;

    ret_code = setjmp(g_jmp_env);
    if (ret_code == 0) {
        __longjmp_chk(g_jmp_env, 66);
    } else {
        EXPECT_EQ(__FUNCTION__, ret_code, 66);
    }

    pid_t pid = fork();
    if (pid == 0) {
        jmp_buf invalid_env;
        __longjmp_chk(invalid_env, 99);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        EXPECT_TRUE(__FUNCTION__, WIFSIGNALED(status));
        EXPECT_EQ(__FUNCTION__, WTERMSIG(status), SIGSEGV);
    }
}

/**
 * @tc.name      : __longjmp_chk_0400
 * @tc.desc      : Compatibility Testing
 * @tc.level     : Level 0
 */
void __longjmp_chk_0400(void)
{
    int ret_code = 0;

    ret_code = setjmp(g_jmp_env);
    if (ret_code == 0) {
        __longjmp_chk(g_jmp_env, 88);
    } else {
        EXPECT_EQ(__FUNCTION__, ret_code, 88);
    }
}

/**
 * @tc.name      : __longjmp_chk_0500
 * @tc.desc      : Boundary scenario testing
 * @tc.level     : Level 0
 */
void __longjmp_chk_0500(void)
{
    int ret_code = 0;

    ret_code = setjmp(g_jmp_env);
    if (ret_code == 0) {
        __longjmp_chk(g_jmp_env, 0);
    } else {
        EXPECT_EQ(__FUNCTION__, ret_code, 1);
    }

    for (int i = 0; i < 3; i++) {
        ret_code = setjmp(g_jmp_env);
        if (ret_code == 0) {
            __longjmp_chk(g_jmp_env, 100 + i);
            break;
        } else {
            EXPECT_EQ(__FUNCTION__, ret_code, i + 100);
        }
    }
}

void* mt_worker(void* arg)
{
    __longjmp_chk_0200();
    return NULL;
}

/**
 * @tc.name      : __longjmp_chk_0600
 * @tc.desc      : Stability Testing.
 * @tc.level     : Level 0
 */
void __longjmp_chk_0600(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(__FUNCTION__, pthread_create(&tids[i], NULL, mt_worker, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    __longjmp_chk_0100();
    __longjmp_chk_0200();
    __longjmp_chk_0300();
    __longjmp_chk_0400();
    __longjmp_chk_0500();
    __longjmp_chk_0600();

    return t_status;
}