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

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "test.h"

const char *path = "/data/tests/file.txt";
const char *linkpath = "/data/tests/linkfile.txt";
const char buf[] = "hello";
const char *dirname = "/data/tests";
const char *filename = "./file.txt";
const char *linkfilename = "./linkfile.txt";

int create_file(void)
{
    int fd = open(path, O_RDWR | O_CREAT);
    if (fd < 0) {
        return -1;
    }

    ssize_t bytes = write(fd, buf, sizeof(buf));
    if (bytes <= 0) {
        return -1;
    }

    int result = close(fd);
    if (result != 0) {
        return -1;
    }

    return 0;
}

/**
 * @tc.name      : readlinkat_0100
 * @tc.desc      : read value of a symbolic link
 * @tc.level     : Level 0
 */
void readlinkat_0100(void)
{
    int result = create_file();
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    remove(linkpath);

    result = symlink(path, linkpath);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    int fd = open(linkpath, O_RDONLY);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
        return;
    }

    char rbuf[BUFSIZ] = {0};
    ssize_t bytes = readlinkat(fd, linkpath, rbuf, sizeof(rbuf));
    if (bytes != strlen(rbuf)) {
        t_error("%s failed: bytes = %ld\n", __func__, bytes);
    }

    if (strcmp(rbuf, path)) {
        t_error("%s failed: rbuf = %s\n", __func__, rbuf);
    }

    remove(linkpath);
    remove(path);
}

/**
 * @tc.name      : readlinkat_0200
 * @tc.desc      : read value of a symbolic link with a dir fd
 * @tc.level     : Level 1
 */
void readlinkat_0200(void)
{
    int result = create_file();
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        t_error("%s failed: dirname = %s\n", __func__, dirname);
        return;
    }

    int fd = dirfd(dir);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    remove(linkpath);

    result = symlinkat(filename, fd, linkfilename);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    char rbuf[BUFSIZ] = {0};
    ssize_t bytes = readlinkat(fd, linkfilename, rbuf, sizeof(rbuf));
    if (bytes != strlen(rbuf)) {
        t_error("%s failed: bytes = %ld\n", __func__, bytes);
    }

    if (strcmp(rbuf, filename)) {
        t_error("%s failed: rbuf = %s\n", __func__, rbuf);
    }

    remove(linkpath);
    remove(path);
}

/**
 * @tc.name      : readlinkat_0300
 * @tc.desc      : read value of a symbolic link with invalid parameters
 * @tc.level     : Level 2
 */
void readlinkat_0300(void)
{
    ssize_t bytes = readlinkat(-1, NULL, NULL, 0);
    if (bytes >= 0) {
        t_error("%s failed: bytes = %ld\n", __func__, bytes);
    }
}

int main(int argc, char *argv[])
{
    readlinkat_0100();
    readlinkat_0200();
    readlinkat_0300();

    return t_status;
}
