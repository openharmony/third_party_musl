#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include "syscall.h"

static void dummy(void) { }
weak_alias(dummy, __vm_wait);

#define UNIT SYSCALL_MMAP2_UNIT
#define OFF_MASK ((-0x2000ULL << (8*sizeof(syscall_arg_t)-1)) | (UNIT-1))

void *__mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
	long ret;
	if (off & OFF_MASK) {
		errno = EINVAL;
		return MAP_FAILED;
	}
	if (len >= PTRDIFF_MAX) {
		errno = ENOMEM;
		return MAP_FAILED;
	}
	if (len == 0) {
                errno = EINVAL;
                return MAP_FAILED;
	}
	if (flags & MAP_FIXED) {
		__vm_wait();
	}
#ifdef SYS_mmap2
	ret = __syscall(SYS_mmap2, start, len, prot, flags, fd, off/UNIT);
#else
	ret = __syscall(SYS_mmap, start, len, prot, flags, fd, off);
#endif
	/* Fixup incorrect EPERM from kernel. */
	if (ret == -EPERM && !start && (flags&MAP_ANON) && !(flags&MAP_FIXED))
		ret = -ENOMEM;
	return (void *)__syscall_ret(ret);
}


#ifdef HOOK_ENABLE
void* __libc_mmap(void*, size_t, int, int, int, off_t);
weak_alias(__mmap, __libc_mmap);
weak_alias(__libc_mmap, mmap64);
#else
weak_alias(__mmap, mmap);
weak_alias(mmap, mmap64);
#endif // HOOK_ENABLE

/* 
Adapter function for lldb to not deal with 64-bit arguments on 32-bit systems 
Issue I661Z1
Test: Build and boot devices.
*/
void *__lldb_mmap(void *start, size_t len, int prot, int flags, int fd, intptr_t off) {
	return mmap(start, len, prot, flags, fd, off);
}
