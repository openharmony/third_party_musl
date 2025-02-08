#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <poll.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "stdio_impl.h"
#include "syscall.h"
#include "lookup.h"
#include <errno.h>

static void cleanup(void *p)
{
	struct pollfd *pfd = p;
	for (int i=0; pfd[i].fd >= -1; i++)
		if (pfd[i].fd >= 0) __syscall(SYS_close, pfd[i].fd);
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
#ifndef __LITEOS__
	if (fd < 0) {
		MUSL_LOGE("%{public}s: %{public}d: create TCP socket failed, errno id: %{public}d",
			__func__, __LINE__, errno);
	}
	/**
	 * Todo FwmarkClient::BindSocket
	*/
	if (netid > 0) {
		res_bind_socket(fd, netid);
	}
#endif
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

// equal to answer buffer size
#define BPBUF_SIZE 4800

/* Internal contract for __res_msend[_rc]: asize must be >=512, nqueries
 * must be sufficiently small to be safe as VLA size. In practice it's
 * either 1 or 2, anyway. */

int __res_msend_rc(int nqueries, const unsigned char *const *queries,
	const int *qlens, unsigned char *const *answers, int *alens, int asize,
	const struct resolvconf *conf)
{
	return res_msend_rc_ext(0, nqueries, queries, qlens, answers, alens, asize, conf, NULL);
}

int res_msend_rc_ext(int netid, int nqueries, const unsigned char *const *queries,
	const int *qlens, unsigned char *const *answers, int *alens, int asize,
	const struct resolvconf *conf, int *dns_errno)
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
	int qpos[nqueries], apos[nqueries], retry[nqueries];
	unsigned char alen_buf[nqueries][2];
	int r;
	unsigned long t0, t1, t2, temp_t;
	uint8_t nres, end_query;
	int blens[2] = {0};
	unsigned char *bp[2] = { NULL, NULL };

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
			ns[nns].sin6.sin6_port = htons(53);
			ns[nns].sin6.sin6_scope_id = iplit->scopeid;
			ns[nns].sin6.sin6_family = family = AF_INET6;
		}
	}

	/* Get local address and open/bind a socket */
	fd = socket(family, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
#ifndef __LITEOS__
	if (fd < 0) {
		MUSL_LOGE("%{public}s: %{public}d: create UDP socket failed, errno id: %{public}d",
			__func__, __LINE__, errno);
	}
#endif

	/* Handle case where system lacks IPv6 support */
	if (fd < 0 && family == AF_INET6 && errno == EAFNOSUPPORT) {
		for (i=0; i<nns && conf->ns[nns].family == AF_INET6; i++);
		if (i==nns) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: system lacks IPv6 support: %{public}d",
				__func__, __LINE__, errno);
#endif
			pthread_setcancelstate(cs, 0);
			return DNS_FAIL_REASON_LACK_V6_SUPPORT;
		}
		fd = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
		family = AF_INET;
		sl = sizeof sa.sin;
	}

#ifndef __LITEOS__
	/**
	 * Todo FwmarkClient::BindSocket
	*/
	if (netid > 0) {
		res_bind_socket(fd, netid);
	}
#endif

	/* Convert any IPv4 addresses in a mixed environment to v4-mapped */
	if (fd >= 0 && family == AF_INET6) {
		setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &(int){0}, sizeof 0);
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
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: AF_INET fd failed or bind failed, fd: %{public}d, errno: %{public}d",
			__func__, __LINE__, fd, errno);
#endif
		if (fd >= 0) close(fd);
		pthread_setcancelstate(cs, 0);
		return DNS_FAIL_REASON_CREATE_UDP_SOCKET_FAILED;
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
	temp_t = 0;
	nres = 0;
	end_query = 0;

	for (; t2-t0 < timeout; t2=mtime()) {
		/* This is the loop exit condition: that all queries
		 * have an accepted answer. */
		for (i=0; i<nqueries && alens[i]>0; i++);
		if (i==nqueries) break;

		/* if the temp_t * 2 timeout, return result immediately. */
		if (end_query) {
			goto out;
		}

		if (t2-t1 >= retry_interval) {
			/* if the first query round timeout, determine whether
			 * to return based on the num of answers. */
			if (nres) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: first round timeout and had answer", __func__, __LINE__);
#endif
				goto out;
			}
			/* Query all configured namservers in parallel */
			for (i=0; i<nqueries; i++) {
				retry[i] = 0;
				if (!alens[i]) {
					for (j=0; j<nns; j++) {
						if (sendto(fd, queries[i], qlens[i], MSG_NOSIGNAL, (void *)&ns[j], sl) == -1) {
							int errno_code = errno;
#ifndef __LITEOS__
							MUSL_LOGE("%{public}s: %{public}d: sendto failed, errno id: %{public}d",
								__func__, __LINE__, errno_code);
#endif
							if (dns_errno) {
								*dns_errno = errno_code;
							}
						}
					}
				}
			}
			t1 = t2;
			servfail_retry = 2 * nqueries;
		}

		unsigned long remaining_time = t1 + retry_interval - t2;
        if (nres) {
            if (!temp_t) {
                temp_t = t2 - t1;
            }
            if (temp_t >= retry_interval / 2 && temp_t < retry_interval) {
                remaining_time = retry_interval - temp_t;
            } else if (temp_t < retry_interval / 2 && temp_t > 0) {
                remaining_time = temp_t;
                end_query = 1;
            } else {
                goto out;
            }
        }

		/* Wait for a response, or until time to retry */
		if (poll(pfd, nqueries+1, remaining_time) <= 0) continue;

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
#ifndef __LITEOS__
				if (errno != EAGAIN) {
					MUSL_LOGE("%{public}s: %{public}d: recvmsg failed, errno id: %{public}d",
					__func__, __LINE__, errno);
				}
#endif
				break;
			}

			/* Ignore non-identifiable packets */
			if (rlen < 4) continue;

			/* Ignore replies from addresses we didn't send to */
			switch (sa.sin.sin_family) {
				// for ipv4 response, need to compare family, port and address
				case AF_INET:
					for (j = 0; j < nns; j++) {
						if (ns[j].sin.sin_family == AF_INET && ns[j].sin.sin_port == sa.sin.sin_port && (
							ns[j].sin.sin_addr.s_addr == INADDR_ANY ||
							ns[j].sin.sin_addr.s_addr == sa.sin.sin_addr.s_addr)) {
							break;
						}
					}
					break;
				// for ipv6 response, need to compare family, port and address, flowinfo and scopeid is not necessary
				case AF_INET6:
					for (j = 0; j < nns; j++) {
						if (ns[j].sin6.sin6_family == AF_INET6 &&
							ns[j].sin6.sin6_port == sa.sin6.sin6_port && (
							IN6_IS_ADDR_UNSPECIFIED(&ns[j].sin6.sin6_addr) ||
							IN6_ARE_ADDR_EQUAL(&ns[j].sin6.sin6_addr, &sa.sin6.sin6_addr))) {
							break;
						}
					}
					break;
				default:
					j = nns;
					break;
			}
			if (j==nns) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: replies from wrong addresses, ignore it", __func__, __LINE__);
#endif
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
				break;
			case 3:
				if (retry[i] + 1 < nns) {
					retry[i]++;
					continue;
				} else {
#ifndef __LITEOS__
					MUSL_LOGE("%{public}s: %{public}d: retry failed for %{public}d nameservers, and get no such name",
						__func__, __LINE__, retry[i]);
#endif
					break;
				}
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
			nres++;
			if (i == next)
				for (; next<nqueries && alens[next]; next++);
			else
				memcpy(answers[i], answers[next], rlen);

			/* If answer is truncated (TC bit), before fallback to TCP, restore the UDP answer*/
			if ((answers[i][2] & 2) || (mh.msg_flags & MSG_TRUNC)) {
				if (bp[i] == NULL) {
					bp[i] = calloc(1, sizeof(unsigned char) * BPBUF_SIZE);
					/* If fail to calloc backup buffer, only use TCP even if it fails*/
					if (bp[i] != NULL) {
						blens[i] = rlen;
						memcpy(bp[i], answers[i], rlen);
					}
				}
			}

			/* Ignore further UDP if all slots full or TCP-mode */
			if (next == nqueries) pfd[nqueries].events = 0;

			/* If answer is truncated (TC bit), fallback to TCP */
			if ((answers[i][2] & 2) || (mh.msg_flags & MSG_TRUNC)) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: fallback to TCP, msg_flags: %{public}d",
					__func__, __LINE__, mh.msg_flags);
#endif
				alens[i] = -1;
				nres--;
				if (dns_errno) {
					*dns_errno = FALLBACK_TCP_QUERY;
				}
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
			nres++;
			__syscall(SYS_close, pfd[i].fd);
			pfd[i].fd = -1;
		}
	}
out:
	pthread_cleanup_pop(1);
	/* Disregard any incomplete TCP results and try to reuse UDP */
	for (i = 0; i < nqueries; i++) {
		if (alens[i] < 0) {
			if (blens[i] != 0 && bp[i] != NULL) {
				alens[i] = blens[i];
				memcpy(answers[i], bp[i], blens[i]);
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: rollback to UDP", __func__, __LINE__);
#endif
			} else {
				alens[i] = 0;
			}
		}
		if (bp[i] != NULL) {
			free(bp[i]);
		}
	}

	return 0;
}

int __res_msend(int nqueries, const unsigned char *const *queries,
	const int *qlens, unsigned char *const *answers, int *alens, int asize)
{
	struct resolvconf conf;
	if (__get_resolv_conf(&conf, 0, 0) < 0) return -1;
	return __res_msend_rc(nqueries, queries, qlens, answers, alens, asize, &conf);
}
