/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdarg.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "pthread_impl.h"
#include "malloc_impl.h"

#ifdef MUSL_ITERATE_AND_STATS_API
#define STAT_PRINTF_MAX_LEN 255
#define ALLOCATOR_VERSION 1
#define SEPARATOR_REPEATS 7

typedef void (write_cb_fun)(void *, const char *);

typedef enum {
	TABLE, XML
} print_mode;

typedef struct {
	size_t mmapped_regions;
	size_t total_mmapped_memory;
	size_t total_allocated_memory;
	size_t total_allocated_heap_space;
} malloc_stats_t;

static void stat_printf(write_cb_fun *write_cb, void *write_cb_arg, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buf[STAT_PRINTF_MAX_LEN + 1];
	if (vsnprintf(buf, STAT_PRINTF_MAX_LEN, fmt, args)) {
		if (write_cb != NULL) {
			write_cb(write_cb_arg, buf);
		} else {
			printf(buf);
		}
	} else {
		fprintf(stderr, "Error writing to buffer");
	}
	va_end(args);
}

static void print_thread_stats_table(
	write_cb_fun *write_cb,
	void *write_cb_arg,
	int tid,
	malloc_stats_t *stats
)
{
	stat_printf(
		write_cb,
		write_cb_arg,
		"%-11d %-23zu %-20zu %-20zu\n",
		tid,
		stats->total_allocated_memory,
		stats->total_mmapped_memory,
		stats->mmapped_regions
	);
}

static void print_amount_xml(write_cb_fun *write_cb, void *write_cb_arg, const char *name, size_t value)
{
	stat_printf(write_cb, write_cb_arg, "<%s>%zu</%s>\n", name, value, name);
}

static void print_thread_specific_amounts_xml(write_cb_fun *write_cb, void *write_cb_arg, malloc_stats_t *stats)
{
	print_amount_xml(write_cb, write_cb_arg, "total_allocated_memory", stats->total_allocated_memory);
	print_amount_xml(write_cb, write_cb_arg, "total_mmapped_memory", stats->total_mmapped_memory);
	print_amount_xml(write_cb, write_cb_arg, "mmapped_regions", stats->mmapped_regions);
}

static void print_thread_stats_xml(
	write_cb_fun *write_cb,
	void *write_cb_arg,
	int tid,
	malloc_stats_t *stats
)
{
	stat_printf(write_cb, write_cb_arg, "<thread id=\"%d\">\n", tid);
	print_thread_specific_amounts_xml(write_cb, write_cb_arg, stats);
	stat_printf(write_cb, write_cb_arg, "</thread>\n");
}

static malloc_stats_t add_up_chunks(occupied_bin_t *occupied_bin)
{
	malloc_stats_t stats = {0, 0, 0, 0};
	for (struct chunk *c = occupied_bin->head; c != NULL; c = c->next_occupied) {
		size_t chunk_memory = CHUNK_SIZE(c) - OVERHEAD;
		stats.total_allocated_memory += chunk_memory;
		if (IS_MMAPPED(c)) {
			stats.mmapped_regions++;
			stats.total_mmapped_memory += chunk_memory;
		} else {
			stats.total_allocated_heap_space += chunk_memory;
		}
	}
	return stats;
}

static malloc_stats_t add_up_chunks_by_threads(occupied_bin_t *occupied_bin, int tid)
{
	malloc_stats_t stats = {0, 0, 0, 0};
	for (struct chunk *c = occupied_bin->head; c != NULL; c = c->next_occupied) {
		if (c->thread_id == tid) {
			size_t chunk_memory = CHUNK_SIZE(c) - OVERHEAD;
			stats.total_allocated_memory += chunk_memory;
			if (IS_MMAPPED(c)) {
				stats.mmapped_regions++;
				stats.total_mmapped_memory += chunk_memory;
			} else {
				stats.total_allocated_heap_space += chunk_memory;
			}
		}
	}
	return stats;
}

static size_t print_threads(write_cb_fun *write_cb, void *write_cb_arg, print_mode mode)
{
	size_t total_allocated_heap_space = 0;

	for (size_t i = 0; i < OCCUPIED_BIN_COUNT; ++i) {
		occupied_bin_t *occupied_bin = __get_occupied_bin_by_idx(i);
		int min_id = 0;
		int found;
		do {
			found = 0;
			for (struct chunk *c = occupied_bin->head; c != NULL; c = c->next_occupied) {
				if (c->thread_id > min_id) {
					min_id = c->thread_id;
					found = 1;
				}
			}
			if (found) {
				malloc_stats_t stats = add_up_chunks_by_threads(occupied_bin, min_id);
				total_allocated_heap_space += stats.total_allocated_heap_space;

				if (mode == TABLE) {
					print_thread_stats_table(write_cb, write_cb_arg, min_id, &stats);
				} else {
					print_thread_stats_xml(write_cb, write_cb_arg, min_id, &stats);
				}
			}
		} while (found);
	}

	return total_allocated_heap_space;
}

static void print_total_free_heap_space(
	write_cb_fun *write_cb,
	void *write_cb_arg,
	size_t total_allocated_heap_space,
	print_mode mode
)
{
	if (mode == TABLE) {
		stat_printf(write_cb, write_cb_arg, "\n");
		for (size_t i = 0; i < SEPARATOR_REPEATS; i++) {
			stat_printf(
				write_cb,
				write_cb_arg,
				"-----------"
			);
		}
		stat_printf(
			write_cb,
			write_cb_arg,
			"\ntotal free heap space: %zu\n",
			__get_total_heap_space() - total_allocated_heap_space
		);
	} else {
		print_amount_xml(
			write_cb,
			write_cb_arg,
			"total_free_heap_space",
			__get_total_heap_space() - total_allocated_heap_space
		);
	}
}

static void print_to_file(void *fp, const char *s)
{
	if (fputs(s, fp) == EOF) {
		fprintf(stderr, "Error writing to file stream: %s", strerror(errno));
	}
}

static void add_stats(malloc_stats_t *destination, const malloc_stats_t *source)
{
	destination->total_allocated_memory += source->total_allocated_memory;
	destination->total_mmapped_memory += source->total_mmapped_memory;
	destination->mmapped_regions += source->mmapped_regions;
	destination->total_allocated_heap_space += source->total_allocated_heap_space;
}
#endif

int malloc_info(int options, FILE* fp)
{
#ifdef MUSL_ITERATE_AND_STATS_API
	if (options != 0) {
		errno = EINVAL;
		return -1;
	}
	malloc_disable();
	stat_printf(print_to_file, fp, "<?xml version=\"1.0\"?>\n");
	stat_printf(print_to_file, fp, "<malloc version=\"%d\">\n", ALLOCATOR_VERSION);
	stat_printf(print_to_file, fp, "<threads>\n");
	size_t total_allocated_heap_space = print_threads(print_to_file, fp, XML);
	stat_printf(print_to_file, fp, "</threads>\n");
	print_total_free_heap_space(print_to_file, fp, total_allocated_heap_space, XML);
	stat_printf(print_to_file, fp, "</malloc>\n");
	malloc_enable();
#endif
	return 0;
}

#ifdef USE_JEMALLOC_DFX_INTF
extern struct mallinfo je_mallinfo();
extern void je_malloc_stats_print(void (*write_cb)(void *, const char *), void *cbopaque,
	const char *opts);
#endif

void malloc_stats_print(void (*write_cb) (void *, const char *), void *cbopaque, const char *opts)
{
#ifdef USE_JEMALLOC_DFX_INTF
	je_malloc_stats_print(write_cb, cbopaque, opts);
#elif defined(MUSL_ITERATE_AND_STATS_API)
	malloc_disable();
	stat_printf(
		write_cb,
		cbopaque,
		"%-11s %-23s %-20s %-20s\n",
		"thread_id",
		"total_allocated_memory",
		"total_mmapped_memory",
		"mmapped_regions"
	);
	size_t total_allocated_heap_space = print_threads(write_cb, cbopaque, TABLE);
	print_total_free_heap_space(write_cb, cbopaque, total_allocated_heap_space, TABLE);
	malloc_enable();
#endif
}

struct mallinfo2 mallinfo2(void)
{
#ifdef USE_JEMALLOC_DFX_INTF
	struct mallinfo info = je_mallinfo();
	struct mallinfo2 res = {
		.hblks = info.hblks,
		.hblkhd = info.hblkhd,
		.usmblks = info.usmblks,
		.uordblks = info.uordblks,
		.fordblks = info.fordblks,
	};
	return res;
#elif defined(MUSL_ITERATE_AND_STATS_API)
	malloc_disable();
	malloc_stats_t shared_stats = {0, 0, 0, 0};
	for (size_t i = 0; i < OCCUPIED_BIN_COUNT; ++i) {
		malloc_stats_t stats = add_up_chunks(__get_occupied_bin_by_idx(i));
		add_stats(&shared_stats, &stats);
	}

	struct mallinfo2 res = {
		.hblks = shared_stats.mmapped_regions,
		.hblkhd = shared_stats.total_mmapped_memory,
		.uordblks = shared_stats.total_allocated_memory,
		.fordblks = __get_total_heap_space() - shared_stats.total_allocated_heap_space
	};
	malloc_enable();
	return res;
#endif
	return (struct mallinfo2){};
}

struct mallinfo mallinfo(void)
{
	struct mallinfo2 mallinfo2_res = mallinfo2();
	return (struct mallinfo) {
		.hblks = mallinfo2_res.hblks,
		.hblkhd = mallinfo2_res.hblkhd,
		.uordblks = mallinfo2_res.uordblks,
		.fordblks = mallinfo2_res.fordblks,
	};
}
