/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <poll.h>
#include "functionalext.h"


/**
 * 该用例未在编译构建文件内开启，需手动开启，添加到test_src_functionalext_supplement_network.gni内
 * 该用例需要两台测试设备，两台设备连接同一热点网络，本文件产物作为server端
 * 测试前，client端设备执行ipconfig -a指令，查询wlan0 ipv6地址，设置给client端dst addr
 * server端程序为循环监听，除非系统调用发生异常退出，否则需手动退出
 */
int main(void) {
    int udp6_socket, ret;
    socklen_t addr_len;
    struct sockaddr_in6 saddr = {0};
    struct sockaddr_in6 daddr = {0};
    char buffer[] = "test.check.platform.com A 111.111.11.11";
    char recvbuff[1024] = {0};
    struct pollfd fds[1];

    saddr.sin6_addr = in6addr_any;
    saddr.sin6_family = AF_INET6;
    saddr.sin6_port = htons(40000);
    saddr.sin6_scope_id = 0;

    daddr.sin6_family = AF_INET6;

    udp6_socket = socket(AF_INET6, SOCK_DGRAM, 0);
    if (udp6_socket == -1) {
        t_error("%s create udp6 socket failed, errno is %d\n", __func__, errno);
        return t_status;
    }

    ret = setsockopt(udp6_socket, IPPROTO_IPV6, IPV6_V6ONLY, &(int){0}, sizeof 0);
    if (ret == -1) {
        close(udp6_socket);
        t_error("%s setsockopt failed, errno is %d\n", __func__, errno);
        return t_status;
    }

    if (bind(udp6_socket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in6))) {
        close(udp6_socket);
        t_error("%s udp6 bind addr failed, errno is %d\n", __func__, errno);
        return t_status;
    }

    fds[0].fd = udp6_socket;
    fds[0].events = POLLIN;

    while (1) {
        ret = poll(fds, 1, -1);
        if (ret < 0) {
            t_error("%s poll failed, errno is %d\n", __func__, errno);
            goto out;
        }

        ret = recvfrom(udp6_socket, recvbuff, 1024, 0, (struct sockaddr *)&daddr, &addr_len);
        if (ret < 0) {
            t_error("%s udp6 recv failed, errno is %d\n", __func__, errno);
            goto out;
        }

        ret = sendto(udp6_socket, &buffer[0], sizeof(buffer), 0, (struct sockaddr *)&daddr, sizeof(struct sockaddr_in6));
    }

out:
    close(udp6_socket);
    return t_status;
}