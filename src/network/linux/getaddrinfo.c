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

#define COST_FOR_MS 1000
#define COST_FOR_NANOSEC 1000000
#define DNS_QUERY_SUCCESS 0
#define DNS_QUERY_COMMOM_FAIL (-1)
#define GETADDRINFO_PRINT_DEBUG(...)
#define DNS_FAIL_REASON2_ROUND 99
#define DNS_FAIL_REASON3_ROUND 299
#define DNS_FAIL_REASON11_ROUND 99
#define GAI_KET_STR_MAX 512

typedef struct {
	char node[GAI_KET_STR_MAX];
	char service[GAI_KET_STR_MAX];
	int family;
	int socktype;
	int protocol;
	int flags;
} GaiKey;

typedef struct SharedEntry {
	GaiKey key;
	int done;
	int rc;
	struct addrinfo *head;
	int consumers;
	struct SharedEntry *next;
} SharedEntry;

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_cond = PTHREAD_COND_INITIALIZER;
static SharedEntry    *g_entries = NULL;

static void make_key(GaiKey *k, const char *node, const char *service, const struct addrinfo *hints)
{
	if (node) {
		memcpy(k->node, node, GAI_KET_STR_MAX - 1);
		k->node[GAI_KET_STR_MAX - 1] = '\0';
	} else {
		k->node[0] = '\0';
	}

	if (service) {
		memcpy(k->service, service, GAI_KET_STR_MAX - 1);
		k->service[GAI_KET_STR_MAX - 1] = '\0';
	} else {
		k->service[0] = '\0';
	}
	

	k->family   = hints ? hints->ai_family   : 0;
	k->socktype = hints ? hints->ai_socktype : 0;
	k->protocol = hints ? hints->ai_protocol : 0;
	k->flags    = hints ? hints->ai_flags    : 0;
}

static int key_equal(const GaiKey *a, const GaiKey *b)
{
	return a->family   == b->family   &&
		   a->socktype == b->socktype &&
		   a->protocol == b->protocol &&
		   a->flags    == b->flags    &&
		   strcmp(a->node, b->node) == 0 &&
		   strcmp(a->service, b->service) == 0;
}

static SharedEntry* acquire_entry(const GaiKey *key, int *i_am_leader)
{
	SharedEntry *e = NULL;
	pthread_mutex_lock(&g_lock);
	for (SharedEntry *p = g_entries;p; p = p->next) {
		if (key_equal(&p->key, key)) {
			e = p;
			break;
		}
	}
	if (!e) {
		e = (SharedEntry*)calloc(1, sizeof(SharedEntry));
		if (!e) {
			pthread_mutex_unlock(&g_lock);
			*i_am_leader = 0;
			return NULL;
		}
		e->key = *key;
		e->done = 0;
		e->rc = EAI_AGAIN;
		e->head = NULL;
		e->consumers = 1;
		e->next = g_entries;
		g_entries = e;
		*i_am_leader = 1;
	} else {
		*i_am_leader = 0;
		e->consumers++;
	}
	pthread_mutex_unlock(&g_lock);
	return e;
}

static void release_entry(SharedEntry *e)
{
	pthread_mutex_lock(&g_lock);
	e->consumers--;
	if (e->consumers == 0) {
		SharedEntry **pp = &g_entries;
		while (*pp && *pp != e) pp = &(*pp)->next;
		if (*pp == e) *pp = e->next;
		if (e->head) freeaddrinfo(e->head);
		free(e);
	}
	pthread_mutex_unlock(&g_lock);
}

static int deep_copy_ai_list(const struct addrinfo *src, struct addrinfo **dst)
{
	struct addrinfo *head = NULL, *tail = NULL;

	for (const struct addrinfo *p = src; p; p = p->ai_next) {
		struct addrinfo *n = (struct addrinfo*)calloc(1, sizeof(*n));
		if (!n) {
			freeaddrinfo(head);
			return EAI_MEMORY;
		}

		n->ai_flags    = p->ai_flags;
		n->ai_family   = p->ai_family;
		n->ai_socktype = p->ai_socktype;
		n->ai_protocol = p->ai_protocol;
		n->ai_addrlen  = p->ai_addrlen;
		n->ai_next     = NULL;

		if (p->ai_addr && p->ai_addrlen > 0) {
			n->ai_addr = malloc(p->ai_addrlen);
			if (!n->ai_addr) {
				freeaddrinfo(n);
				freeaddrinfo(head);
				return EAI_MEMORY;
			}
			memcpy(n->ai_addr, p->ai_addr, p->ai_addrlen);
		}

		if (p->ai_canonname) {
			size_t clen = strnlen(p->ai_canonname, 4096);
			char *c = (char*)malloc(clen + 1);
			if (!c) {
				if (n->ai_addr) free(n->ai_addr);
				freeaddrinfo(n);
				freeaddrinfo(head);
				return EAI_MEMORY;
			}
			memcpy(c, p->ai_canonname, clen);
			c[clen] = '\0';
			n->ai_canonname = NULL;
		} else {
			n->ai_canonname = NULL;
		}

		if (!head) head = n;
		else tail->ai_next = n;
		tail = n;
	}

	*dst = head;
	return 0;
}

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

	if (!host && !serv) return EAI_NONAME;
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
			MUSL_LOGW("bad hint ai_flag: %{public}d", flags);
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
			MUSL_LOGW("wrong family in hint: %{public}d", family);
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
				MUSL_LOGW("create socket failed for family: %{public}d, errno: %{public}d", tf[i], errno);
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
				MUSL_LOGW("connect to local address failed: %{public}d", errno);
#endif
				return EAI_SYSTEM;
			}
			if (family == tf[i]) {
#ifndef __LITEOS__
				MUSL_LOGW("family mismatch: %{public}d", EAI_NONAME);
#endif
                return EAI_NONAME;
			}
			family = tf[1 - i];
		}
	}

	GaiKey key;
	make_key(&key, host, serv, hint);
	int i_am_leader = 0;
	int rc = 0;
	struct addrinfo *shared_head = NULL;
	int t_cost = 0;
	SharedEntry *entry = acquire_entry(&key, &i_am_leader);
	if (!entry) return EAI_MEMORY;

	int timeStartRet = gettimeofday(&timeStart, NULL);
	if (i_am_leader) {
		struct service serv_array[MAXSERVS];
		nservs = __lookup_serv(serv_array, serv, proto, socktype, flags);
		if (nservs < 0) {
			pthread_mutex_lock(&g_lock);
			entry->rc = nservs;
			entry->head = NULL;
			entry->done = 1;
			pthread_cond_broadcast(&g_cond);
			pthread_mutex_unlock(&g_lock);
			release_entry(entry);
			return nservs;
		}
		struct address addr_array[MAXADDRS];
		naddrs = lookup_name_ext(addr_array, canon, host, family, flags, netid);
		int timeEndRet = gettimeofday(&timeEnd, NULL);
		if (timeStartRet == 0 && timeEndRet == 0) {
			t_cost = COST_FOR_NANOSEC * (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_usec - timeStart.tv_usec);
			t_cost /= COST_FOR_MS;
		}
		if (naddrs < 0) {
			reportdnsresult(netid, (char *)host, t_cost, naddrs, NULL, param);
#ifndef __LITEOS__
			MUSL_LOGW("reportdnsresult: %{public}d", naddrs);
#endif
			naddrs = revert_dns_fail_cause(naddrs);
			pthread_mutex_lock(&g_lock);
			entry->rc = naddrs;
			entry->head = NULL;
			entry->done = 1;
			pthread_cond_broadcast(&g_cond);
			pthread_mutex_unlock(&g_lock);
			release_entry(entry);
			return naddrs;
		}
		struct addrinfo *head = NULL, *tail = NULL;
		for (int i = 0; i < naddrs; i++) {
			for (int j = 0; j < nservs; j++) {
				struct addrinfo *n = calloc(1, sizeof(*n));
				if (!n) {
					freeaddrinfo(head);
					pthread_mutex_lock(&g_lock);
					entry->rc = EAI_MEMORY;
					entry->head = NULL;
					entry->done = 1;
					pthread_cond_broadcast(&g_cond);
					pthread_mutex_unlock(&g_lock);
					release_entry(entry);
					return EAI_MEMORY;
				}

				n->ai_family = addr_array[i].family;
				n->ai_socktype = serv_array[j].socktype;
				n->ai_protocol = serv_array[j].proto;
				n->ai_flags = hint ? hint->ai_flags : 0;

				if (addr_array[i].family == AF_INET) {
					struct sockaddr_in *sa = malloc(sizeof(*sa));
					if (!sa) { free(n); freeaddrinfo(head); rc = EAI_MEMORY; goto publish; }
					memset(sa, 0, sizeof(*sa));
					sa->sin_family = AF_INET;
					sa->sin_port = htons(serv_array[j].port);
					if (addr_array[i].addr) {
						memcpy(&sa->sin_addr, addr_array[i].addr, sizeof(sa->sin_addr));
					}
					n->ai_addr = (struct sockaddr*)sa;
					n->ai_addrlen = sizeof(*sa);
				} else if (addr_array[i].family == AF_INET6) {
					struct sockaddr_in6 *sa6 = malloc(sizeof(*sa6));
					if (!sa6) { free(n); freeaddrinfo(head); rc = EAI_MEMORY; goto publish; }
					memset(sa6, 0, sizeof(*sa6));
					sa6->sin6_family = AF_INET6;
					sa6->sin6_port = htons(serv_array[j].port);
					sa6->sin6_scope_id = addr_array[i].scopeid;
					if (addr_array[i].addr) {
						memcpy(&sa6->sin6_addr, addr_array[i].addr, sizeof(sa6->sin6_addr));
					}
					n->ai_addr = (struct sockaddr*)sa6;
					n->ai_addrlen = sizeof(*sa6);
				}

				if (!head) head = n;
				else tail->ai_next = n;
				tail = n;
			}
		}

		shared_head = head;
		rc = (shared_head ? 0 : EAI_FAIL);

publish:
		pthread_mutex_lock(&g_lock);
		entry->rc = rc;
		entry->head = (rc == 0) ? shared_head : NULL;
		entry->done = 1;
		pthread_cond_broadcast(&g_cond);
		pthread_mutex_unlock(&g_lock);
	} else {
		pthread_mutex_lock(&g_lock);
		while (entry->done == 0) {
			pthread_cond_wait(&g_cond, &g_lock);
		}
		rc = entry->rc;
		shared_head = entry->head;
		pthread_mutex_unlock(&g_lock);
	}

	if (rc != 0) {
		release_entry(entry);
		return rc;
	}

	int cprc = deep_copy_ai_list(shared_head, res);
	release_entry(entry);

	int timeEndRet = gettimeofday(&timeEnd, NULL);
	if (timeStartRet == 0 && timeEndRet == 0) {
		t_cost = COST_FOR_NANOSEC * (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_usec - timeStart.tv_usec);
		t_cost /= COST_FOR_MS;
	}
	reportdnsresult(netid, (char *)host, t_cost, DNS_QUERY_SUCCESS, *res, param);
	int cnt = predefined_host_is_contain_host(host);
#if OHOS_DNS_PROXY_BY_NETSYS
	if (type == QEURY_TYPE_NORMAL && cnt == 0) {
		dns_set_addr_info_to_netsys_cache2(netid, host, serv, hint, *res);
	}
#endif
	return cprc;


// 	nservs = __lookup_serv(ports, serv, proto, socktype, flags);
// 	if (nservs < 0) return nservs;

// 	naddrs = lookup_name_ext(addrs, canon, host, family, flags, netid);
// 	int timeEndRet = gettimeofday(&timeEnd, NULL);
// 	int t_cost = 0;
// 	if (timeStartRet == 0 && timeEndRet == 0) {
// 		t_cost = COST_FOR_NANOSEC * (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_usec - timeStart.tv_usec);
// 		t_cost /= COST_FOR_MS;
// 	}
// 	if (naddrs < 0) {
// 		reportdnsresult(netid, (char *)host, t_cost, naddrs, NULL, param);
// #ifndef __LITEOS__
// 		MUSL_LOGW("reportdnsresult: %{public}d", naddrs);
// #endif
// 		naddrs = revert_dns_fail_cause(naddrs);
// 		return naddrs;
// 	}

// 	nais = nservs * naddrs;
// 	canon_len = strlen(canon);
// 	out = calloc(1, nais * sizeof(*out) + canon_len + 1);
// 	if (!out) return EAI_MEMORY;

// 	if (canon_len) {
// 		outcanon = (void *)&out[nais];
// 		memcpy(outcanon, canon, canon_len + 1);
// 	} else {
// 		outcanon = 0;
// 	}

// 	for (k = i = 0; i < naddrs; i++) for (j = 0; j < nservs; j++, k++) {
// 		out[k].slot = k;
// 		out[k].ai = (struct addrinfo) {
// 			.ai_family = addrs[i].family,
// 			.ai_socktype = ports[j].socktype,
// 			.ai_protocol = ports[j].proto,
// 			.ai_addrlen = addrs[i].family == AF_INET
// 				? sizeof(struct sockaddr_in)
// 				: sizeof(struct sockaddr_in6),
// 			.ai_addr = (void *)&out[k].sa,
// 			.ai_canonname = outcanon };
// 		if (k) out[k-1].ai.ai_next = &out[k].ai;
// 		switch (addrs[i].family) {
// 		case AF_INET:
// 			out[k].sa.sin.sin_family = AF_INET;
// 			out[k].sa.sin.sin_port = htons(ports[j].port);
// 			memcpy(&out[k].sa.sin.sin_addr, &addrs[i].addr, 4);
// 			break;
// 		case AF_INET6:
// 			out[k].sa.sin6.sin6_family = AF_INET6;
// 			out[k].sa.sin6.sin6_port = htons(ports[j].port);
// 			out[k].sa.sin6.sin6_scope_id = addrs[i].scopeid;
// 			memcpy(&out[k].sa.sin6.sin6_addr, &addrs[i].addr, 16);
// 			break;
// 		}
// 	}
// 	out[0].ref = nais;
// 	*res = &out->ai;

// 	reportdnsresult(netid, (char *)host, t_cost, DNS_QUERY_SUCCESS, *res, param);
// 	int cnt = predefined_host_is_contain_host(host);
// #if OHOS_DNS_PROXY_BY_NETSYS
// 	if (type == QEURY_TYPE_NORMAL && cnt == 0) {
// 		dns_set_addr_info_to_netsys_cache2(netid, host, serv, hint, *res);
// 	}
// #endif
// 	return 0;
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
