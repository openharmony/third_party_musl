#include "test-malloc-info-stats-print.h"

static void stderr_stats_cb(void)
{
    malloc_stats_print(print_to_file, stderr, "");
}

static int parse_amount(char **s, long long *destination)
{
    char *end_ptr = NULL;
    long long result = strtoll(*s, &end_ptr, 10);
    if (end_ptr == *s) {
        return 0;
    }
    *s = end_ptr;
    if ((!isspace(*end_ptr) && *end_ptr != '\n' && *end_ptr != '\0') || result < 0) {
        return 0;
    }
    *destination = result;
    return 1;
}

static int populate_thread_stats(const char *input, const char *thread_id, malloc_thread_stats_t *stats)
{
    char *thread_id_start = strstr(input, thread_id);
    if (thread_id_start == NULL) {
        return 0;
    }
    thread_id_start += strlen(thread_id);
    int result = 1;
    result &= parse_amount(&thread_id_start, &stats->total_allocated_memory);
    result &= parse_amount(&thread_id_start, &stats->total_mmapped_memory);
    result &= parse_amount(&thread_id_start, &stats->mmapped_regions);

    return result;
}

static int populate_total_free_heap_space(const char *input, long long *total_free_heap_space)
{
    char *free_heap_space_start = strstr(input, "total free heap space:");
    if (free_heap_space_start == NULL) {
        return 0;
    }
    free_heap_space_start += strlen("total free heap space:");
    return parse_amount(&free_heap_space_start, total_free_heap_space);
}
