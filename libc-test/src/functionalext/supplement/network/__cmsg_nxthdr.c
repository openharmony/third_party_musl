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

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include <sys/types.h>

static int test_data[] = {
    42, 64,
};

static void fill_control_buffer(char *control_buf, size_t control_len) {
    struct cmsghdr *cmsg;
    int *data;

    // first control msg
    cmsg = (struct cmsghdr *)control_buf;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    data = (int *)CMSG_DATA(cmsg);
    *data = test_data[0];

    // second control msg
    cmsg = (struct cmsghdr *)(control_buf + CMSG_SPACE(sizeof(int)));
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    data = (int *)CMSG_DATA(cmsg);
    *data = test_data[1];
}

static void fill_control_buffer_align(char *control_buf, size_t control_len) {
    struct cmsghdr *cmsg;
    int *data;
    char *char_data;

    // first control msg
    cmsg = (struct cmsghdr *)control_buf;
    cmsg->cmsg_len = CMSG_LEN(sizeof(char));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    char_data = (char *)CMSG_DATA(cmsg);
    *char_data = 'a';

    // second control msg
    cmsg = (struct cmsghdr *)(control_buf + CMSG_SPACE(sizeof(char)));
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    data = (int *)CMSG_DATA(cmsg);
    *data = test_data[0];

    cmsg = (struct cmsghdr *)(control_buf + CMSG_SPACE(sizeof(int) + sizeof(char)));
    cmsg->cmsg_len = CMSG_LEN(sizeof(char)*3);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    char_data = (char *)CMSG_DATA(cmsg);
    *char_data = 'a';
    *(char_data + 1) = 'b';
    *(char_data + 2) = 'c';
}

static void fill_control_buffer_len_error(char *control_buf, size_t control_len) {
    struct cmsghdr *cmsg;
    int *data;

    cmsg = (struct cmsghdr *)control_buf;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int)) - 1;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    data = (int *)CMSG_DATA(cmsg);
    *data = test_data[0];
}

/**
 * @tc.name      : __cmsg_nxthdr_0100
 * @tc.desc      : Use the __cmsg_nxthdr function to control messages.
 * @tc.level     : Level 0
 */
void __cmsg_nxthdr_0100(void)
{
    char control_buf[1024];
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;

    fill_control_buffer(control_buf, sizeof(control_buf));
    msg.msg_control = control_buf;
    msg.msg_controllen = CMSG_SPACE(sizeof(int)) * 2;

    int count = 0;
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = __cmsg_nxthdr(&msg, cmsg)) {
        if (cmsg->cmsg_level != SOL_SOCKET) {
            t_error("%s cmsg_level error cmsg->cmsg_level=%d \n", __func__, cmsg->cmsg_level);
        }
        if (cmsg->cmsg_type != SCM_RIGHTS) {
            t_error("%s cmsg_level error cmsg->cmsg_type=%d \n", __func__, cmsg->cmsg_type);
        }
        if (*(int *)CMSG_DATA(cmsg) != test_data[count]) {
            t_error("%s test_data error test_data=%d \n", __func__, *(int *)CMSG_DATA(cmsg));
        }
        count++;
    }
}

/**
 * @tc.name      : __cmsg_nxthdr_0200
 * @tc.desc      : Use the __cmsg_nxthdr function to control messages, cmsg_len error.
 * @tc.level     : Level 0
 */
void __cmsg_nxthdr_0200(void)
{
    char control_buf[1024];
    struct msghdr msg = {0};
    struct cmsghdr cmsg = {0};

    fill_control_buffer_len_error(control_buf, sizeof(control_buf));
    msg.msg_control = control_buf;
    msg.msg_controllen = CMSG_SPACE(sizeof(int));
    if (__cmsg_nxthdr(&msg, &cmsg) != NULL) {
        t_error("%s cmsg error \n", __func__);
    }
}

/**
 * @tc.name      : __cmsg_nxthdr_0300
 * @tc.desc      : Use the __cmsg_nxthdr function to control messages. align
 * @tc.level     : Level 0
 */
void __cmsg_nxthdr_0300(void)
{
    char control_buf[1024];
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;

    fill_control_buffer_align(control_buf, sizeof(control_buf));
    msg.msg_control = control_buf;
    msg.msg_controllen = CMSG_SPACE(sizeof(int)+sizeof(char)*4);

    int count = 0;
    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = __cmsg_nxthdr(&msg, cmsg)) {
        if (cmsg->cmsg_level != SOL_SOCKET) {
            t_error("%s cmsg_level error cmsg->cmsg_level=%d \n", __func__, cmsg->cmsg_level);
        }
        if (cmsg->cmsg_type != SCM_RIGHTS) {
            t_error("%s cmsg_level error cmsg->cmsg_type=%d \n", __func__, cmsg->cmsg_type);
        }
        switch (count) {
            case 0:
                if (*(char *)CMSG_DATA(cmsg) != 'a') {
                    t_error("%s test_data error test_data=%d \n", __func__, *(int *)CMSG_DATA(cmsg));
                }
                break;
            case 1:
                if (*(int *)CMSG_DATA(cmsg) != test_data[0]) {
                    t_error("%s test_data error test_data=%d \n", __func__, *(int *)CMSG_DATA(cmsg));
                }
                break;
            case 2:
                if (*(char *)CMSG_DATA(cmsg) != 'a' && *((char *)CMSG_DATA(cmsg)+1) != 'b' &&
                    *((char *)CMSG_DATA(cmsg)+2) != 'c') {
                    t_error("%s test_data error test_data=%d \n", __func__, *(int *)CMSG_DATA(cmsg));
                }
                break;
            default:
                break;
        }
        count++;
    }
}

int main(int argc, char *argv[])
{
    __cmsg_nxthdr_0100();
    __cmsg_nxthdr_0200();
    __cmsg_nxthdr_0300();
    return t_status;
}