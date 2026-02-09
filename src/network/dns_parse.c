#include <string.h>
#include "lookup.h"

#if OHOS_DNS_PROXY_BY_NETSYS
int get_ipv4_invalid_type(const void *data)
{
	if (data == NULL) {
		return IPV4_VALID_TYPE;
	}
	const unsigned char* addr = (const unsigned char *)data;
	unsigned char all_zero_addr[4] = { 0, 0, 0, 0 };
	if (memcmp(addr, all_zero_addr, 4) == 0) {
		return IPV4_INVALID_TYPE_ALLZERO;
	}

	if (addr[0] == 127) {
		return IPV4_INVALID_TYPE_LOCAL;
	}
	return IPV4_VALID_TYPE;
}
#endif

int __dns_parse(const unsigned char *r, int rlen, int (*callback)(void *, int, const void *, int, const void *, int, int), void *ctx)
{
	int qdcount, ancount;
	const unsigned char *p;
	int len;
	int ttl;

	if (rlen<12) return -1;
	if ((r[3]&15)) return 0;
	p = r+12;
	qdcount = r[4]*256 + r[5];
	ancount = r[6]*256 + r[7];
	while (qdcount--) {
		while (p-r < rlen && *p-1U < 127) p++;
		if (p>r+rlen-6)
			return -1;
		p += 5 + !!*p;
	}
	while (ancount--) {
		while (p-r < rlen && *p-1U < 127) p++;
		if (p>r+rlen-12)
			return -1;
		p += 1 + !!*p;
		len = p[8]*256 + p[9];
		if (len+10 > r+rlen-p) return -1;
		ttl = (p[4] << 24) | (p[5] << 16) | (p[6] << 8) | p[7];
		if (callback(ctx, p[1], p+10, len, r, rlen, ttl) < 0) return -1;
		p += 10 + len;
	}
	return 0;
}
