/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : eventfd_read_0100
 * @tc.desc      : The flags parameter is equal to EFD_SEMAPHORE, which creates an eventfd object.
 * @tc.level     : Level 0
 */
void eventfd_read_0100(void)
{
    int fd, write_ret, read_ret;
    uint64_t value;
    fd = eventfd(1, EFD_NONBLOCK);
    write_ret = eventfd_write(fd, 20);
    read_ret = eventfd_read(fd, &value);
    EXPECT_EQ("eventfd_read_0100", read_ret, 0);
}

/**
 * @tc.name      : eventfd_read_0200
 * @tc.desc      : The flags parameter is equal to EFD_CLOEXEC, which creates an eventfd object.
 * @tc.level     : Level 2
 */
void eventfd_read_0200(void)
{
    int fd, write_ret, read_ret;
    fd = eventfd(1, EFD_NONBLOCK);
    write_ret = eventfd_write(fd, 20);
    read_ret = eventfd_read(fd, NULL);
    EXPECT_EQ("eventfd_read_0200", read_ret, -1);
}

/**
 * @tc.name      : eventfd_read_0300
 * @tc.desc      : The flags parameter is equal to EFD_NONBLOCK, which creates an eventfd object.
 * @tc.level     : Level 2
 */
void eventfd_read_0300(void)
{
    int fd, write_ret, read_ret;
    uint64_t value;
    read_ret = eventfd_read(-1, &value);
    EXPECT_EQ("eventfd_read_0300", read_ret, -1);
}

int main()
{
    eventfd_read_0100();
    eventfd_read_0200();
    eventfd_read_0300();
    return t_status;
}