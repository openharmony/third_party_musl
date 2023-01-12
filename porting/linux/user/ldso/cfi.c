#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/prctl.h>
#include "cfi.h"
#include "ld_log.h"
#include "namespace.h"

/* This module provides support for LLVM CFI Cross-DSO by implementing the __cfi_slowpath() and __cfi_slowpath_diag()
 * functions. These two functions will be called before visiting other dso's resources. The responsibility is to
 * calculate the __cfi_check() of the target dso, and call it.
 * The relationship between __cfi_check and dso addr is:
 *     __cfi_check = AlignDown(addr, shadow_alignment) + shadow_alignment - (shadow_value - 2) * 4096;
 * The shadow_alignment presents the size of memory mapped by one shadow value.
 * The shadow_value presents the distance between the __cfi_check() and the the end address of each shadow alignment
 * in the dso. It can be presented as a multiple of 4096.
 * The CFI shadow is used to store shadow value(s) of each dso.
 */

#define MAX(a,b)                (((a) > (b)) ? (a) : (b))
#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#define ALIGN_UP(a, b)          (((a) + (b) - 1) & -(b))
#define ALIGN_DOWN(a, b)        ((a) & -(b))
#if DL_FDPIC
#define LADDR(p, v)             laddr((p), (v))
#else
#define LADDR(p, v)             (void *)((p)->base + (v))
#endif

/* Function ptr for __cfi_check() */
typedef int (*cfi_check_t)(uint64_t, void *, void *);

static const uintptr_t shadow_granularity = LIBRARY_ALIGNMENT_BITS;
static const uintptr_t cfi_check_granularity = 12;
static const uintptr_t shadow_alignment = 1UL << shadow_granularity;
static uintptr_t shadow_size = 0;
/* Start addr of the CFI shadow */
static char *cfi_shadow_start = NULL;
/* List head of all the DSOs loaded by the process */
static struct dso *dso_list_head = NULL;

/* Shadow value */
static const uint16_t sv_invalid = 0;
static const uint16_t sv_uncheck = 1;
static const uint16_t sv_valid_min = 2;

#if defined(__LP64__)
static const uintptr_t max_target_addr = 0xffffffffffff;
#else
static const uintptr_t max_target_addr = 0xffffffff;
#endif

/* Create a cfi shadow */
static bool create_cfi_shadow(void);

/* Map dsos to CFI shadow */
static bool add_dso_to_cfi_shadow(struct dso *dso);
static bool fill_shadow_value_to_shadow(uintptr_t begin, uintptr_t end, uintptr_t cfi_check, uint16_t type);

/* Find the __cfi_check() of target dso and call it */
void __cfi_slowpath(uint64_t call_site_type_id, void *func_ptr);
void __cfi_slowpath_diag(uint64_t call_site_type_id, void *func_ptr, void *diag_data);

static inline uintptr_t addr_to_offset(uintptr_t addr, int bits)
{
    /* Convert addr to CFI shadow offset.
     * Shift left 1 bit because the shadow value is uint16_t.
     */
    return (addr >> bits) << 1;
}

static void set_cfi_shadow_name()
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, cfi_shadow_start, shadow_size, "cfi_shadow:musl");

    return;
}

static struct symdef find_cfi_check_sym(struct dso *p)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);
	if (p == NULL) {
        LD_LOGE("[%{public}s] has null param!\n", __FUNCTION__);
        struct symdef emptysym = {0};
        return emptysym;
    }
    ns_t *ns = p->namespace;
    struct verinfo verinfo = { .s = "__cfi_check", .v = "", .use_vna_hash = false };
    return find_sym2(p, &verinfo, 0, 1, ns);
}

static uintptr_t get_cfi_check_addr(uint16_t value, void* func_ptr)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    uintptr_t addr = (uintptr_t)func_ptr;
    uintptr_t aligned_addr = ALIGN_DOWN(addr, shadow_alignment) + shadow_alignment;
    uintptr_t cfi_check_func_addr = aligned_addr - ((uintptr_t)(value - sv_valid_min) << cfi_check_granularity);
#ifdef __arm__
    LD_LOGI("[%{public}s] __arm__ defined!\n", __FUNCTION__);
    cfi_check_func_addr++;
#endif
    LD_LOGI("[%{public}s] the cfi_check_func_addr is %{public}p!\n", __FUNCTION__, cfi_check_func_addr);

    return cfi_check_func_addr;
}

static void cfi_slowpath_common(uint64_t call_site_type_id, void *func_ptr, void *diag_data)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);
    LD_LOGI("[%{public}s] func_ptr[%{public}p] !\n", __FUNCTION__, func_ptr);

    uint16_t value = sv_invalid;

#if defined(__aarch64__)
    LD_LOGI("[%{public}s] __aarch64__ defined!\n", __FUNCTION__);
    uintptr_t addr = (uintptr_t)func_ptr & ((1ULL << 56) - 1);
#else
    LD_LOGI("[%{public}s] __aarch64__ not defined!\n", __FUNCTION__);
    uintptr_t addr = func_ptr;
#endif

    /* Get shadow value */
    uintptr_t offset = addr_to_offset(addr, shadow_granularity);
    if (offset > shadow_size) {
        value = sv_invalid;
    } else {
        value = *((uint16_t*)(cfi_shadow_start + offset));
    }
    LD_LOGI("[%{public}s] the value is 0x%{public}x!\n", __FUNCTION__, value);

    struct dso *dso = NULL;
    switch (value)
    {
    case sv_invalid:
        dso = (struct dso *)addr2dso((size_t)__builtin_return_address(0));
        if (dso == NULL) {
            LD_LOGE("[%{public}s] can not find the dso!\n", __FUNCTION__);
            __builtin_trap();
        }
        LD_LOGI("[%{public}s] dso name[%{public}s]!\n", __FUNCTION__, dso->name);

        struct symdef cfi_check_sym = find_cfi_check_sym(dso);
        if (!cfi_check_sym.sym) {
            LD_LOGE("[%{public}s] can not find the __cfi_check in the dso!\n", __FUNCTION__);
            __builtin_trap();
        }
        LD_LOGI("[%{public}s] cfi_check addr[%{public}p]!\n",
            __FUNCTION__, LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value));
        ((cfi_check_t)LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value))(call_site_type_id, func_ptr, diag_data);
        break;
    case sv_uncheck:
        break;
    default:
        ((cfi_check_t)get_cfi_check_addr(value, func_ptr))(call_site_type_id, func_ptr, diag_data);
        break;
    }

    return;
}

bool init_cfi_shadow(struct dso *dso_list)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    if (dso_list == NULL) {
        LD_LOGW("[%{public}s] has null param!\n", __FUNCTION__);
        return true;
    }

    /* Save the head node of dso list */
    dso_list_head = dso_list;

    return map_dso_to_cfi_shadow(dso_list);
}

bool map_dso_to_cfi_shadow(struct dso *dso)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    bool has_cfi_check = false;

    if (dso == NULL) {
        LD_LOGE("[%{public}s] has null param!\n", __FUNCTION__);
        return true;
    }

    /* Find __cfi_check symbol in dso list */
    for (struct dso *p = dso; p; p = p->next) {
        if (find_cfi_check_sym(p).sym) {
            LD_LOGI("[%{public}s] find __cfi_check function in dso %{public}s!\n", __FUNCTION__, p->name);
            has_cfi_check = true;
            break;
        }
    }

    /* If the cfi shadow does not exist, create it and map all the dsos and its dependents to it. */
    if (cfi_shadow_start == NULL) {
        if (has_cfi_check) {
            if (!create_cfi_shadow()) {
                LD_LOGE("[%{public}s] create cfi shadow failed!\n", __FUNCTION__);
                return false;
            }
            LD_LOGI("[%{public}s] add_dso_to_cfi_shadow with dso_list_head!\n", __FUNCTION__);
            add_dso_to_cfi_shadow(dso_list_head);
            set_cfi_shadow_name();
        }
    /* If the cfi shadow exists, map the current dso and its dependents to it. */
    } else {
        LD_LOGI("[%{public}s] add_dso_to_cfi_shadow with dso!\n", __FUNCTION__);
        add_dso_to_cfi_shadow(dso);
        set_cfi_shadow_name();
    }

    return true;
}

void unmap_dso_from_cfi_shadow(struct dso *dso)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    if (dso == NULL) {
        LD_LOGE("[%{public}s] has null param!\n", __FUNCTION__);
        return;
    }

    LD_LOGI("[%{public}s] unmap dso %{public}s from shadow!\n", __FUNCTION__, dso->name);

    if (cfi_shadow_start == NULL)
        return;

    if (dso->map == 0 || dso->map_len == 0)
        return;

    /* Set the dso's shadow value as invalid. */
    fill_shadow_value_to_shadow(dso->map, dso->map + dso->map_len, 0, sv_invalid);
    set_cfi_shadow_name();

    return;
}

static bool create_cfi_shadow(void)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    /* Each process can load up to (max_target_addr >> shadow_granularity) dsos. Shift left 1 bit because the shadow 
     * value is uint16_t. The size passed to mmap() should be aligned with 4096, so shadow_size should be aligned.
     */
    shadow_size = ALIGN_UP(((max_target_addr >> shadow_granularity) << 1), PAGE_SIZE);

    uintptr_t *mmap_addr = mmap(NULL, shadow_size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    if (mmap_addr == MAP_FAILED) {
        LD_LOGE("[%{public}s] mmap failed!\n", __FUNCTION__);
        return false;
    }

    cfi_shadow_start = (char*)mmap_addr;
    LD_LOGI("[%{public}s] the cfi_shadow_start addr is %{public}p!\n", __FUNCTION__, cfi_shadow_start);

    return true;
}

static bool add_dso_to_cfi_shadow(struct dso *dso)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    for (struct dso *p = dso; p; p = p->next) {
        LD_LOGI("[%{public}s] start to deal with dso %{public}s!\n", __FUNCTION__, p->name);
        if (p->map == 0 || p->map_len == 0) {
            LD_LOGI("[%{public}s] the dso has no data!\n", __FUNCTION__);
            continue;
        }

        struct symdef cfi_check_sym = find_cfi_check_sym(p);
        /* If the dso doesn't have __cfi_check(), set it's shadow value unchecked. */
        if (!cfi_check_sym.sym) {
            LD_LOGI("[%{public}s] the dso has no __cfi_check func, call fill_shadow_value_to_shadow!\n", __FUNCTION__);
            if (!fill_shadow_value_to_shadow(p->map, p->map + p->map_len, 0, sv_uncheck)) {
                LD_LOGE("[%{public}s] add dso to cfi shadow failed!\n", __FUNCTION__);
                return false;
            }
        /* If the dso has __cfi_check(), set it's shadow value valid. */
        } else {
            LD_LOGI("[%{public}s] the dso has __cfi_check func,call fill_shadow_value_to_shadow!\n", __FUNCTION__);
            uintptr_t end = p->map + p->map_len;
            uintptr_t cfi_check = LADDR(cfi_check_sym.dso, cfi_check_sym.sym->st_value);

            if (cfi_check == 0) {
                LD_LOGE("[%{public}s] the dso has null cfi_check func!\n", __FUNCTION__);
                return false;
            }
            if (!fill_shadow_value_to_shadow(p->map, end, cfi_check, sv_valid_min)) {
                LD_LOGE("[%{public}s] add dso to cfi shadow failed!\n", __FUNCTION__);
                return false;
            }
        }
        LD_LOGI("[%{public}s] finish to deal with dso %{public}s!\n", __FUNCTION__, p->name);
    }

    return true;
}

static bool fill_shadow_value_to_shadow(uintptr_t begin, uintptr_t end, uintptr_t cfi_check, uint16_t type)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);
    LD_LOGI("[%{public}s] begin[%{public}x] end[%{public}x] cfi_check[%{public}x] type[%{public}x]!\n",
        __FUNCTION__, begin, end, cfi_check, type);

    if (begin == 0 || end == 0) {
        LD_LOGE("[%{public}s] has error param!\n", __FUNCTION__);
        return false;
    }

    /* To ensure the atomicity of the CFI shadow operation, we create a temp_shadow, write the shadow value to 
     * the temp_shadow, and then write it back to the CFI shadow by mremap().*/
    begin = ALIGN_DOWN(MAX(begin, cfi_check), shadow_alignment);
    char* shadow_begin = cfi_shadow_start + addr_to_offset(begin, LIBRARY_ALIGNMENT_BITS);
    char* shadow_end = (char*)(((uint16_t*)(cfi_shadow_start + addr_to_offset(end - 1, LIBRARY_ALIGNMENT_BITS))) + 1);
    char* aligned_shadow_begin = (char*)ALIGN_DOWN((uintptr_t)shadow_begin, PAGE_SIZE);
    char* aligned_shadow_end = (char*)ALIGN_UP((uintptr_t)shadow_end, PAGE_SIZE);

    uint16_t tmp_shadow_size = aligned_shadow_end - aligned_shadow_begin;
    uint16_t offset_begin = shadow_begin - aligned_shadow_begin;
    uint16_t offset_end = shadow_end - aligned_shadow_begin;

    char* tmp_shadow_start = (char*)mmap(NULL, tmp_shadow_size,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (tmp_shadow_start == MAP_FAILED) {
        LD_LOGE("[%{public}s] mmap failed!\n", __FUNCTION__);
        return false;
    }

    LD_LOGI("[%{public}s] tmp_shadow_start is %{public}p\t tmp_shadow_size is 0x%{public}x!\n",
        __FUNCTION__, tmp_shadow_start, tmp_shadow_size);
    memcpy(tmp_shadow_start, aligned_shadow_begin, offset_begin);
    memcpy(tmp_shadow_start + offset_end, shadow_end, aligned_shadow_end - shadow_end);

    /* If the dso has __cfi_check(), calculate valid shadow value */
    if (type == sv_valid_min) {
#ifdef __arm__
        uint16_t shadow_value_begin = ((begin + shadow_alignment - (cfi_check - 1))
            >> cfi_check_granularity) + sv_valid_min;
#else
        uint16_t shadow_value_begin = ((begin + shadow_alignment - cfi_check)
            >> cfi_check_granularity) + sv_valid_min;
#endif
        LD_LOGI("[%{public}s] shadow_value_begin is 0x%{public}x!\n", __FUNCTION__, shadow_value_begin);
        uint16_t shadow_value_step = 1 << (shadow_granularity - cfi_check_granularity);
        uint16_t shadow_value = shadow_value_begin;

        /* Set shadow_value */
        for (uint16_t *shadow_addr = tmp_shadow_start + offset_begin;
            shadow_addr != tmp_shadow_start + offset_end; shadow_addr++) {
            if (shadow_value < shadow_value_begin) {
                *shadow_addr = sv_uncheck;
                continue;
            }
            *shadow_addr = shadow_value;
            shadow_value += shadow_value_step;
        }
    /* in these cases, shadow_value will always be sv_uncheck or sv_invalid */
    } else if (type == sv_uncheck || type == sv_invalid) {
        /* Set shadow_value */
        for (uint16_t *shadow_addr = tmp_shadow_start + offset_begin;
            shadow_addr != tmp_shadow_start + offset_end; shadow_addr++) {
            *shadow_addr = type;
        }
    } else {
        LD_LOGE("[%{public}s] has error param!\n", __FUNCTION__);
        return false;
    }

    mprotect(tmp_shadow_start, tmp_shadow_size, PROT_READ);
    /* Remap temp_shadow to CFI shadow. */
    uint16_t* mremap_addr = mremap(tmp_shadow_start, tmp_shadow_size, tmp_shadow_size,
        MREMAP_MAYMOVE | MREMAP_FIXED, aligned_shadow_begin);

    if (mremap_addr == MAP_FAILED)
    {
        LD_LOGE("[%{public}s] mremap failed!\n", __FUNCTION__);
        return false;
    }

    LD_LOGI("[%{public}s] fill completed!\n", __FUNCTION__);
    return true;
}

void __cfi_slowpath(uint64_t call_site_type_id, void *func_ptr)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    if (func_ptr == NULL) {
        LD_LOGE("[%{public}s] has error param!\n", __FUNCTION__);
        return;
    }

    if (cfi_shadow_start == NULL) {
        LD_LOGE("[%{public}s] the cfi_shadow_start is null!\n", __FUNCTION__);
        __builtin_trap();
    }

    cfi_slowpath_common(call_site_type_id, func_ptr, NULL);

    return;
}

void __cfi_slowpath_diag(uint64_t call_site_type_id, void *func_ptr, void *diag_data)
{
    LD_LOGI("[%{public}s] start!\n", __FUNCTION__);

    if (func_ptr == NULL) {
        LD_LOGE("[%{public}s] has error param!\n", __FUNCTION__);
        return;
    }

    if (cfi_shadow_start == NULL) {
        LD_LOGE("[%{public}s] the cfi_shadow_start is null!\n", __FUNCTION__);
        __builtin_trap();
    }

    cfi_slowpath_common(call_site_type_id, func_ptr, diag_data);

    return;
}