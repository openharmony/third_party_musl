#ifndef _MALLOC_H
#define _MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t

#include <bits/alltypes.h>

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *valloc (size_t);
void *memalign(size_t, size_t);

size_t malloc_usable_size(void *);

#define __MALLINFO_BODY \
  size_t arena; \
  size_t ordblks; \
  size_t smblks; \
  size_t hblks; \
  size_t hblkhd; \
  size_t usmblks; \
  size_t fsmblks; \
  size_t uordblks; \
  size_t fordblks; \
  size_t keepcost;

struct mallinfo { __MALLINFO_BODY };

struct mallinfo mallinfo(void);

struct mallinfo2 { __MALLINFO_BODY };

struct mallinfo2 mallinfo2(void);

#ifdef __cplusplus
}
#endif

#endif
