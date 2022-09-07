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
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>

#include "test.h"

const char *fromfile = "/data/tests/libc-test/src/fromfile.txt";
const char *tofile = "/data/tests/libc-test/src/tofile.txt";

/**
 * @tc.name      : sendfile_0100
 * @tc.desc      : transfer data between file descriptors
 * @tc.level     : Level 0
 */
void sendfile_0100(void)
{
    FILE *f = fopen(fromfile, "w+");
    const char src[] = "A";
    fwrite(src, strlen(src), 1, f);
    fclose(f);

    f = fopen(tofile, "w+");
    fclose(f);

    int fromfd = open(fromfile, O_RDONLY);
    if (fromfd < 0) {
        t_error("%s failed: open. fromfd = %d\n", __func__, fromfd);
    }

    int tofd = open(tofile, O_WRONLY | O_CREAT);
    if (tofd < 0) {
        t_error("%s failed: open. tofd = %d\n", __func__, tofd);
    }

    off_t off = 0;
    int result = sendfile(tofd, fromfd, &off, 1);
    if (result < 0) {
        t_error("%s failed: sendfile. result = %d\n", __func__, result);
    }

    close(fromfd);
    close(tofd);

    f = fopen(tofile, "r");
    char buf[sizeof(src) + 1] = {0};
    fread(buf, 1, strlen(src), f);
    fclose(f);

    if (strcmp(src, buf)) {
        t_error("%s failed: sendfile. buf = %s\n", __func__, buf);
    }

    remove(fromfile);
    remove(tofile);
}

/**
 * @tc.name      : sendfile_0200
 * @tc.desc      : transfer data between invalid file descriptors
 * @tc.level     : Level 2
 */
void sendfile_0200(void)
{
    int fromfd = -1;
    int tofd = -1;
    off_t off = 0;

    int result = sendfile(tofd, fromfd, &off, 1);
    if (result >= 0) {
        t_error("%s failed: sendfile. result = %d\n", __func__, result);
    }

    if (errno != EBADF) {
        t_error("%s failed: sendfile. errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sendfile_0100();
    sendfile_0200();

    return t_status;
}
