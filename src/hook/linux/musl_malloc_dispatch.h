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

#ifndef _MUSL_MALLOC_DISPATCH_H
#define _MUSL_MALLOC_DISPATCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void* (*MallocMmapType) (void*, size_t, int, int, int, off_t);
typedef int (*MallocMunmapType) (void*, size_t);
typedef void* (*MallocMallocType)(size_t);
typedef void* (*MallocReallocType)(void*, size_t);
typedef void* (*MallocCallocType)(size_t, size_t);
typedef void* (*MallocVallocType)(size_t);
typedef void (*MallocFreeType)(void*);
typedef void* (*MallocAlignedAllocType)(size_t, size_t);
typedef size_t (*MallocMallocUsableSizeType)(void*);
typedef int (*MallocPrctlType)(int, ...);

typedef struct mallinfo (*MallinfoType)(void);
typedef struct mallinfo2 (*Mallinfo2Type)(void);
typedef int (*MallocIterateType)(void*, size_t, void (*callback)(void*, size_t, void*), void*);
typedef void (*MallocDisableType)(void);
typedef void (*MallocEnableType)(void);
typedef int (*MallocInfoType)(int, FILE*);
typedef void (*MallocStatsPrintType)(void (*) (void *, const char *), void *, const char *);
typedef int (*MalloptType)(int, int);
typedef ssize_t (*MallocBacktraceType)(void*, uintptr_t*, size_t);
typedef void (*MemTrace)(void*, size_t, const char*, bool);

typedef bool (*GetHookFlagType)();
typedef bool (*SetHookFlagType)(bool);
typedef bool (*SendHookMiscData)(uint64_t, const char*, size_t, uint32_t);
typedef void* (*GetHookConfig)();

struct MallocDispatchType {
	MallocMmapType mmap;
	MallocMunmapType munmap;
	MallocMallocType malloc;
	MallocCallocType calloc;
	MallocReallocType realloc;
	MallocVallocType valloc;
	MallocFreeType free;
	MallocAlignedAllocType aligned_alloc;
	MallocMallocUsableSizeType malloc_usable_size;
	MallinfoType mallinfo;
	Mallinfo2Type mallinfo2;
	MallocIterateType malloc_iterate;
	MallocDisableType malloc_disable;
	MallocEnableType malloc_enable;
	MallocInfoType malloc_info;
	MallocStatsPrintType malloc_stats_print;
	MalloptType mallopt;
	MallocBacktraceType malloc_backtrace;
	GetHookFlagType get_hook_flag;
	SetHookFlagType set_hook_flag;
	MemTrace memtrace;
	MallocPrctlType prctl;
};
#ifdef __cplusplus
}
#endif

#endif
