#ifndef _MALLOC_H
#define _MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_uintptr_t

#include <bits/alltypes.h>
#include <stdio.h>

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *valloc (size_t);
void *memalign(size_t, size_t);

size_t malloc_usable_size(void *);

struct mallinfo {
  int arena;
  int ordblks;
  int smblks;
  int hblks;
  int hblkhd;
  int usmblks;
  int fsmblks;
  int uordblks;
  int fordblks;
  int keepcost;
};

struct mallinfo mallinfo(void);

struct mallinfo2 {
  size_t arena;
  size_t ordblks;
  size_t smblks;
  size_t hblks;
  size_t hblkhd;
  size_t usmblks;
  size_t fsmblks;
  size_t uordblks;
  size_t fordblks;
  size_t keepcost;
};

struct mallinfo2 mallinfo2(void);

int malloc_iterate(void* base, size_t size, void (*callback)(void* base, size_t size, void* arg), void* arg);
void malloc_disable(void);
void malloc_enable(void);

int malloc_info(int options, FILE* fp);
void malloc_stats_print(void (*write_cb) (void *, const char *), void *cbopaque, const char *opts);

int mallopt(int param, int value);
ssize_t malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count);

#ifdef __cplusplus
}
#endif

#endif
