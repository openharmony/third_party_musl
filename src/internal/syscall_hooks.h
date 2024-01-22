#ifndef INT_SYSCALL_HOOKS_H
#define INT_SYSCALL_HOOKS_H

#include <stddef.h>

extern volatile const char *g_syscall_hooks_table __attribute__((aligned(8)));
extern volatile void *g_syscall_hooks_entry __attribute__((aligned(8)));

static inline int is_syscall_hooked(long n)
{
    return g_syscall_hooks_table != NULL && g_syscall_hooks_table[n] != 0;
}

static inline long __syscall_hooks_entry0(long n)
{
    return ((long (*)(long))g_syscall_hooks_entry)(n);
}

static inline long __syscall_hooks_entry1(long n, long a)
{
    return ((long (*)(long, long))g_syscall_hooks_entry)(n, a);
}

static inline long __syscall_hooks_entry2(long n, long a, long b)
{
    return ((long (*)(long, long, long))g_syscall_hooks_entry)(n, a, b);
}

static inline long __syscall_hooks_entry3(long n, long a, long b, long c)
{
    return ((long (*)(long, long, long, long))g_syscall_hooks_entry)(n, a, b, c);
}

static inline long __syscall_hooks_entry4(long n, long a, long b, long c, long d)
{
    return ((long (*)(long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d);
}

static inline long __syscall_hooks_entry5(long n, long a, long b, long c, long d, long e)
{
    return ((long (*)(long, long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d, e);
}

static inline long __syscall_hooks_entry6(long n, long a, long b, long c, long d, long e, long f)
{
    return ((long (*)(long, long, long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d, e, f);
}
#endif