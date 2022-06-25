/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "lookup.h"
#include <dlfcn.h>

#if OHOS_DNS_PROXY_BY_NETSYS

void
dns_set_addr_info_to_netsys_cache(const char *restrict host, const char *restrict serv, const struct addrinfo *restrict
hint, struct addrinfo *res) {
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (handle == NULL) {
		DNS_CONFIG_PRINT("dns_set_addr_info_to_netsys_cache dlopen err %s\n", dlerror());
		return;
	}

	SetCache func = dlsym(handle, OHOS_SET_CACHE_FUNC_NAME);
	if (func == NULL) {
		DNS_CONFIG_PRINT("dns_set_addr_info_to_netsys_cache dlsym err %s\n", dlerror());
		dlclose(handle);
		return;
	}

	struct param_wrapper param = {(char *) host, (char *) serv, (struct addrinfo *) hint};
	int ret = func(0, param, res);
	dlclose(handle);
	if (ret < 0) {
		DNS_CONFIG_PRINT("dns_set_addr_info_to_netsys_cache OHOS_SET_CACHE_FUNC_NAME err %d\n", ret);
		return;
	}

	DNS_CONFIG_PRINT("set to netsys cache OK\n");
}

int dns_get_addr_info_from_netsys_cache(const char *restrict host, const char *restrict serv,
										const struct addrinfo *restrict hint, struct addrinfo **restrict res) {
	void *handle = dlopen(DNS_SO_PATH, RTLD_LAZY);
	if (handle == NULL) {
		DNS_CONFIG_PRINT("dns_get_addr_info_from_netsys_cache dlopen err %s\n", dlerror());
		return -1;
	}

	GetCache func = dlsym(handle, OHOS_GET_CACHE_FUNC_NAME);
	if (func == NULL) {
		DNS_CONFIG_PRINT("dns_get_addr_info_from_netsys_cache dlsym err %s\n", dlerror());
		dlclose(handle);
		return -1;
	}

	struct addr_info_wrapper addr_info[MAX_RESULTS] = {0};
	uint32_t num = 0;
	struct param_wrapper param = {(char *) host, (char *) serv, (struct addrinfo *) hint};
	int ret = func(0, param, addr_info, &num);
	dlclose(handle);
	if (ret < 0) {
		DNS_CONFIG_PRINT("dns_get_addr_info_from_netsys_cache OHOS_GET_CACHE_FUNC_NAME err %d\n", ret);
		return -1;
	}

	num = MACRO_MIN(num, MACRO_MIN(MAX_RESULTS, MAXADDRS));
	if (num == 0) {
		DNS_CONFIG_PRINT("dns_get_addr_info_from_netsys_cache num is invalid err %u", num);
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

	DNS_CONFIG_PRINT("dns_get_addr_info_from_netsys_cache end\n");
	return 0;
}

#endif
