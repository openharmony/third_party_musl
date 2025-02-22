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

#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <poll.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "syscall.h"
#include "functionalext.h"

struct address {
	int family;
	unsigned scopeid;
	uint8_t addr[16];
	int sortkey;
};

#define MAXNS 3
#define TCP_FASTOPEN_CONNECT 30

struct resolvconf {
	struct address ns[MAXNS];
	unsigned nns, attempts, ndots, non_public;
	unsigned timeout;
};

static void cleanup(void *p)
{
	struct pollfd *pfd = p;
	for (int i=0; pfd[i].fd >= -1; i++)
		if (pfd[i].fd >= 0) close(pfd[i].fd);
}

static unsigned long mtime()
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0 && errno == ENOSYS)
		clock_gettime(CLOCK_REALTIME, &ts);
	return (unsigned long)ts.tv_sec * 1000
		+ ts.tv_nsec / 1000000;
}

static int start_tcp(struct pollfd *pfd, int family, const void *sa,
	socklen_t sl, const unsigned char *q, int ql, int netid)
{
	struct msghdr mh = {
		.msg_name = (void *)sa,
		.msg_namelen = sl,
		.msg_iovlen = 2,
		.msg_iov = (struct iovec [2]){
			{ .iov_base = (uint8_t[]){ ql>>8, ql }, .iov_len = 2 },
			{ .iov_base = (void *)q, .iov_len = ql } }
	};
	int r;
	int fd = socket(family, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
	pfd->fd = fd;
	pfd->events = POLLOUT;
	if (!setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN_CONNECT,
	    &(int){1}, sizeof(int))) {
		r = sendmsg(fd, &mh, MSG_FASTOPEN|MSG_NOSIGNAL);
		if (r == ql+2) pfd->events = POLLIN;
		if (r >= 0) return r;
		if (errno == EINPROGRESS) return 0;
	}
	r = connect(fd, sa, sl);
	if (!r || errno == EINPROGRESS) return 0;
	close(fd);
	pfd->fd = -1;
	return -1;
}

static void step_mh(struct msghdr *mh, size_t n)
{
	/* Adjust iovec in msghdr to skip first n bytes. */
	while (mh->msg_iovlen && n >= mh->msg_iov->iov_len) {
		n -= mh->msg_iov->iov_len;
		mh->msg_iov++;
		mh->msg_iovlen--;
	}
	if (!mh->msg_iovlen) return;
	mh->msg_iov->iov_base = (char *)mh->msg_iov->iov_base + n;
	mh->msg_iov->iov_len -= n;
}

int res_msend_rc_ext(int netid, int nqueries, const unsigned char *const *queries,
	const int *qlens, unsigned char *const *answers, int *alens, int asize,
	const struct resolvconf *conf, int newcheck)
{
	int fd;
	int timeout, attempts, retry_interval, servfail_retry;
	union {
		struct sockaddr_in sin;
		struct sockaddr_in6 sin6;
	} sa = {0}, ns[MAXNS] = {{0}};
	socklen_t sl = sizeof sa.sin;
	int nns = 0;
	int family = AF_INET;
	int rlen;
	int next;
	int i, j;
	int cs;
	struct pollfd pfd[nqueries+2];
	int qpos[nqueries], apos[nqueries];
	unsigned char alen_buf[nqueries][2];
	int r;
	unsigned long t0, t1, t2;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	timeout = 1000*conf->timeout;
	attempts = conf->attempts;

	for (nns=0; nns<conf->nns; nns++) {
		const struct address *iplit = &conf->ns[nns];
		if (iplit->family == AF_INET) {
			memcpy(&ns[nns].sin.sin_addr, iplit->addr, 4);
			ns[nns].sin.sin_port = htons(53);
			ns[nns].sin.sin_family = AF_INET;
		} else {
			sl = sizeof sa.sin6;
			memcpy(&ns[nns].sin6.sin6_addr, iplit->addr, 16);
			ns[nns].sin6.sin6_port = htons(40000);
			ns[nns].sin6.sin6_scope_id = iplit->scopeid;
			ns[nns].sin6.sin6_family = family = AF_INET6;
		}
	}

	/* Get local address and open/bind a socket */
	fd = socket(family, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);

	/* Handle case where system lacks IPv6 support */
	if (fd < 0 && family == AF_INET6 && errno == EAFNOSUPPORT) {
		for (i=0; i<nns && conf->ns[nns].family == AF_INET6; i++);
		if (i==nns) {
			pthread_setcancelstate(cs, 0);
			return -1;
		}
		fd = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
		family = AF_INET;
		sl = sizeof sa.sin;
	}

	/* Convert any IPv4 addresses in a mixed environment to v4-mapped */
	if (fd >= 0 && family == AF_INET6) {
		setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &(int){0}, sizeof(int));
		for (i=0; i<nns; i++) {
			if (ns[i].sin.sin_family != AF_INET) continue;
			memcpy(ns[i].sin6.sin6_addr.s6_addr+12,
				&ns[i].sin.sin_addr, 4);
			memcpy(ns[i].sin6.sin6_addr.s6_addr,
				"\0\0\0\0\0\0\0\0\0\0\xff\xff", 12);
			ns[i].sin6.sin6_family = AF_INET6;
			ns[i].sin6.sin6_flowinfo = 0;
			ns[i].sin6.sin6_scope_id = 0;
		}
	}

	sa.sin.sin_family = family;
	if (fd < 0 || bind(fd, (void *)&sa, sl) < 0) {
		if (fd >= 0) close(fd);
		pthread_setcancelstate(cs, 0);
		return -1;
	}

	/* Past this point, there are no errors. Each individual query will
	 * yield either no reply (indicated by zero length) or an answer
	 * packet which is up to the caller to interpret. */

	for (i=0; i<nqueries; i++) pfd[i].fd = -1;
	pfd[nqueries].fd = fd;
	pfd[nqueries].events = POLLIN;
	pfd[nqueries+1].fd = -2;

	pthread_cleanup_push(cleanup, pfd);
	pthread_setcancelstate(cs, 0);

	memset(alens, 0, sizeof *alens * nqueries);

	retry_interval = timeout / attempts;
	next = 0;
	t0 = t2 = mtime();
	t1 = t2 - retry_interval;

	for (; t2-t0 < timeout; t2=mtime()) {
		/* This is the loop exit condition: that all queries
		 * have an accepted answer. */
		for (i=0; i<nqueries && alens[i]>0; i++);
		if (i==nqueries) break;

		if (t2-t1 >= retry_interval) {
			/* Query all configured namservers in parallel */
			for (i=0; i<nqueries; i++) {
				if (!alens[i]) {
					for (j=0; j<nns; j++) {
						if (sendto(fd, queries[i], qlens[i], MSG_NOSIGNAL, (void *)&ns[j], sl) == -1) {
						}
					}
				}
			}
			t1 = t2;
			servfail_retry = 2 * nqueries;
		}

		/* Wait for a response, or until time to retry */
		if (poll(pfd, nqueries+1, t1+retry_interval-t2) <= 0) continue;

		while (next < nqueries) {
			struct msghdr mh = {
				.msg_name = (void *)&sa,
				.msg_namelen = sl,
				.msg_iovlen = 1,
				.msg_iov = (struct iovec []){
					{ .iov_base = (void *)answers[next],
					  .iov_len = asize }
				}
			};
			rlen = recvmsg(fd, &mh, 0);
			if (rlen < 0) {
				break;
			}

			/* Ignore non-identifiable packets */
			if (rlen < 4) continue;

			/* Ignore replies from addresses we didn't send to */
            if (newcheck) {
                switch (sa.sin.sin_family) {
                    case AF_INET:
                        for (j = 0; j < nns; j++) {
                            if (ns[j].sin.sin_family == AF_INET && ns[j].sin.sin_port == sa.sin.sin_port &&
                            (ns[j].sin.sin_addr.s_addr == INADDR_ANY ||
                            ns[j].sin.sin_addr.s_addr == sa.sin.sin_addr.s_addr)) {
                                break;
                            }
                        }
                        break;
                    case AF_INET6:
                        for (j = 0; j < nns; j++) {
                            if (ns[j].sin6.sin6_family == AF_INET6 &&
                            ns[j].sin6.sin6_port == sa.sin6.sin6_port &&
                            (ns[j].sin6.sin6_scope_id == 0 || ns[j].sin6.sin6_scope_id == sa.sin6.sin6_scope_id) &&
                            (IN6_IS_ADDR_UNSPECIFIED(&ns[j].sin6.sin6_addr) ||
                            IN6_ARE_ADDR_EQUAL(&ns[j].sin6.sin6_addr, &sa.sin6.sin6_addr))) {
                                break;
                            }
                        }
                        break;
                    default:
                        j = nns;
                        break;
                }
            } else {
                for (j=0; j<nns && memcmp(ns+j, &sa, sl); j++);
            }
			if (j==nns) {
				memset(answers[next], 0, asize);
				continue;
			}

			/* Find which query this answer goes with, if any */
			for (i=next; i<nqueries && (
				answers[next][0] != queries[i][0] ||
				answers[next][1] != queries[i][1] ); i++);
			if (i==nqueries) continue;
			if (alens[i]) continue;

			/* Only accept positive or negative responses;
			 * retry immediately on server failure, and ignore
			 * all other codes such as refusal. */
			switch (answers[next][3] & 15) {
				case 0:
				case 3:
					break;
				case 2:
					if (servfail_retry && servfail_retry--)
						sendto(fd, queries[i],
							qlens[i], MSG_NOSIGNAL,
							(void *)&ns[j], sl);
				default:
					continue;
			}

			/* Store answer in the right slot, or update next
			 * available temp slot if it's already in place. */
			alens[i] = rlen;
			if (i == next)
				for (; next<nqueries && alens[next]; next++);
			else
				memcpy(answers[i], answers[next], rlen);

			/* Ignore further UDP if all slots full or TCP-mode */
			if (next == nqueries) pfd[nqueries].events = 0;

			/* If answer is truncated (TC bit), fallback to TCP */
			if ((answers[i][2] & 2) || (mh.msg_flags & MSG_TRUNC)) {
				alens[i] = -1;
				pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, 0);
				r = start_tcp(pfd+i, family, ns+j, sl, queries[i], qlens[i], netid);
				pthread_setcancelstate(cs, 0);
				if (r >= 0) {
					qpos[i] = r;
					apos[i] = 0;
				}
				continue;
			}
		}

		for (i=0; i<nqueries; i++) if (pfd[i].revents & POLLOUT) {
			struct msghdr mh = {
				.msg_iovlen = 2,
				.msg_iov = (struct iovec [2]){
					{ .iov_base = (uint8_t[]){ qlens[i]>>8, qlens[i] }, .iov_len = 2 },
					{ .iov_base = (void *)queries[i], .iov_len = qlens[i] } }
			};
			step_mh(&mh, qpos[i]);
			r = sendmsg(pfd[i].fd, &mh, MSG_NOSIGNAL);
			if (r < 0) goto out;
			qpos[i] += r;
			if (qpos[i] == qlens[i]+2)
				pfd[i].events = POLLIN;
		}

		for (i=0; i<nqueries; i++) if (pfd[i].revents & POLLIN) {
			struct msghdr mh = {
				.msg_iovlen = 2,
				.msg_iov = (struct iovec [2]){
					{ .iov_base = alen_buf[i], .iov_len = 2 },
					{ .iov_base = answers[i], .iov_len = asize } }
			};
			step_mh(&mh, apos[i]);
			r = recvmsg(pfd[i].fd, &mh, 0);
			if (r <= 0) goto out;
			apos[i] += r;
			if (apos[i] < 2) continue;
			int alen = alen_buf[i][0]*256 + alen_buf[i][1];
			if (alen < 13) goto out;
			if (apos[i] < alen+2 && apos[i] < asize+2)
				continue;
			int rcode = answers[i][3] & 15;
			if (rcode != 0 && rcode != 3)
				goto out;

			/* Storing the length here commits the accepted answer.
			 * Immediately close TCP socket so as not to consume
			 * resources we no longer need. */
			alens[i] = alen;
			close(pfd[i].fd);
			pfd[i].fd = -1;
		}
	}
out:
	pthread_cleanup_pop(1);

	/* Disregard any incomplete TCP results */
	for (i=0; i<nqueries; i++) if (alens[i]<0) alens[i] = 0;

	return 0;
}

/**
 * 该用例未在编译构建文件内开启，需手动开启，添加到test_src_functionalext_supplement_network.gni内
 * 该用例需要两台测试设备，两台设备连接同一热点网络，本文件产物作为client端
 * client端设备同时需执行ip addr flush dev p2p0, 关闭p2p0的ipv6网络，否则当前模拟场景因为p2p0存在，
 * 无法在scopeid为0的情况下，正常发送给server端(内核将默认的0优先匹配给了p2p0)
 */

int main(void)
{
    unsigned char qbuf[2][280], abuf[2][4800];
	const unsigned char *qp[2] = { qbuf[0], qbuf[1] };
	unsigned char *ap[2] = { abuf[0], abuf[1] };
	int qlens[2], alens[2];
    int nqueries = 1;
	// timeout: 查询超时时间，attempts: 失败重试次数, nns: nameserver数量
    struct resolvconf conf = {
        .nns = 1,
        .attempts = 2,
        .ndots = 1,
        .non_public = 1
        .timeout = 5};
    conf.ns[0].family = AF_INET6;
	// scopeid设置为0，在本地网络下(fe80::***), 内核会查询对应网卡的ifindex并返回
	// 当前使用wlan0，在client端设置scopeid为0的情况下，内核返回的wlan0的ifindex为41或42(设备差异)
    conf.ns[0].scopeid = 0;
	// server设备wlan0网卡的ipv6地址，因设备与网络不同而不同，需要测试人员替换
	// wlan0网卡的ipv6地址查询方式: ifconfig -a
    char* dst = "fe80::****:****:****:****";
    if ((inet_pton(AF_INET6, dst, conf.ns[0].addr) != 1)) {
        t_error("%s invalid dns convert for ipv6\n", __func__);
        return t_status;
    }
	// client端发送query内容，非正式的dns请求格式
    char buffer[] = "test.check.platform.com";
	// server端预期answer回复
    unsigned char target[4800] = "test.check.platform.com A 111.111.11.11";
    strcpy((char*)&qbuf[0], buffer);
    qlens[0] = sizeof(buffer);
	// 使用新的nameserver check逻辑，放宽scopeid严格相等的校验，校验通过
    if (res_msend_rc_ext(0, nqueries, qp, qlens, ap, alens, sizeof *abuf, &conf, 1) < 0) {
        t_error("%s invalid dns query\n", __func__);
    }
	if (strcmp((char*)&abuf[0], (char*)&target)) {
		t_error("%s invalid answer %s, target: %s\n", __func__, abuf[0], target);
	}
	alens[0] = 0;
	memset(abuf[0], 0, sizeof *abuf);
	// 使用原本的nameserver check逻辑，scopeid校验失败
    if (res_msend_rc_ext(0, nqueries, qp, qlens, ap, alens, sizeof *abuf, &conf, 0) < 0) {
        t_error("%s invalid dns query\n", __func__);
    }
	if (!strcmp((char*)&abuf[0], (char*)&target)) {
		t_error("%s invalid answer %s, target: %s\n", __func__, abuf[0], target);
	}
    
    return t_status;
}