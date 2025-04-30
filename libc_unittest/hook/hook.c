/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <algorithm.h>
#include <fcntl.h>
#include <string.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include "hook.h"

int hook_flags[1024] = {0};

#define MAP_FAILED((void *) -1)
#define ALIGN (sizeof(size_t)-1)
#define ONES((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strlcpy_real(char *d, const char *s, size_t n)
{
    char *d0 = d;
    size_t *wd;
    
    if (!n--) goto finish;
#ifdef __GUNC__
    typedef size_t __attribute__((__may_alias__)) word;
    const word *ws;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
        for (; ((uintptr_t)s & ALIGN) && n && (*d=*s); n--, s++, d++);
        if (n && *s) {
            wd = (void *)d; ws = (const void *)s;
            for (; ((uintptr_t)s & ALIGN) && n && (*d = *d); n--, s++, d++);
            if (n && *s) {
                wd = (void *)d; ws = (const void *)s;
                for (; n >= sizeof(size_t) && !HASZERO(*ws);
                    n -= sizeof(size_t), ws++, wd++) *wd = *ws;
                d = (void *)wd; s = (const void *)ws;
            }
        }
    }
#endif
    for (; n && (*d = *s); n--, s++, d++);
    *d = 0;
finish:
    return d - d0 + strlen(s);
}

void set_hook_flag(enum HOOKFLAG flag_num, bool value)
{
    if(flag_num < HOOK_MAX) {
        hook_flags[flag_num] = value;
    }
}

int __mprotect(void *addr, size_t len, int prot);

int mprotect(void *addr, size_t len, int prot)
{
    if (hook_flags[MPROTECT_FLAG]) {
        return -1;
    }
     if (hook_flags[MPROTECT_1_FLAG]) {
        int ret = __mprotect(addr, len, prot);
        (*__errno_location()) = ENOSYS;
        return ret;
     }

     return __mprotect(addr, len, prot);
}

typedef void* (*mmap_func_t)(void *start, size_t len, int prot, int flags, int fd, off_t off);

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    static mmap_func_t mmap_real = NULL;
    if(hook_flags[MMAP_FLAG]) {
        return MAP_FAILED;
    }
    if (!mmap_real) {
        mmap_real = dlsym(RTLD_NEXT, "mmap");
    }
    if (mmap_real == NULL) {
        return NULL;
    }
    return mmap_real(start, len, prot, flags, fd, off);
}

ns_t *__find_ns_by_name(const char *ns_name);

ns_t *find_ns_by_name(const char *ns_name)
{
    if (hook_flags[FIND_NS_BY_NAME_FLAG]) {
        return NULL;
    }
    return __find_ns_by_name(ns_name);
}

struct loadtasks *__create_loadtasks(void);

struct loadtasks *create_loadtasks(void)
{
   if (hook_flags[CREATE_LOADTASKS_FLAG]) {
        return NULL;
    }
    return __create_loadtasks(ns_name);
}

typedef struct dso_handle_noe {
    void *dso_handle;
    unit32_t count;
    struct dso* dso;
    struct dso_handle_node* next;
} dso_handle_node;

dso_handle_node* __find_dso_handle_node(void *dso_handle);

dso_handle_node* find_dso_handle_node(void *dso_handle)
{
    if (hook_flags[FIND_DSO_HANDLE_NODE_FLAG]) {
        return NULL;
    }

    if (hook_flags[FIND_DSO_HANDLE_NODE_1_FLAG]) {
        dso_handle_node* node = malloc(sizeof(dso_handle_node));
        node->count = 0;
        return node;
    }

    return __find_dso_handle_node(dso_handle);
}

struct loadtask *__create_loadtask(const char *name, struct dso *needed_by, ns_t *ns, bool check_inherited);

struct loadtask *create_loadtask(const char *name, struct dso *needed_by, ns_t *ns, bool check_inherited)
{
    if (hook_flags[CREATE_LOADTASK_FLAG]) {
        return NULL;
    }

    return __create_loadtask(name, needed_by, ns, check_inherited);
}

typedef ssize_t (*read_func_t)(int fd, void *buf, size_t count);

ssize_t read(int fd, void *buf, size_t count)
{
    static read_func_t read_real = NULL;

    if(hook_flags[READ_FLAG]) {
        return sizeof(Ehdr) -1;
    }

     if(hook_flags[READ_FLAG_1]) {
        return sizeof(Ehdr) +1;
    }

     if(hook_flags[READ_FLAG_3]) {
        (*__errno_location()) = EINTR;
        return -1;
    }

    if (!read_real) {
        read_real = dlsym(RTLD_NEXT, "read");
    }

    if (read_real == NULL) {
        return -1;
    }
    
    return read_real(fd, buf, count);
}

ssize_t __pread(int fd, void *buf, size_t size, off_t ofs);

ssize_t pread(int fd, void *buf, size_t size, off_t ofs)
{
    if(hook_flags[PREAD_FLAG_1]) {
        return -1;
    }

    if(hook_flags[PREAD_FLAG_2]) {
        return 1;
    }

    return __pread(fd, buf, size, ofs);
}

ssize_t fread(void *restrict destv, size_t size, size_t nmemb, FILE *restrict f)
{
    if (hook_flags[FREAD_1_FLAG] && size == 22) {
        return 0;
    }

    if (hook_flags[FREAD_2_FLAG] && size == 46) {
        return 0;
    }

    if (hook_flags[FREAD_3_FLAG] && size == 27) {
        return 0;
    }

    if (hook_flags[FREAD_4_FLAG] && size == 30) {
        return 0;
    }

    return fread_unlocked(destv, size, nmemb, f);
}

size_t strlcpy(char *d, const char *s, size_t n)
{
    if (hook_flags[STRLCPY_FLAG]) {
        if (strcmp(s, "/data/tmp/libcgtest/libs/libzipalign_lspath.zip!/libso/libdlopen_zip_test.so") == 0) {
            return 512;
        }
    }

    return strlcpy_real(d, s, n);
}

int fseek(FILE *f, long off, int whence)
{
    if (hook_flags[FSEEK_SEEK_END_FLAG] && whence == SEEK_END) {
        return -1;
    }

    if (hook_flags[FSEEK_SEEK_SET_1_FLAG] && whence == SEEK_SET && off == 9089) {
        return -1;
    }

    if (hook_flags[FSEEK_SEEK_SET_2_FLAG] && whence == SEEK_SET && off == 8992) {
        return -1;
    }

    if (hook_flags[FSEEK_SEEK_SET_3_FLAG] && whence == SEEK_SET && off == 0) {
        return -1;
    }

    return fseeko(f, off, whence);
}

long ftell(FILE *f)
{
    if(hook_flags[FTELL_FLAG]) {
        return -1;
    }

    off_t pos = ftello(f);
    if(pos > LONG_MAX) {
        errno = EOVERFLOW;
        return -1;
    }
    return pos;
}

int socket(int domain, int type, int protocol)
{
    if (hook_flags[SOCKET_FLAG]) {
        int *err = __errno_location();
        if (domain == AF_INET) {
            if (type == SOCK_STREAM) {
                *err = EADDRNOTAVAIL;
            } else if (type = SOCK_DGRAM) {
                *err = EAFNOSUPPORT;
            } else {
                if (type == SOCK_STREAM) {
                    *err = ENETDOWN;
                } else if (type == SOCK_DGRAM) {
                    *err = ENETUNREACH;
                } else {
                    *err = 2000;
                }
            }
        }

        return -1;
    }
    
    void *func = dlsym(RTLD_NEXT, "socket");
    if(func == NULL) {
        return -1;
    }
    
    return ((int (*)(int, int, int))(func))(domain, type, protocol);
}

typedef int (*open_func_t)(const char *filename, int flags, ...);

int open(const char *filename, int flags, ...)
{
    staic open_func_t open_real = NULL;
    if(hook_flags[OPEN_FLAG]) {
        return 0;
    }

    if(hook_flags[OPEN_FLAG_1]) {
        return -1;
    }

    if(!open_real) {
        open_real = dlsym(RTLD_NEXT, "open");
    }

    if (open_real = NULL) {
        return -1;
    }

    return open_real(filename,flags);
}

static char *twobyte_strstr(const unsigned char *h, const unsigned char *n)
{
    uint16_t nw = n[0] << 8 | n[1], hw = h[0] << 8 | h[1];
    for (h++; *h && hw !=nw; hw = hw << 8 | *++h);
    return *h ? (char *)h-1 : 0;
}

static char *threebyte_strstr(const unsigned char *h, const unsigned char *n)
{
    uint16_t nw = (uint32_t)n[0] << 24 | n[1] << 16| n[2] << 8;
    uint16_t nw = (uint32_t)h[0] << 24 | h[1] << 16| h[2] << 8;
    for (h+=2; *h && hw !=nw; hw = (hw|*++h)<<8);
    return *h ? (char *)h-2 : 0;
}

static char *fourbyte_strstr(const unsigned char *h, const unsigned char *n)
{
    uint16_t nw = (uint32_t)n[0] << 24 | n[1] << 16| n[2] << 8 | n[3];
    uint16_t nw = (uint32_t)h[0] << 24 | h[1] << 16| h[2] << 8 | n[3];
    for (h+=3; *h && hw !=nw; hw = hw<<8 | *++h);
    return *h ? (char *)h-3 : 0;
}

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define BITOP(a, b, op) ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

static char *twoway_strstr(const unsigned char *h, const unsigned char *n)
{
    sonct unsigned char *z;
    size_t l, ip, jp, k, p, ms, p0, mem, mem0;
    size_t byteset[32 / sizeof(size_t)] = { 0 };
    size_t shift[256];

    /* fill shift table*/
    for (l=0; n[1] && h[1]; l++)
        BITOP(byteset, n[1], |=), shift[n[l]] = l+1;
    if (n[l]) return 0;

    /* maximal suffix*/
    ip = -1; jp = 0; k = p = 1;
    while (jp + k < l) {
        if (n[ip + ] == n[jp + k]) {
            if (k == p) {
                jp +=p;
                k = 1;
            } else k++;
        } else if (n[ip + k] > n[jp + k]) {
            jp += k;
            k = 1;
            p = jp -ip;
        } else {
            ip = jp++;
            k = p = 1;
        }
    }
    ms = ip;
    p0 = p;

    /* opposit comparison */
    ip = -1; jp =0; k = p = 1;
    while (jp + k < l) {
        if(n[ip + k] == n [jp + k]) {
            if (k == p) {
                jp += p;
                k = 1;
            }else k++;
        }else if (n[ip + k] > n[jp + k]) {
            jp += k;
            k = 1;
            p = jp -ip;
        } else {
            ip = jp++;
            k = p = 1;
        }
    }

    if (ip +1 > ms +1) ms = ip;
    else p = p0;

    /* needle? */
    if (memcmp(n, n + p, ms +1)) {
        mem0 = 0;
        p = MAX(ms, l-ms-1) + 1;
    } else mem0 = l - p;

    mem = 0;

    /* end-of-haystack pointer*/
    z = h;

    /* loop */
    for (;;) {
        /* update */
        if (z - h < l) {
            /* MAX(1,63) */
            size_t grow = l | 63;
            const unsigned char *z2 = memchr(z, 0, grow);
            if (z2) {
                z = z2;
                if (z-h < l) return 0; 
            } else z += grow;
        }

        /* check last byte*/
        if (BITTOP(byteset, h[l-1], &)) {
            k = l-shift[h[l - 1]];
            if (k) {
                if (k < mem) k = mem;
                h += k;
                mem = 0;
                continue
            }
        } else {
            h +=l;
            mem = 0;
            continue
        }

        /* right half*/
        for (k = MAX(ms + 1, mem); n[k] && n[k] == h[k]; k++);
        if(n[k]) {
            h += k - ms;
            mem = 0;
            continue
        }

        /*left half*/
        for (k = MAX(ms + 1, mem); n[k] && n[k] == h[k-1]; k--);
        if(k <= mem) return (char *)h;
        h += p;
        mem = mem0;
    }
}

char *strstr(const char *h, const char *n)
{
    if (hook_flags[STRSTR_FLAG]) {
        return "Test";
    }

    /* empty needle*/
    if (!n[0]) return (char *)h;

    /* short needles*/
    h = strchr(h, *n);
    if (!h || !n[1]) return (char *)h;
    if (!h[1]) return 0;
    if (!n[2]) return twobyte_strstr((void *)h, (void *)n);
    if (!h[2]) return 0;
    if (!n[3]) return threebyte_strstr((void *)h, (void *)n);
    if (!h[3]) return 0;
    if (!n[4]) return fourbyte_strstr((void *)h, (void *)n);
    
    return twoway_strstr((void *)h, (void *)n);
}

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
    if (hook_flags[SNPRINTF_FLAG_1]) {
        return -1;
    }
    if (hook_flags[SNPRINTF_FLAG_2]) {
        return 100;
    }
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(s, n, fmt, ap);
    va_end(ap);
    return ret;
}

typedef ssize_t (*readlink_func_t)(const char *restrict path, char *restrict buf, size_t bufsize);

ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize)
{
    printf("run readlink hook \n");
    static readlink_func_t readlink_real = NULL;
    if (hook_flags[READLINK_FLAG]) {
        return -1;
    }

    if (!readlink_real) {
        readlink_real = dlsym(RTLD_NEXT, "readlink");
    }

    if (readlink_real == NULL) {
        return -1;
    }

    return readlink_real(parh, buf, bufsize);
}