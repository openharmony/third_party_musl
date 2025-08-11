#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include "lookup.h"
#include "lock.h"

#define COST_FOR_MS 1000
#define COST_FOR_NANOSEC 1000000
#define DNS_QUERY_SUCCESS 0
#define DNS_QUERY_COMMOM_FAIL (-1)
#define GETADDRINFO_PRINT_DEBUG(...)
#define DNS_FAIL_REASON2_ROUND 99
#define DNS_FAIL_REASON3_ROUND 299
#define DNS_FAIL_REASON11_ROUND 99

#if OHOS_DNS_PROXY_BY_NETSYS
#define CACHE_VALID_TIME 2000
#define CACHE_INFLIGHT_TIMEOUT 4000
struct aicache {
	const char *host;
	const char *serv;
	struct aibuf *res;
	struct addrinfo hint;
	int ressize;
	int netid;
};

typedef struct aicachelist {
	struct aicache ai;
    /* Lock the node that the first thread enters when multi threads are looking up the same host */
	volatile int lock[1];
	unsigned long obtainingtime;
	unsigned long starttime;
	struct aicachelist *next;
	struct aicachelist *prev;
} aicachelist;

static aicachelist g_dnscachelist;
/* Lock the global cache each time it is used. */
volatile int g_dnscachelock[1];

static unsigned long mtime(void)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0 && errno == ENOSYS) {
		clock_gettime(CLOCK_REALTIME, &ts);
	}
	return (unsigned long)ts.tv_sec * COST_FOR_MS + ts.tv_nsec / COST_FOR_NANOSEC;
}

void free_host(aicachelist* node)
{
	if (node == NULL) {
		return;
	}

	if (node->ai.host != NULL) {
		free((char *)node->ai.host);
	}

	if (node->ai.serv != NULL) {
		free((char *)node->ai.serv);
	}

	if (node->ai.res != NULL) {
		free(node->ai.res);
	}
	free(node);
}

aicachelist *alloc_host(const char *restrict host, const char *restrict serv,
						const struct addrinfo *restrict hint, int netid)
{
	aicachelist *node = (aicachelist *)calloc(1, sizeof(aicachelist));
	if (node == NULL) {
		return NULL;
	}
	if (host != NULL) {
		node->ai.host = strdup(host);
        if (node->ai.host == NULL) {
            free(node);
            return NULL;
        }
	}
	if (serv != NULL) {
		node->ai.serv = strdup(serv);
        if (node->ai.serv == NULL) {
            free((char *)node->ai.host);
            free(node);
            return NULL;
        }
	}
	if (hint != NULL) {
		memcpy(&node->ai.hint, hint, sizeof(struct addrinfo));
	}
	node->ai.netid = netid;
	node->obtainingtime = 0;
    node->starttime = 0;
	return node;
}

void remove_host(aicachelist *node)
{
	if (node == NULL) {
		return;
	}
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
}

struct aibuf *copy_aibuf(struct aibuf *src, int size)
{
	if (src == NULL || size == 0) {
		return NULL;
	}

	struct aibuf *des = calloc(1, size);
	if (des == NULL) {
		return NULL;
	}

	memcpy(des, src, size);

	int nais = des[0].ref;
	if (nais > 0) {
		for (int i = 0; i < nais - 1; i++) {
			des[i].ai.ai_next = &des[i + 1].ai;
		}
	}

	return des;
}

void refresh_cache(void)
{
	LOCK(g_dnscachelock);
	if (g_dnscachelist.next == NULL) {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
		UNLOCK(g_dnscachelock);
		return;
	}
	aicachelist *node = NULL;
	unsigned long currenttime = mtime();
	for (node = g_dnscachelist.next; node != NULL; node = node->next) {
		if (node->obtainingtime == 0) {
            if (node->starttime != 0 && currenttime - node->starttime > CACHE_INFLIGHT_TIMEOUT) {
#ifndef __LITEOS__
                MUSL_LOGE("%{public}s: release inflight timeout node, line %d", __func__, __LINE__);
#endif
                node->ai.ressize = 0;
                node->ai.res = NULL;
                node->obtainingtime = currenttime;
                UNLOCK(node->lock);
            }
			continue;
		}
		if (currenttime - node->obtainingtime > CACHE_VALID_TIME) {
			aicachelist *tmp = node->prev;
			remove_host(node);
			free_host(node);
			node = tmp;
		}
	}
	UNLOCK(g_dnscachelock);
}

int compare_hint(aicachelist *node, const struct addrinfo *restrict hint)
{
	if (!hint) {
		if (node->ai.hint.ai_flags == 0 && node->ai.hint.ai_family == 0 &&
			node->ai.hint.ai_socktype == 0 && node->ai.hint.ai_protocol == 0) {
			return 0;
		} else {
			return 1;
		}
	} else {
		if (hint->ai_flags != node->ai.hint.ai_flags || hint->ai_family != node->ai.hint.ai_family ||
			hint->ai_socktype != node->ai.hint.ai_socktype || hint->ai_protocol != node->ai.hint.ai_protocol) {
			return 1;
		} else {
			return 0;
		}
	}
	return 0;
}

int compare_serv(const char *l, const char *r)
{
	if (l == NULL && r == NULL) {
		return 0;
	}
	if (l != NULL && r != NULL && strcmp(l, r) == 0) {
		return 0;
	}
	return 1;
}

aicachelist *find_host(const char *restrict host,
	const char *restrict serv, const struct addrinfo *restrict hint, int netid, int needlock)
{
	if (host == NULL) {
		return NULL;
	}
	aicachelist *node = NULL;
	LOCK(g_dnscachelock);
	if (g_dnscachelist.next == NULL) {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
		UNLOCK(g_dnscachelock);
		return NULL;
	}
	for (node = g_dnscachelist.next; node != NULL; node = node->next) {
		if (netid != node->ai.netid || compare_serv(host, node->ai.host) != 0 ||
			compare_serv(serv, node->ai.serv) != 0 || compare_hint(node, hint) != 0 ||
			(node->ai.res == NULL && node->obtainingtime != 0)) {
			continue;
		}
		UNLOCK(g_dnscachelock);

		if (!needlock) {
			return node;
		}

        unsigned long start = mtime();
        while (node->obtainingtime == 0) {
            if (mtime() - start > CACHE_INFLIGHT_TIMEOUT) {
#ifndef __LITEOS__
                MUSL_LOGE("%{public}s: %{public}d wait inflight timeout", __func__, __LINE__);
#endif
                return NULL;
            }
            struct timespec ts;
			ts.tv_sec = 0;
			ts.tv_nsec = 1000000; // 1 ms
			nanosleep(&ts, NULL);
        }

		LOCK(node->lock);
		if (node->ai.res == NULL) {
			UNLOCK(node->lock);
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
			return NULL;
		}
		UNLOCK(node->lock);
		return node;
	}
	UNLOCK(g_dnscachelock);
	return NULL;
}

int query_addr_info_from_local_cache(const char *restrict host, const char *restrict serv,
	const struct addrinfo *restrict hint, struct addrinfo **restrict res, int netid)
{
	refresh_cache();

	aicachelist *node = find_host(host, serv, hint, netid, true);
	if (node != NULL) {
#ifndef __LITEOS__
		MUSL_LOGI("%{public}s: %{public}d, found from cache", __func__, __LINE__);
#endif
		struct aibuf *out = copy_aibuf(node->ai.res, node->ai.ressize);
		if (out != NULL) {
			*res = &out->ai;
			return 1;
		}
	}
	node = alloc_host(host, serv, hint, netid);
	if (node == NULL) {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
		return 0;
	}

	LOCK(g_dnscachelock);
	LOCK(node->lock);
	node->prev = &g_dnscachelist;
	if (g_dnscachelist.next != NULL) {
		g_dnscachelist.next->prev = node;
	}
	g_dnscachelist.next = node;
	UNLOCK(g_dnscachelock);
	node->starttime = mtime();
	return 0;
}

void update_addr_info_cache(const char *restrict host,
	const char *restrict serv, const struct addrinfo *restrict hint, struct aibuf *res, int size, int netid)
{
	aicachelist *node = find_host(host, serv, hint, netid, false);
	if (node == NULL) {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
		return;
	}
	if (res == NULL) {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d", __func__, __LINE__);
#endif
		node->ai.ressize = 0;
		node->ai.res = NULL;
		node->obtainingtime = mtime();
		UNLOCK(node->lock);
		return;
	}
	node->ai.ressize = 0;
	node->ai.res = copy_aibuf(res, size);
	if (node->ai.res != NULL) {
		node->ai.ressize = size;
	}
	node->obtainingtime = mtime();
	UNLOCK(node->lock);
}
#endif

int reportdnsresult(int netid, char* name, int usedtime, int queryret, struct addrinfo *res, struct queryparam *param)
{
#if OHOS_DNS_PROXY_BY_NETSYS
	if (dns_post_result_to_netsys_cache(netid, name, usedtime, queryret, res, param) == 0) {
		GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext reportdnsresult fail\n");
	}
#endif
	return 0;
}

static custom_dns_resolver g_customdnsresolvehook;
static pthread_key_t g_recursiveKey;
static int* g_recursive;

int setdnsresolvehook(custom_dns_resolver hookfunc)
{
	int ret = -1;
	if (g_customdnsresolvehook) {
		return ret;
	}
	if (hookfunc) {
		g_customdnsresolvehook = hookfunc;
		pthread_key_create(&g_recursiveKey, NULL);
		ret = 0;
	}
	return ret;
}

int removednsresolvehook()
{
	g_customdnsresolvehook = NULL;
	if (g_recursive) {
		free(g_recursive);
		g_recursive = NULL;
	}
	if (g_recursiveKey) {
		pthread_key_delete(g_recursiveKey);
		g_recursiveKey = NULL;
	}
	return 0;
}

int getaddrinfo_hook(const char* host, const char* serv, const struct addrinfo* hints,
    struct addrinfo** res)
{
    if (g_customdnsresolvehook) {
        int ret = g_customdnsresolvehook(host, serv, hints, res);
        if (ret == 0) {
            return ret;
        }
    }
    return predefined_host_lookup_ip(host, serv, hints, res);
}

int getaddrinfo(const char *restrict host, const char *restrict serv, const struct addrinfo *restrict hint, struct addrinfo **restrict res)
{
	struct queryparam param = {0, 0, 0, 0, NULL};
	return getaddrinfo_ext(host, serv, hint, res, &param);
}

int getaddrinfo_ext(const char *restrict host, const char *restrict serv, const struct addrinfo *restrict hint,
					struct addrinfo **restrict res, struct queryparam *restrict param)
{
	int netid = 0;
	int type = 0;
	struct timeval timeStart, timeEnd;

	if (!host && !serv) {
#if OHOS_DNS_PROXY_BY_NETSYS
		update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
		return EAI_NONAME;
	}
	if (!param) {
		netid = 0;
		type = 0;
	} else {
		netid = param->qp_netid;
		type = param->qp_type;
	}

	if (g_customdnsresolvehook) {
		g_recursive = pthread_getspecific(g_recursiveKey);
		if (g_recursive == NULL) {
			int *newRecursive = malloc(sizeof(int));
			*newRecursive = 0;
			pthread_setspecific(g_recursiveKey, newRecursive);
			g_recursive = newRecursive;
		}
		if (*g_recursive == 0) {
			++(*g_recursive);
			int ret = g_customdnsresolvehook(host, serv, hint, res);
			--(*g_recursive);
			return ret;
		}
	}

#if OHOS_DNS_PROXY_BY_NETSYS
	GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext netid:%{public}d type:%{public}d \n", netid, type);
	if (type == QEURY_TYPE_NORMAL && predefined_host_is_contain_host(host) == 0) {
		if (dns_get_addr_info_from_netsys_cache2(netid, host, serv, hint, res) == 0) {
			GETADDRINFO_PRINT_DEBUG("getaddrinfo_ext get from netsys cache OK\n");
			reportdnsresult(netid, (char *)host, 0, DNS_QUERY_SUCCESS, *res, param);
			return 0;
		}
	}
#endif

	struct service ports[MAXSERVS];
	struct address addrs[MAXADDRS];
	char canon[256], *outcanon;
	int nservs, naddrs, nais, canon_len, i, j, k;
	int family = AF_UNSPEC, flags = 0, proto = 0, socktype = 0;
	struct aibuf *out;

	if (hint) {
		family = hint->ai_family;
		flags = hint->ai_flags;
		proto = hint->ai_protocol;
		socktype = hint->ai_socktype;

		const int mask = AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST |
			AI_V4MAPPED | AI_ALL | AI_ADDRCONFIG | AI_NUMERICSERV;
		if ((flags & mask) != flags) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: bad hint ai_flag: %{public}d", __func__, __LINE__, flags);
#endif
#if OHOS_DNS_PROXY_BY_NETSYS
			update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
			return EAI_BADFLAGS;
		}

		switch (family) {
		case AF_INET:
		case AF_INET6:
		case AF_UNSPEC:
			break;
		default:
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: wrong family in hint: %{public}d", __func__, __LINE__, family);
#endif
#if OHOS_DNS_PROXY_BY_NETSYS
			update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
			return EAI_FAMILY;
		}
	}

	if (flags & AI_ADDRCONFIG) {
		/* Define the "an address is configured" condition for address
		 * families via ability to create a socket for the family plus
		 * routability of the loopback address for the family. */
		static const struct sockaddr_in lo4 = {
			.sin_family = AF_INET, .sin_port = 65535,
			.sin_addr.s_addr = __BYTE_ORDER == __BIG_ENDIAN
				? 0x7f000001 : 0x0100007f
		};
		static const struct sockaddr_in6 lo6 = {
			.sin6_family = AF_INET6, .sin6_port = 65535,
			.sin6_addr = IN6ADDR_LOOPBACK_INIT
		};
		int tf[2] = { AF_INET, AF_INET6 };
		const void *ta[2] = { &lo4, &lo6 };
		socklen_t tl[2] = { sizeof lo4, sizeof lo6 };
		for (i = 0; i < 2; i++) {
			if (family == tf[1 - i]) continue;
			int s = socket(tf[i], SOCK_CLOEXEC | SOCK_DGRAM,
				IPPROTO_UDP);
#ifndef __LITEOS__
			if (s < 0) {
				MUSL_LOGE("%{public}s: %{public}d: create socket failed for family: %{public}d, errno: %{public}d",
					__func__, __LINE__, tf[i], errno);
			}
#endif
			if (s >= 0) {
				int cs;
				pthread_setcancelstate(
					PTHREAD_CANCEL_DISABLE, &cs);
				int r = connect(s, ta[i], tl[i]);
				int saved_errno = errno;
				pthread_setcancelstate(cs, 0);
				close(s);
				if (!r) continue;
				errno = saved_errno;
			}
			switch (errno) {
			case EADDRNOTAVAIL:
			case EAFNOSUPPORT:
			case EHOSTUNREACH:
			case ENETDOWN:
			case ENETUNREACH:
				break;
			default:
#ifndef __LITEOS__
				MUSL_LOGW("%{public}s: %{public}d: connect to local address failed: %{public}d",
					__func__, __LINE__, errno);
#endif
#if OHOS_DNS_PROXY_BY_NETSYS
				update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
				return EAI_SYSTEM;
			}
			if (family == tf[i]) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: family mismatch: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
#if OHOS_DNS_PROXY_BY_NETSYS
				update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
                return EAI_NONAME;
			}
			family = tf[1 - i];
		}
	}

#if OHOS_DNS_PROXY_BY_NETSYS
	if (query_addr_info_from_local_cache(host, serv, hint, res, netid) == 1) {
		reportdnsresult(netid, (char *)host, 0, DNS_QUERY_SUCCESS, *res, param);
		return 0;
	}
#endif

	int timeStartRet = gettimeofday(&timeStart, NULL);
	nservs = __lookup_serv(ports, serv, proto, socktype, flags);
	if (nservs < 0) {
#if OHOS_DNS_PROXY_BY_NETSYS
		update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
		return nservs;
	}

	naddrs = lookup_name_ext(addrs, canon, host, family, flags, netid);
	int timeEndRet = gettimeofday(&timeEnd, NULL);
	int t_cost = 0;
	if (timeStartRet == 0 && timeEndRet == 0) {
		t_cost = COST_FOR_NANOSEC * (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_usec - timeStart.tv_usec);
		t_cost /= COST_FOR_MS;
	}
	if (naddrs < 0) {
		reportdnsresult(netid, (char *)host, t_cost, naddrs, NULL, param);
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: reportdnsresult: %{public}d",
			__func__, __LINE__, naddrs);
#endif
#if OHOS_DNS_PROXY_BY_NETSYS
		update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
		naddrs = revert_dns_fail_cause(naddrs);
		return naddrs;
	}

	nais = nservs * naddrs;
	canon_len = strlen(canon);
	int totalsize = nais * sizeof(*out) + canon_len + 1;
	out = calloc(1, nais * sizeof(*out) + canon_len + 1);
	if (!out) {
#if OHOS_DNS_PROXY_BY_NETSYS
		update_addr_info_cache(host, serv, hint, NULL, 0, netid);
#endif
		return EAI_MEMORY;
	}

	if (canon_len) {
		outcanon = (void *)&out[nais];
		memcpy(outcanon, canon, canon_len + 1);
	} else {
		outcanon = 0;
	}

	for (k = i = 0; i < naddrs; i++) for (j = 0; j < nservs; j++, k++) {
		out[k].slot = k;
		out[k].ai = (struct addrinfo) {
			.ai_family = addrs[i].family,
			.ai_socktype = ports[j].socktype,
			.ai_protocol = ports[j].proto,
			.ai_addrlen = addrs[i].family == AF_INET
				? sizeof(struct sockaddr_in)
				: sizeof(struct sockaddr_in6),
			.ai_addr = (void *)&out[k].sa,
			.ai_canonname = outcanon };
		if (k) out[k-1].ai.ai_next = &out[k].ai;
		switch (addrs[i].family) {
		case AF_INET:
			out[k].sa.sin.sin_family = AF_INET;
			out[k].sa.sin.sin_port = htons(ports[j].port);
			memcpy(&out[k].sa.sin.sin_addr, &addrs[i].addr, 4);
			break;
		case AF_INET6:
			out[k].sa.sin6.sin6_family = AF_INET6;
			out[k].sa.sin6.sin6_port = htons(ports[j].port);
			out[k].sa.sin6.sin6_scope_id = addrs[i].scopeid;
			memcpy(&out[k].sa.sin6.sin6_addr, &addrs[i].addr, 16);
			break;
		}
	}
	out[0].ref = nais;
	*res = &out->ai;

	reportdnsresult(netid, (char *)host, t_cost, DNS_QUERY_SUCCESS, *res, param);
	int cnt = predefined_host_is_contain_host(host);
#if OHOS_DNS_PROXY_BY_NETSYS
	update_addr_info_cache(host, serv, hint, out, totalsize, netid);
	if (type == QEURY_TYPE_NORMAL && cnt == 0) {
		dns_set_addr_info_to_netsys_cache2(netid, host, serv, hint, *res);
	}
#endif
	return 0;
}

hidden int revert_dns_fail_cause(int cause)
{
	if (cause <= DNS_FAIL_REASON_PARAM_INVALID && cause > DNS_FAIL_REASON_PARAM_INVALID - DNS_FAIL_REASON2_ROUND) {
		return EAI_NONAME;
	}
	if (cause <= DNS_FAIL_REASON_ROUTE_CONFIG_ERR && cause > DNS_FAIL_REASON_ROUTE_CONFIG_ERR -
		DNS_FAIL_REASON3_ROUND) {
		return EAI_AGAIN;
	}
	if (cause <= DNS_FAIL_REASON_LACK_V6_SUPPORT && cause > DNS_FAIL_REASON_LACK_V6_SUPPORT -
		DNS_FAIL_REASON2_ROUND) {
		return EAI_SYSTEM;
	}
	return cause;
}
