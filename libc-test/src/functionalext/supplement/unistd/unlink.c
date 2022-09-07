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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "test.h"

/**
 * @tc.name      : unlink_0100
 * @tc.desc      : Test the timegm result when tm exceeds the limit
 * @tc.level     : Level 0
 */
void unlink_0100(void)
{
    int fd = open("/data/tests/libc-test/src/test_unlink.txt", O_CREAT);
    int error_code = -1;
    if (fd == error_code) {
        t_error("%s unlink create file error", __func__);
        return;
    }
    close(fd);
    int result = unlink("/data/tests/libc-test/src/test_unlink.txt");
    if (result != 0) {
        t_error("%s unlink get result is %d not want 0", __func__, result);
    }
}

/**
 * @tc.name      : unlink_0200
 * @tc.desc      : Test the timegm result when tm exceeds the limit
 * @tc.level     : Level 1
 */
void unlink_0200(void)
{
    int result = unlink("/data/tests/libc-test/src/unexist_test_unlink.txt");
    int error_code = -1;
    if (result != error_code) {
        t_error("%s unlink get result is %d not want -1", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    unlink_0100();
    unlink_0200();
    return t_status;
}