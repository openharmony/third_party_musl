/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <signal.h>

#include "test.h"

static int handler_count = 0;

void handler(int sig)
{
    handler_count++;
}

/**
 * @tc.name      : sighold_0100
 * @tc.desc      : add sig to the signal mask of the calling process
 * @tc.level     : Level 0
 */
void sighold_0100(void)
{
    handler_count = 0;

    int sig = SIGALRM;
    struct sigaction act = {.sa_flags = 0, .sa_handler = handler};
    struct sigaction old_act = {0};
    int result = sigaction(sig, &act, &old_act);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    result = sighold(sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    sigset_t set = {0};
    result = sigprocmask(SIG_SETMASK, NULL, &set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    result = sigismember(&set, sig);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    result = raise(sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (handler_count != 0) {
        t_error("%s failed: handler_count = %d\n", __func__, handler_count);
    }
}

/**
 * @tc.name      : sighold_0200
 * @tc.desc      : add an invalid sig to the signal mask of the calling process
 * @tc.level     : Level 2
 */
void sighold_0200(void)
{
    errno = 0;
    int sig = 99999;
    int result = sighold(sig);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sighold_0100();
    sighold_0200();

    return t_status;
}
