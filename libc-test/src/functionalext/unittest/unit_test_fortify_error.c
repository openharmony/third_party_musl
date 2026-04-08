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

#include <fortify/fortify.h>
#include <info/fatal_message.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functionalext.h"

typedef void (*TEST_FUN)(void);

static const char EXPECTED_FATAL_MESSAGE[] =
    "Musl Fortify runtime error: fortify fatal test 42";

static void fortify_abort_handler(int signo)
{
    (void)signo;
    fatal_msg_t *fatal_message = get_fatal_message();
    if (fatal_message != NULL &&
        strcmp(fatal_message->msg, EXPECTED_FATAL_MESSAGE) == 0) {
        _exit(0);
    }
    _exit(1);
}

/**
 * @tc.name      : fortify_error_0010
 * @tc.desc      : Test that __fortify_error saves the composed fatal message before abort.
 * @tc.level     : Level 0
 */
static void fortify_error_0010(void)
{
    pid_t pid = fork();
    EXPECT_NE("fortify_error_0010", pid, -1);
    if (pid == 0) {
        struct sigaction action = { 0 };
        action.sa_handler = fortify_abort_handler;
        sigemptyset(&action.sa_mask);
        sigaction(SIGABRT, &action, NULL);
        __fortify_error("fortify fatal test %d", 42);
        _exit(2);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    EXPECT_TRUE("fortify_error_0010 child exited", WIFEXITED(status));
    EXPECT_EQ("fortify_error_0010 exit code", WEXITSTATUS(status), 0);
}

TEST_FUN G_Fun_Array[] = {
    fortify_error_0010,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
