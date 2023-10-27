/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <dlfcn.h>
#include "lookup.h"

#if OHOS_DNS_PROXY_BY_NETSYS

#include "atomic.h"

#define GETADDRINFO_PRINT_DEBUG(...)

void
dns_set_addr_info_to_netsys_cache2(const int netid, const char *restrict host, const char *restrict serv,
								   const struct addrinfo *restrict hint, struct addrinfo *res)
{
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (handle == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_set_addr_info_to_netsys_cache dlopen err %s\n", dlerror());
		return;
	}

	SetCache func = dlsym(handle, OHOS_SET_CACHE_FUNC_NAME);
	if (func == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_set_addr_info_to_netsys_cache dlsym err %s\n", dlerror());
		dlclose(handle);
		return;
	}

	struct param_wrapper param = {(char *) host, (char *) serv, (struct addrinfo *) hint};
	int ret = func(netid, param, res);
	dlclose(handle);
	if (ret < 0) {
		GETADDRINFO_PRINT_DEBUG("dns_set_addr_info_to_netsys_cache OHOS_SET_CACHE_FUNC_NAME err %d\n", ret);
		return;
	}

	GETADDRINFO_PRINT_DEBUG("set to netsys cache OK\n");
}

void dns_set_addr_info_to_netsys_cache(const char *restrict host, const char *restrict serv,
									   const struct addrinfo *restrict hint, struct addrinfo *res)
{
	dns_set_addr_info_to_netsys_cache2(0, host, serv, hint, res);
}

int dns_get_addr_info_from_netsys_cache2(const int netid, const char *restrict host, const char *restrict serv,
										 const struct addrinfo *restrict hint, struct addrinfo **restrict res)
{
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (handle == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlopen err %s\n", dlerror());
		return -1;
	}

	GetCache func = dlsym(handle, OHOS_GET_CACHE_FUNC_NAME);
	if (func == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlsym err %s\n", dlerror());
		dlclose(handle);
		return -1;
	}

	struct addr_info_wrapper addr_info[MAX_RESULTS] = {0};
	uint32_t num = 0;
	struct param_wrapper param = {(char *) host, (char *) serv, (struct addrinfo *) hint};
	int ret = func(netid, param, addr_info, &num);
	dlclose(handle);

	if (ret < 0) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache OHOS_GET_CACHE_FUNC_NAME err %d\n", ret);
		return -1;
	}

	num = MACRO_MIN(num, MACRO_MIN(MAX_RESULTS, MAXADDRS));
	if (num == 0) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache num is invalid err %u", num);
		return -1;
	}

	int canon_len = (int) strlen(addr_info[0].ai_canonName);
	struct aibuf *out = calloc(1, num * sizeof(*out) + canon_len + 1);
	if (!out) {
		return -1;
	}
	char *outcanon = NULL;
	if (canon_len) {
		outcanon = (char *) &out[num];
		memcpy(outcanon, addr_info[0].ai_canonName, canon_len + 1);
	}

	for (int i = 0; i < num; i++) {
		out[i].slot = (short) i;
		out[i].ai = (struct addrinfo) {
				.ai_flags = (int) addr_info[i].ai_flags,
				.ai_family = (int) addr_info[i].ai_family,
				.ai_socktype = (int) addr_info[i].ai_sockType,
				.ai_protocol = (int) addr_info[i].ai_protocol,
				.ai_addrlen = (socklen_t) addr_info[i].ai_addrLen,
				.ai_addr = (void *) &out[i].sa,
				.ai_canonname = outcanon,
		};
		memcpy(&out[i].sa, &addr_info[i].ai_addr, addr_info[i].ai_addrLen);
		if (i > 0) {
			out[i - 1].ai.ai_next = &out[i].ai;
		}
	}

	out[0].ref = (short) num;
	*res = &out->ai;

	GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache end\n");
	return 0;
}

int dns_get_addr_info_from_netsys_cache(const char *restrict host, const char *restrict serv,
										const struct addrinfo *restrict hint, struct addrinfo **restrict res)
{
	return dns_get_addr_info_from_netsys_cache2(0, host, serv, hint, res);
}

int dns_post_result_to_netsys_cache(int netid, char* name, int usedtime, int queryret,
									struct addrinfo *res, struct queryparam *param)
{
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	GETADDRINFO_PRINT_DEBUG("dns_post_result_to_netsys_cache \n");
	if (handle == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_post_result_to_netsys_cache dlopen err %{public}s\n", dlerror());
		return -1;
	}

	PostDnsResult func = dlsym(handle, OHOS_POST_DNS_RESULT_FUNC_NAME);
	if (!func) {
		GETADDRINFO_PRINT_DEBUG("%s: loading %s failed", __func__, OHOS_POST_DNS_RESULT_FUNC_NAME);
		dlclose(handle);
		return -1;
	}

	int ret = func(netid, name, usedtime, queryret, res, param);
	dlclose(handle);
	if (ret < 0) {
		GETADDRINFO_PRINT_DEBUG("dns_set_result_to_netsys_cache OHOS_POST_DNS_RESULT_FUNC_NAME err %d\n", ret);
		return -1;
	}

	GETADDRINFO_PRINT_DEBUG("dns_post_result_to_netsys_cache OK\n");
	return 0;
}

int dns_get_default_network(int *currentnetid)
{
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (handle == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlopen err %s\n", dlerror());
		return -1;
	}

	GetDefaultNet func = dlsym(handle, OHOS_GET_DEFAULT_NET_FUNC_NAME);
	if (func == NULL) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache dlsym err %s\n", dlerror());
		dlclose(handle);
		return -1;
	}

	int ret = func(0, currentnetid);
	dlclose(handle);

	if (ret < 0) {
		GETADDRINFO_PRINT_DEBUG("dns_get_addr_info_from_netsys_cache OHOS_GET_DEFAULT_NET_FUNC_NAME err %d\n", ret);
		return -1;
	}

	GETADDRINFO_PRINT_DEBUG("dns_post_result_to_netsys_cache OK %d\n", currentnetid);
	return 0;
}

#endif
