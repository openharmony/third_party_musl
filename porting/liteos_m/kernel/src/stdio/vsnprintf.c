#include "stdio_impl.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <pthread.h>

struct cookie {
	char *s;
	size_t n;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t sn_write(FILE *f, const unsigned char *s, size_t l)
{
	struct cookie *c = f->cookie;
	size_t k = MIN(c->n, f->wpos - f->wbase);
	if (k) {
		memcpy(c->s, f->wbase, k);
		c->s += k;
		c->n -= k;
	}
	k = MIN(c->n, l);
	if (k) {
		memcpy(c->s, s, k);
		c->s += k;
		c->n -= k;
	}
	*c->s = 0;
	f->wpos = f->wbase = f->buf;
	/* pretend to succeed, even if we discarded extra data */
	return l;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	unsigned char buf[1];
	char dummy[1];
	int ret;
	struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
	pthread_mutex_t locallock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	FILE f = {
		.lbf = EOF,
		.write = sn_write,
		.lock = &locallock,
		.buf = buf,
		.cookie = &c,
	};

	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	}

	*c.s = 0;
	ret = vfprintf(&f, fmt, ap);
	pthread_mutex_destroy(&locallock);
	return ret;
}
