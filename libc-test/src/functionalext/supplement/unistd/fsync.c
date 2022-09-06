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

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include "functionalext.h"
#include <sys/stat.h>

/**
 * @tc.name      : fsync_0100
 * @tc.desc      : Verify that the kernel's cache can be flushed (parameter invalid)
 * @tc.level     : Level 0
 */
void fsync_0100(void)
{
    const char *msg = "This is a c test code for write function";
    const char *path = "/data/tests/libc-test/src/functionalext/supplement/unistd/test_write.txt";
    int len = strlen(msg);
    struct stat buf;

    int fd = open(path, O_RDWR | O_RSYNC | O_CREAT, 0777);
    EXPECT_TRUE("fsync_0100", fd >= 0);

    int assos = write(fd, msg, len);
    EXPECT_EQ("fsync_0100", assos, 40);

    int result = fsync(fd);
    EXPECT_EQ("fsync_0100", result, 0);

    close(fd);
    remove(path);
}

/**
 * @tc.name      : fsync_0200
 * @tc.desc      : Verify that the kernel's cache cannot be flushed (fd parameter invalid file close)
 * @tc.level     : Level 2
 */
void fsync_0200(void)
{
    int fd, size;
    char w[] = "this is a good boy!\n";
    fd = open("data/test.txt", S_IRWXO);
    write(fd, w, sizeof("w"));
    int result = fsync(fd);
    close(fd);
    EXPECT_EQ("fsync_0200", result, -1);
}

/**
 * @tc.name      : fsync_0300
 * @tc.desc      : Verify that the kernel's cache cannot be flushed (fd parameter is invalid)
 * @tc.level     : Level 2
 */
void fsync_0300(void)
{
    int fd = -1;
    int result = fsync(fd);
    EXPECT_EQ("fsync_0300", result, -1);
}

int main()
{
    fsync_0100();
    fsync_0200();
    fsync_0300();

    return t_status;
}