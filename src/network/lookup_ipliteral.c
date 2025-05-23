#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lookup.h"

int __lookup_ipliteral(struct address buf[static 1], const char *name, int family)
{
	struct in_addr a4;
	struct in6_addr a6;
	if (__inet_aton(name, &a4) > 0) {
		 /* wrong family */
		if (family == AF_INET6) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: wrong family AF_INET6: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
			return EAI_NONAME;
		}
		memcpy(&buf[0].addr, &a4, sizeof a4);
		buf[0].family = AF_INET;
		buf[0].scopeid = 0;
		return 1;
	}

	char tmp[64];
	char *p = strchr(name, '%'), *z;
	unsigned long long scopeid = 0;
	if (p && p-name < 64) {
		memcpy(tmp, name, p-name);
		tmp[p-name] = 0;
		name = tmp;
	}

	if (inet_pton(AF_INET6, name, &a6) <= 0)
		return 0;
	if (family == AF_INET) /* wrong family */ {
#ifndef __LITEOS__
		MUSL_LOGE("%{public}s: %{public}d: wrong family AF_INET: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
		return EAI_NONAME;
	}

	memcpy(&buf[0].addr, &a6, sizeof a6);
	buf[0].family = AF_INET6;
	if (p) {
		if (isdigit(*++p)) scopeid = strtoull(p, &z, 10);
		else z = p-1;
		if (*z) {
			if (!IN6_IS_ADDR_LINKLOCAL(&a6) &&
			    !IN6_IS_ADDR_MC_LINKLOCAL(&a6)) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: illegal ipv6 address: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
				return EAI_NONAME;
			}
			scopeid = if_nametoindex(p);
			if (!scopeid) {
#ifndef __LITEOS__
				MUSL_LOGE("%{public}s: %{public}d: scopeid is zero: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
				return EAI_NONAME;
			}
		}
		if (scopeid > UINT_MAX) {
#ifndef __LITEOS__
			MUSL_LOGE("%{public}s: %{public}d: scopeid is over than UINT_MAX: %{public}d", __func__, __LINE__, EAI_NONAME);
#endif
			return EAI_NONAME;
		}
	}
	buf[0].scopeid = scopeid;
	return 1;
}
