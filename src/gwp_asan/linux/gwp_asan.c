/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef USE_GWP_ASAN
#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/random.h>

#include "gwp_asan.h"

#include "musl_log.h"
#include "musl_malloc.h"
#include "pthread.h"
#include "pthread_impl.h"

#ifdef OHOS_ENABLE_PARAMETER
#include "sys_param.h"
#endif

#define MAX_SIMULTANEOUS_ALLOCATIONS 32
#define SAMPLE_RATE 2500
#define MIN_SAMPLE_SIZE 0
#define WHITE_LIST_PATH ""
#define GWP_ASAN_NAME_LEN 256
#define SECONDS_PER_DAY (24ULL * 60 * 60)
#define GWP_ASAN_PREDICT_TRUE(exp) __builtin_expect((exp) != 0, 1)
#define GWP_ASAN_PREDICT_FALSE(exp) __builtin_expect((exp) != 0, 0)
#define GWP_ASAN_LOGD(...) // change it to MUSL_LOGD to get gwp_asan debug log.
#define GWP_ASAN_NO_ADDRESS __attribute__((no_sanitize("address", "hwaddress")))

#if defined(__aarch64__)
#define REG_AARCH64_X29 29
static size_t get_pc(ucontext_t *context)
{
    return context->uc_mcontext.pc;
}

static size_t get_frame_pointer(ucontext_t *context)
{
    return context->uc_mcontext.regs[REG_AARCH64_X29];
}
#else
static size_t get_pc(ucontext_t *context)
{
    GWP_ASAN_LOGD("[gwp_asan] Unsupported platform");
    return 0;
}

static size_t get_frame_pointer(ucontext_t *context)
{
    GWP_ASAN_LOGD("[gwp_asan] Unsupported platform");
    return 0;
}
#endif

static bool gwp_asan_initialized = false;
static bool gwp_asan_thirdparty_telemetry = false;
static uint8_t process_sample_rate = 128;
static uint8_t force_sample_alloctor = 0;
static uint8_t previous_random_value = 0;

#ifndef _GNU_SOURCE
typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const char *dli_sname;
    void *dli_saddr;
} Dl_info;
#endif

// C interfaces of gwp_asan provided by LLVM side.
extern void init_gwp_asan(void *init_options);
extern void* gwp_asan_malloc(size_t bytes);
extern void gwp_asan_free(void *mem);
extern bool gwp_asan_should_sample();
extern bool gwp_asan_pointer_is_mine(void *mem);
extern bool gwp_asan_has_free_mem();
extern size_t gwp_asan_get_size(void *mem);
extern void gwp_asan_disable();
extern void gwp_asan_enable();
extern void gwp_asan_iterate(void *base, size_t size,
                                  void (*callback)(void* base, size_t size, void *arg), void *arg);


extern int dladdr(const void *addr, Dl_info *info);

static char *get_process_short_name(char *buf, size_t length)
{
    char *app = NULL;
    int fd = open("/proc/self/cmdline", O_RDONLY);
    if (fd != -1) {
        ssize_t ret = read(fd, buf, length - 1);
        if (ret != -1) {
            buf[ret] = 0;
            app = strrchr(buf, '/');
            if (app) {
                app++;
            } else {
                app = buf;
            }
            char *app_end = strchr(app, ':');
            if (app_end) {
                *app_end = 0;
            }
        }
        close(fd);
    }
    return app;
}

bool is_gwp_asan_disable()
{
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.disable.all";
    static CachedHandle para_handler = NULL;
    if (para_handler == NULL) {
        para_handler = CachedParameterCreate(para_name, "false");
    }
    const char *para_value = CachedParameterGet(para_handler);
    if (para_value != NULL && strcmp(para_value, "true") == 0) {
        return true;
    }
    return false;
}

bool force_sample_process_by_env()
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        return false;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.enable.app.";
    strcat(para_name, path);
    static CachedHandle app_enable_handle = NULL;
    if (app_enable_handle == NULL) {
        app_enable_handle = CachedParameterCreate(para_name, "false");
    }
    const char *param_value = CachedParameterGet(app_enable_handle);
    if (param_value != NULL) {
        if (strcmp(param_value, "true") == 0) {
            return true;
        }
    }
    return false;
}

bool force_sample_alloctor_by_env()
{
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.sample.all";
    static CachedHandle para_handler = NULL;
    if (para_handler == NULL) {
        para_handler = CachedParameterCreate(para_name, "false");
    }
    const char *para_value = CachedParameterGet(para_handler);
    if (para_value != NULL && strcmp(para_value, "true") == 0) {
        force_sample_alloctor = 1;
        return true;
    }
    return false;
}

bool should_sample_process()
{
#ifdef OHOS_ENABLE_PARAMETER
    if (force_sample_process_by_env()) {
        return true;
    }
#endif

    uint8_t random_value;
    // If getting a random number using a non-blocking fails, the random value is incremented.
    if (getrandom(&random_value, sizeof(random_value), GRND_RANDOM | GRND_NONBLOCK) == -1) {
        random_value = previous_random_value + 1;
        previous_random_value = random_value;
    }

    return (random_value % process_sample_rate) == 0 ? true : false;
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
const char *get_sample_parameter()
{
    static char sample_param[GWP_ASAN_NAME_LEN];
    snprintf(sample_param, GWP_ASAN_NAME_LEN, "%d:%d", SAMPLE_RATE, MAX_SIMULTANEOUS_ALLOCATIONS);
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        MUSL_LOGE("[gwp_asan]: get_sample_parameter get process_name failed!");
        return sample_param;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.sample.app.";
    strcat(para_name, path);
    CachedHandle handle = CachedParameterCreate(para_name, "");
    const char *param_value = CachedParameterGet(handle);
    if (!param_value || strlen(param_value) == 0) {
        MUSL_LOGE("[gwp_asan]: set default sample_rate and max_slot.");
        return sample_param;
    }
    return param_value;
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
void parse_sample_parameter(int *sample_rate, int *max_slot)
{
    const int MAX_SLOT_PARAM_SIZE = 20000;
    const int PARSE_FULL_PARAM = 2;
    const int PARSE_SINGLE_PARAM = 1;
    if (!sample_rate || !max_slot) {
        MUSL_LOGE("[gwp_asan]: parse_sample_parameter invalid pointer!");
        return;
    }
    const char *param_value = get_sample_parameter();
    int parsed_rate = 0, parsed_slot = 0;
    char extra_char;
    if (sscanf(param_value, "%d:%d%c", &parsed_rate, &parsed_slot, &extra_char) == PARSE_FULL_PARAM) {
        if (parsed_rate < 0 || parsed_rate > INT_MAX || parsed_slot < 0 ||
            (!gwp_asan_thirdparty_telemetry && parsed_slot > MAX_SLOT_PARAM_SIZE)) {
            MUSL_LOGE("[gwp_asan]: parse_sample_parameter abnormal rate and slot.");
            return;
        }
        *sample_rate = parsed_rate;
        *max_slot = parsed_slot;
    } else if (sscanf(param_value, "%d:%c", &parsed_rate, &extra_char) == PARSE_SINGLE_PARAM) {
        if (parsed_rate < 0 || parsed_rate > INT_MAX) {
            MUSL_LOGE("[gwp_asan]: parse_sample_parameter abnormal rate.");
            return;
        }
        *sample_rate = parsed_rate;
    } else if (sscanf(param_value, ":%d%c", &parsed_slot, &extra_char) == PARSE_SINGLE_PARAM) {
        if (parsed_slot < 0 || (!gwp_asan_thirdparty_telemetry && parsed_slot > MAX_SLOT_PARAM_SIZE)) {
            MUSL_LOGE("[gwp_asan]: parse_sample_parameter abnormal slot.");
            return;
        }
        *max_slot = parsed_slot;
    } else {
        MUSL_LOGE("[gwp_asan]: parse_sample_parameter invalid format: %{public}s.", param_value);
    }
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
int get_min_size_parameter()
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        MUSL_LOGE("[gwp_asan]: get_min_size_parameter get process_name failed!");
        return MIN_SAMPLE_SIZE;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.app.min_size.";
    strcat(para_name, path);
    CachedHandle handle = CachedParameterCreate(para_name, "0");
    const char *param_value = CachedParameterGet(handle);
    if (!param_value || strlen(param_value) == 0) {
        MUSL_LOGE("[gwp_asan]: set default min_sample_size %{public}d.", MIN_SAMPLE_SIZE);
        return MIN_SAMPLE_SIZE;
    }
    char *endPtr = NULL;
    long min_size = strtol(param_value, &endPtr, 10);
    if (*endPtr != '\0' || min_size < 0 || min_size > INT_MAX) {
        MUSL_LOGE("[gwp_asan]: set default min_sample_size %{public}d.", MIN_SAMPLE_SIZE);
        return MIN_SAMPLE_SIZE;
    }
    return (int)min_size;
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
const char *get_library_parameter()
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        MUSL_LOGE("[gwp_asan]: get_library_parameter get process_name failed!");
        return WHITE_LIST_PATH;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.app.library.";
    strcat(para_name, path);
    CachedHandle handle = CachedParameterCreate(para_name, "");
    const char *param_value = CachedParameterGet(handle);
    if (!param_value || strlen(param_value) == 0) {
        MUSL_LOGE("[gwp_asan]: set default white_list_path %{public}s.", WHITE_LIST_PATH);
        return WHITE_LIST_PATH;
    }
    return param_value;
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
uint64_t get_gray_begin_parameter()
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        MUSL_LOGE("[gwp_asan]: get_gray_begin_parameter get process_name failed!");
        return 0;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.gray_begin.app.";
    strcat(para_name, path);
    CachedHandle handle = CachedParameterCreate(para_name, "");
    const char *param_value = CachedParameterGet(handle);
    if (!param_value || strlen(param_value) == 0) {
        MUSL_LOGE("[gwp_asan]: gray_begin not set.");
        return 0;
    }
    char *endPtr = NULL;
    unsigned long long gray_begin = strtoull(param_value, &endPtr, 10);
    if (*endPtr != '\0' || gray_begin < 0 || gray_begin > ULLONG_MAX) {
        MUSL_LOGE("[gwp_asan]: invalid gray_begin value: %s", param_value);
        return 0;
    }
    return (uint64_t)gray_begin;
}

/* This function is used for gwp_asan to get telemetry param to init gwp_asan.
 * It is not signal-safe, do not call from signal handlers.
 */
uint64_t get_gray_days_parameter()
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        MUSL_LOGE("[gwp_asan]: get_gray_days_parameter get process_name failed!");
        return 0;
    }
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.gray_days.app.";
    strcat(para_name, path);
    CachedHandle handle = CachedParameterCreate(para_name, "");
    const char *param_value = CachedParameterGet(handle);
    if (!param_value || strlen(param_value) == 0) {
        MUSL_LOGE("[gwp_asan]: gray_days not set.");
        return 0;
    }
    char *endPtr = NULL;
    unsigned long long gray_days = strtoull(param_value, &endPtr, 10);
    if (*endPtr != '\0' || gray_days < 0 || gray_days > ULLONG_MAX) {
        MUSL_LOGE("[gwp_asan]: invalid gray_days value: %s", param_value);
        return 0;
    }
    return (uint64_t)gray_days * SECONDS_PER_DAY;
}

#define ASAN_LOG_LIB "libasan_logger.z.so"
static void (*WriteSanitizerLog)(char*, size_t, char*);
static void* handle = NULL;
static bool try_load_asan_logger = false;
static pthread_mutex_t gwpasan_mutex = PTHREAD_MUTEX_INITIALIZER;

void gwp_asan_printf(const char *fmt, ...)
{
    char para_name[GWP_ASAN_NAME_LEN] = "gwp_asan.log.path";
    static CachedHandle para_handler = NULL;
    if (para_handler == NULL) {
        para_handler = CachedParameterCreate(para_name, "default");
    }
    const char *para_value = CachedParameterGet(para_handler);
    if (strcmp(para_value, "file") == 0) {
        char process_short_name[GWP_ASAN_NAME_LEN];
        char *path = get_process_short_name(process_short_name, GWP_ASAN_NAME_LEN);
        if (!path) {
            MUSL_LOGE("[gwp_asan]: get_process_short_name failed!");
            return;
        }
        char log_path[GWP_ASAN_NAME_LEN];
        snprintf(log_path, GWP_ASAN_NAME_LEN, "%s%s.%s.%d.log", GWP_ASAN_LOG_DIR, GWP_ASAN_LOG_TAG, path, getpid());
        FILE *fp = fopen(log_path, "a+");
        if (!fp) {
            MUSL_LOGE("[gwp_asan]: %{public}s fopen %{public}s failed!", path, log_path);
            return;
        } else {
            MUSL_LOGE("[gwp_asan]: %{public}s fopen %{public}s succeed.", path, log_path);
        }
        va_list ap;
        va_start(ap, fmt);
        int result = vfprintf(fp, fmt, ap);
        va_end(ap);
        fclose(fp);
        if (result < 0) {
            MUSL_LOGE("[gwp_asan] %{public}s write log failed!\n", path);
        }
        return;
    }
    if (strcmp(para_value, "default") == 0) {
        va_list ap;
        va_start(ap, fmt);
        char log_buffer[PATH_MAX];
        int result = vsnprintf(log_buffer, PATH_MAX, fmt, ap);
        va_end(ap);
        if (result < 0) {
            MUSL_LOGE("[gwp_asan] write log failed!\n");
        }
        if (WriteSanitizerLog != NULL) {
            WriteSanitizerLog(log_buffer, strlen(log_buffer), "faultlogger");
            return;
	}
        if (try_load_asan_logger) {
            return;
        }
        pthread_mutex_lock(&gwpasan_mutex);
        if (WriteSanitizerLog != NULL) {
            WriteSanitizerLog(log_buffer, strlen(log_buffer), "faultlogger");
            pthread_mutex_unlock(&gwpasan_mutex);
            return;
        }
        if (!try_load_asan_logger && handle == NULL) {
            handle = dlopen(ASAN_LOG_LIB, RTLD_LAZY);
	    if (handle == NULL) {
                pthread_mutex_unlock(&gwpasan_mutex);
                return;
            }
            try_load_asan_logger = true;
            *(void**)(&WriteSanitizerLog) = dlsym(handle, "WriteSanitizerLog");
            if (WriteSanitizerLog != NULL) {
                WriteSanitizerLog(log_buffer, strlen(log_buffer), "faultlogger");
            }
        }
        pthread_mutex_unlock(&gwpasan_mutex);
        return;
    }
    if (strcmp(para_value, "stdout") == 0) {
        va_list ap;
        va_start(ap, fmt);
        int result = vfprintf(stdout, fmt, ap);
        va_end(ap);
        if (result < 0) {
            MUSL_LOGE("[gwp_asan] write log failed!\n");
        }
        return;
    }
}

void gwp_asan_printf_backtrace(uintptr_t *trace_buffer, size_t trace_length, printf_t gwp_asan_printf)
{
    if (trace_length == 0) {
        gwp_asan_printf("It dosen't see any stack trace!\n");
    }
    for (size_t i = 0; i < trace_length; i++) {
        if (trace_buffer[i]) {
            Dl_info info;
            if (dladdr(trace_buffer[i], &info)) {
                size_t offset = trace_buffer[i] - (uintptr_t)info.dli_fbase;
                gwp_asan_printf("  #%zu %p (%s+%p)\n", i, trace_buffer[i], info.dli_fname, offset);
            } else {
                gwp_asan_printf("  #%zu %p\n", i, trace_buffer[i]);
            }
        }
    }
    gwp_asan_printf("\n");
}

// Strip pc because pc may have been protected by pac(Pointer Authentication) when build with "-mbranch-protection".
size_t strip_pac_pc(size_t ptr)
{
#if defined(MUSL_AARCH64_ARCH)
    register size_t x30 __asm__("x30") = ptr;
    // "xpaclri" is a NOP on pre armv8.3-a arch.
    __asm__ volatile("xpaclri" : "+r"(x30));
    return x30;
#else
    return ptr;
#endif
}

/* This function is used for gwp_asan to record the call stack when allocate and deallocate.
 * So we implemented a fast unwind function by using fp.
 * The unwind process may stop because the value of fp is incorrect(fp was not saved on the stack due to optimization)
 * We can build library with "-fno-omit-frame-pointer" to get a more accurate call stack.
 * The basic unwind principle is as follows:
 * Stack: func1->func2->func3
 * --------------------| [Low Adress]
 * |   fp      |       |-------->|
 * |   lr      | func3 |         |
 * |   ......  |       |         |
 * --------------------|<--------|
 * |   fp      |       |-------->|
 * |   lr      | func2 |         |
 * |   ......  |       |         |
 * --------------------|         |
 * |   fp      |       |<--------|
 * |   lr      | func1 |
 * |   ......  |       |
 * --------------------| [High Address]
 */
GWP_ASAN_NO_ADDRESS size_t run_unwind(size_t *frame_buf,
                                      size_t num_frames,
                                      size_t max_record_stack,
                                      size_t current_frame_addr)
{
    size_t stack_end = (size_t)(__pthread_self()->stack);
    size_t prev_fp = 0;
    size_t prev_lr = 0;
    while (true) {
        unwind_info *frame = (unwind_info*)(current_frame_addr);
        GWP_ASAN_LOGD("[gwp_asan] unwind info:%{public}d cur:%{public}p, end:%{public}p fp:%{public}p lr:%{public}p \n",
                      num_frames, current_frame_addr, stack_end, frame->fp, frame->lr);
        size_t stripped_lr = strip_pac_pc(frame->lr);
        if (!stripped_lr) {
            break;
        }
        if (num_frames < max_record_stack) {
            frame_buf[num_frames] = stripped_lr - 4;
        }
        ++num_frames;
        if (frame->fp == prev_fp || frame->lr == prev_lr || frame->fp < current_frame_addr + sizeof(unwind_info) ||
            frame->fp >= stack_end || frame->fp % sizeof(void*) != 0) {
            break;
        }
        prev_fp = frame->fp;
        prev_lr = frame->lr;
        current_frame_addr = frame->fp;
    }

    return num_frames;
}

GWP_ASAN_NO_ADDRESS size_t libc_gwp_asan_unwind_fast(size_t *frame_buf, size_t max_record_stack)
{
    size_t current_frame_addr = __builtin_frame_address(0);
    size_t num_frames = 0;

    return run_unwind(frame_buf, num_frames, max_record_stack, current_frame_addr);
}

// Get fault address from sigchain in signal handler
GWP_ASAN_NO_ADDRESS size_t libc_gwp_asan_unwind_segv(size_t *frame_buf, size_t max_record_stack, void *signal_context)
{
    ucontext_t *context = (ucontext_t *)signal_context;
    size_t current_frame_addr = get_frame_pointer(context);
    size_t pc = get_pc(context);
    size_t num_frames = 0;

    if (current_frame_addr && pc) {
        frame_buf[num_frames] = strip_pac_pc(pc);
        ++num_frames;
    }
    return run_unwind(frame_buf, num_frames, max_record_stack, current_frame_addr);
}

void init_gwp_asan_by_telemetry(int *sample_rate, int *max_simultaneous_allocations,
                                int *min_sample_size, const char **white_list_path)
{
    uint64_t gray_begin = get_gray_begin_parameter();
    uint64_t gray_days = get_gray_days_parameter();
    uint64_t now = (uint64_t)time(NULL);
    if (gray_begin > 0) {
        MUSL_LOGI("[gwp_asan]: init_gwp_asan_by_telemetry in thirdparty telemetry.");
        if (now - gray_begin > gray_days) {
            MUSL_LOGI("[gwp_asan]: init_gwp_asan_by_telemetry thirdparty telemetry expired.");
            return;
        }
        gwp_asan_thirdparty_telemetry = true;
    } else {
        MUSL_LOGI("[gwp_asan]: init_gwp_asan_by_telemetry in inner telemetry.");
        *min_sample_size = get_min_size_parameter();
        *white_list_path = get_library_parameter();
    }
    parse_sample_parameter(sample_rate, max_simultaneous_allocations);
}

bool init_gwp_asan_process(gwp_asan_option gwp_asan_option)
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        return false;
    }

    MUSL_LOGE("[gwp_asan]: %{public}d %{public}s gwp_asan initializing.\n", getpid(), path);
    init_gwp_asan((void*)&gwp_asan_option);
    gwp_asan_initialized = true;
    MUSL_LOGE("[gwp_asan]: %{public}d %{public}s gwp_asan initialized.\n", getpid(), path);
    return true;
}

bool may_init_gwp_asan(bool force_init)
{
    int sample_rate = SAMPLE_RATE;
    int max_simultaneous_allocations = MAX_SIMULTANEOUS_ALLOCATIONS;
    int min_sample_size = MIN_SAMPLE_SIZE;
    const char *white_list_path = WHITE_LIST_PATH;

    GWP_ASAN_LOGD("[gwp_asan]: may_init_gwp_asan enter force_init:%{public}d.\n", force_init);
    if (gwp_asan_initialized) {
        GWP_ASAN_LOGD("[gwp_asan]: may_init_gwp_asan return because gwp_asan_initialized is true.\n");
        return false;
    }
#ifdef OHOS_ENABLE_PARAMETER
    // Turn off gwp_asan.
    if (GWP_ASAN_PREDICT_FALSE(is_gwp_asan_disable())) {
        GWP_ASAN_LOGD("[gwp_asan]: may_init_gwp_asan return because gwp_asan is disable by env.\n");
        return false;
    }
#endif

    if (!force_init && !should_sample_process()) {
        GWP_ASAN_LOGD("[gwp_asan]: may_init_gwp_asan return because sample not hit.\n");
        return false;
    }

#ifdef OHOS_ENABLE_PARAMETER
    // All memory allocations use gwp_asan.
    force_sample_alloctor_by_env();
    init_gwp_asan_by_telemetry(&sample_rate, &max_simultaneous_allocations,
        &min_sample_size, &white_list_path);
    MUSL_LOGE("[gwp_asan]: sample_rate:%{public}d, slot:%{public}d, "\
        "min_sample_size:%{public}d, white_list_path:%{public}s",
        sample_rate, max_simultaneous_allocations, min_sample_size, white_list_path);
#endif

    gwp_asan_option gwp_asan_option = {
        .enable = true,
        .install_fork_handlers = true,
        .install_signal_handlers = true,
        .max_simultaneous_allocations = max_simultaneous_allocations,
        .sample_rate = sample_rate,
        .backtrace = libc_gwp_asan_unwind_fast,
        .gwp_asan_printf = gwp_asan_printf,
        .printf_backtrace = gwp_asan_printf_backtrace,
        .segv_backtrace = libc_gwp_asan_unwind_segv,
        .min_sample_size = min_sample_size,
        .white_list_path = white_list_path
    };

    return init_gwp_asan_process(gwp_asan_option);
}

bool init_gwp_asan_by_libc(bool force_init)
{
    char buf[GWP_ASAN_NAME_LEN];
    char *path = get_process_short_name(buf, GWP_ASAN_NAME_LEN);
    if (!path) {
        return false;
    }
    // We don't sample appspawn, and the chaild process decides whether to sample or not.
    if (strcmp(path, "appspawn") == 0 || strcmp(path, "sh") == 0) {
        return false;
    }
    return may_init_gwp_asan(force_init);
}

void* get_platform_gwp_asan_tls_slot()
{
    return (void*)(&(__pthread_self()->gwp_asan_tls));
}

void* libc_gwp_asan_malloc(size_t bytes)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return MuslFunc(malloc)(bytes);
    }
    void *res = NULL;
    if (GWP_ASAN_PREDICT_FALSE(force_sample_alloctor || gwp_asan_should_sample())) {
        res = gwp_asan_malloc(bytes);
        if (res != NULL) {
            return res;
        }
    }
    return MuslFunc(malloc)(bytes);
}

void* libc_gwp_asan_calloc(size_t nmemb, size_t size)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return MuslFunc(calloc)(nmemb, size);
    }

    if (GWP_ASAN_PREDICT_FALSE(force_sample_alloctor || gwp_asan_should_sample())) {
        size_t total_bytes;
        void* result = NULL;
        if (!__builtin_mul_overflow(nmemb, size, &total_bytes)) {
            GWP_ASAN_LOGD("[gwp_asan]: call gwp_asan_malloc nmemb:%{public}d size:%{public}d.\n", nmemb, size);
            result = gwp_asan_malloc(total_bytes);
            if (result != NULL) {
                return result;
            }
        }
    }

    return MuslFunc(calloc)(nmemb, size);
}

void* libc_gwp_asan_realloc(void *ptr, size_t size)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return MuslFunc(realloc)(ptr, size);
    }

    if (GWP_ASAN_PREDICT_FALSE(gwp_asan_pointer_is_mine(ptr))) {
        GWP_ASAN_LOGD("[gwp_asan]: call gwp_asan_malloc ptr:%{public}p size:%{public}d.\n", ptr, size);
        if (GWP_ASAN_PREDICT_FALSE(size == 0)) {
            gwp_asan_free(ptr);
            return NULL;
        }

        void* new_addr = gwp_asan_malloc(size);
        if (new_addr != NULL) {
            size_t old_size = gwp_asan_get_size(ptr);
            memcpy(new_addr, ptr, (size < old_size) ? size : old_size);
            gwp_asan_free(ptr);
            return new_addr;
        } else {
            // Use the default allocator if gwp malloc failed.
            void* addr_of_default_allocator = MuslFunc(malloc)(size);
            if (addr_of_default_allocator != NULL) {
                size_t old_size = gwp_asan_get_size(ptr);
                memcpy(addr_of_default_allocator, ptr, (size < old_size) ? size : old_size);
                gwp_asan_free(ptr);
                return addr_of_default_allocator;
            } else {
                return NULL;
            }
        }
    }
    return MuslFunc(realloc)(ptr, size);
}

void libc_gwp_asan_free(void *addr)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return MuslFunc(free)(addr);
    }
    if (GWP_ASAN_PREDICT_FALSE(gwp_asan_pointer_is_mine(addr))) {
        return gwp_asan_free(addr);
    }
    return MuslFunc(free)(addr);
}

size_t libc_gwp_asan_malloc_usable_size(void *addr)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return MuslMalloc(malloc_usable_size)(addr);
    }
    if (GWP_ASAN_PREDICT_FALSE(gwp_asan_pointer_is_mine(addr))) {
        return gwp_asan_get_size(addr);
    }
    return MuslMalloc(malloc_usable_size)(addr);
}

void libc_gwp_asan_malloc_iterate(void *base, size_t size,
                             void (*callback)(uintptr_t base, size_t size, void *arg), void *arg)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return;
    }
    if (GWP_ASAN_PREDICT_FALSE(gwp_asan_pointer_is_mine(base))) {
        return gwp_asan_iterate(base, size, callback, arg);
    }
    return;
}

void libc_gwp_asan_malloc_disable()
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return;
    }

    return gwp_asan_disable();
}

void libc_gwp_asan_malloc_enable()
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return;
    }

    return gwp_asan_enable();
}

bool libc_gwp_asan_has_free_mem()
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return false;
    }
    gwp_asan_disable();
    int res = gwp_asan_has_free_mem();
    gwp_asan_enable();
    return res;
}
bool libc_gwp_asan_ptr_is_mine(void *addr)
{
    if (GWP_ASAN_PREDICT_TRUE(!gwp_asan_initialized)) {
        return false;
    }

    return gwp_asan_pointer_is_mine(addr);
}
#else
#include <stdbool.h>

// Used for appspawn.
bool may_init_gwp_asan(bool force_init)
{
    return false;
}
#endif
