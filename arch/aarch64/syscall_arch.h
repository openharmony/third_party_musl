#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
#include "syscall_hooks.h"
#endif

#define __SYSCALL_LL_E(x) (x)
#define __SYSCALL_LL_O(x) (x)

#define __asm_syscall(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
	return x0; \
	} while (0)

static inline long __syscall0(long n)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry0(n);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0");
	__asm_syscall("r"(x8));
}

static inline long __syscall1(long n, long a)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry1(n, a);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	__asm_syscall("r"(x8), "0"(x0));
}

static inline long __syscall2(long n, long a, long b)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry2(n, a, b);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1));
}

static inline long __syscall3(long n, long a, long b, long c)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry3(n, a, b, c);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2));
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry4(n, a, b, c, d);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
}

static inline long __syscall5(long n, long a, long b, long c, long d, long e)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry5(n, a, b, c, d, e);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	register long x4 __asm__("x4") = e;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4));
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	if (is_syscall_hooked(n)) {
		return __syscall_hooks_entry6(n, a, b, c, d, e, f);
	}
#endif
	register long x8 __asm__("x8") = n;
	register long x0 __asm__("x0") = a;
	register long x1 __asm__("x1") = b;
	register long x2 __asm__("x2") = c;
	register long x3 __asm__("x3") = d;
	register long x4 __asm__("x4") = e;
	register long x5 __asm__("x5") = f;
	__asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5));
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__kernel_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6.39"
#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
#define VDSO_CGR_SYM "__kernel_clock_getres"
#define VDSO_CGR_VER "LINUX_2.6.39"
#define VDSO_GTD_SYM "__kernel_gettimeofday"
#define VDSO_GTD_VER "LINUX_2.6.39"
#endif

#define IPC_64 0
