#ifndef _MUSL_MALLOC_H
#define _MUSL_MALLOC_H

#include "malloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOOK_ENABLE
#define MuslMalloc(func) __libc_ ## func
#else
#define MuslMalloc(func) func
#endif

void *__libc_mmap(void*, size_t, int, int, int, off_t);
int __libc_munmap(void*, size_t);
void *__libc_malloc(size_t);
void *__libc_calloc(size_t, size_t);
void *__libc_realloc(void *, size_t);
void __libc_free(void *);
void *__libc_valloc(size_t);
void *__libc_memalign(size_t, size_t);
size_t __libc_malloc_usable_size(void *);

struct mallinfo2 __libc_mallinfo2(void);

int __libc_malloc_iterate(void* base, size_t size, void (*callback)(void* base, size_t size, void* arg), void* arg);
void __libc_malloc_disable(void);
void __libc_malloc_enable(void);

int __libc_malloc_info(int options, FILE* fp);
void __libc_malloc_stats_print(void (*write_cb) (void *, const char *), void *cbopaque, const char *opts);

int __libc_mallopt(int param, int value);
ssize_t __libc_malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count);


#ifdef __cplusplus
}
#endif

#endif
