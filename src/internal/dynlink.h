#ifndef _INTERNAL_RELOC_H
#define _INTERNAL_RELOC_H

#include <features.h>
#include <elf.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#ifndef __LITEOS__
#include <link.h>
#include <sys/types.h>
#include <stdlib.h>
#include "libc.h"
#include "stdatomic_impl.h"
#ifdef __HISPARK_LINUX__
#include "../../ldso/namespace.h"
#else
#include "../../ldso/linux/namespace.h"
#endif
#endif

#include "ADLTSection.h"

#if UINTPTR_MAX == 0xffffffff
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
typedef Elf32_Sym Sym;
#ifndef __LITEOS__
typedef Elf32_Shdr Shdr;
typedef Elf32_Verdaux Verdaux;
typedef Elf32_Verdef Verdef;
typedef Elf32_Vernaux Vernaux;
typedef Elf32_Verneed Verneed;
#endif
#define R_TYPE(x) ((x)&255)
#define R_SYM(x) ((x)>>8)
#define R_INFO ELF32_R_INFO
#else
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
typedef Elf64_Sym Sym;
#ifndef __LITEOS__
typedef Elf64_Shdr Shdr;
typedef Elf64_Verdaux Verdaux;
typedef Elf64_Verdef Verdef;
typedef Elf64_Vernaux Vernaux;
typedef Elf64_Verneed Verneed;
#endif
#define R_TYPE(x) ((x)&0x7fffffff)
#define R_SYM(x) ((x)>>32)
#define R_INFO ELF64_R_INFO
#endif

/* These enum constants provide unmatchable default values for
 * any relocation type the arch does not use. */
enum {
	REL_NONE = 0,
	REL_SYMBOLIC = -100,
	REL_USYMBOLIC,
	REL_GOT,
	REL_PLT,
	REL_RELATIVE,
	REL_OFFSET,
	REL_OFFSET32,
	REL_COPY,
	REL_SYM_OR_REL,
	REL_DTPMOD,
	REL_DTPOFF,
	REL_TPOFF,
	REL_TPOFF_NEG,
	REL_TLSDESC,
	REL_FUNCDESC,
	REL_FUNCDESC_VAL,
	REL_AUTH_SYMBOLIC,
	REL_AUTH_RELATIVE,
	REL_AUTH_GLOB_DAT,
};
#ifndef __LITEOS__
struct td_index {
	size_t args[2];
	struct td_index *next;
};

struct verinfo {
	const char *s;
	const char *v;
	bool use_vna_hash;
	uint32_t vna_hash;
};

struct sym_info_pair {
	uint_fast32_t sym_h;
	uint32_t sym_l;
};

struct cfi_modifier {
	size_t offset;
	size_t modifier;
};

struct icall_item {
	_Atomic(uint64_t) pc_check;
	int valid;
	size_t base;
	size_t modifier_begin;
	size_t modifier_end;
};

struct unpack_reloc {
	void *map;            // map address
	size_t map_len;       // map length
	size_t unpack_num;    // unpacking reloc count
	off_t unpack_offset;  // unpacking procedure address offset
};
 
struct adlt_got_entry{
	unsigned char *entry_addr;        // .got section start address
	size_t entry_len;                 // .got section size
	uint32_t *record_entry;           // Whether the entry in the got table has been relocated
	size_t  got_map_len;
	uint8_t *got_map;                // got/got.plt map
	size_t *got_entry;                // initial got/got.plt entry
};
 
struct bolt_remap_data{
    size_t new_addr;
    size_t old_addr; 
};
 
struct adlt {
	unsigned char *base;
	char *name;                       // real path
	uint64_t file_offset;             // > 0 when opening library from zip file; PAGE_SIZE aligned
	bool partly_load;                 // true when nDSOs are loaded separately
	unsigned char *map;               // map address
	size_t map_len;                   // map length
	size_t addr_min;                  // min address (mapping result)
	bool hugepage_enabled;            // hugepage (mapping result)
	uint32_t npsod_load;
	unsigned char *sa_map;            // .adlt section map address
	size_t sa_map_len;                // .adlt section map size
	unsigned char *sa_addr;           // .adlt section address
	struct adlt_got_entry gotEntry;
	struct adlt_got_entry gotPltEntry;
	unsigned char *strtab_map;        // .adlt.strtab section map address
	size_t strtab_map_len;            // .adlt.strtab section map size
	unsigned char *strtab_addr;       // .adlt.strtab section address
	unsigned char *bolt_remap;        // .bolt.remap section map address
	size_t bolt_remap_len;            // .bolt.remap section map size
	unsigned char *bolt_remap_addr;       // .bolt.remap section address
	dev_t dev;                        //
	ino_t ino;                        //   
	off_t file_size;                  //
	Phdr *phdr;                       // PH table address 
	int phnum;                        // PH count
	size_t phentsize;                 // PH size
	struct unpack_reloc android_rel;  // unpacked android rel relocs (SHT_ANDROID_REL type section)
	struct unpack_reloc android_rela; // unpacked android rela relocs (SHT_ANDROID_RELA type section)
	struct unpack_reloc relr_rel;     // unpacked relr relocs (SHT_RELR type section)
	size_t relro_start, relro_end;    // remove
	uint8_t *sym_dso_Idx_map;         // symbol index map to adlt_ndso_index 
	uint32_t nsym;                    // symbol count
	/* symbol section (.symtab) mapping for backtrace symbol lookup */
	unsigned char *bsl_syms_map;      // map address
	size_t bsl_syms_map_len;          // map size
	Sym *bsl_syms;                    // section address (symbols)
	/* string section (.strtab) mapping for backtrace symbol lookup */
	unsigned char *bsl_strings_map;   // map address
	size_t bsl_strings_map_len;       // map size
	char *bsl_strings;                // section address (strings)
	size_t bsl_strings_sec_index;     // section index
	uint64_t pgbo_addr_start;         // pgbo text section sh_addr
	uint64_t pgbo_addr_end;           // pgbo text section end sh_addr
	struct adlt *next;				  // next adlt
	struct adlt *prev;                // prev adlt
};
 
struct adlt_phdr_info {
    char *map;
    size_t map_len;
    char *map_ph_addr;
    struct adlt *map_adlt;
    size_t ph_ndso_index;
    int map_prot;
};
 
struct adlt_dso_sym_index {
	uint32_t *idx;
	uint64_t size;
};

struct dso {
#if DL_FDPIC
	struct fdpic_loadmap *loadmap;
#else
	unsigned char *base;
#endif
	char *name;
	size_t *dynv;
	struct dso *next, *prev;
	/* add namespace */
	ns_t *namespace;
	int cache_sym_index;
	struct dso *cache_dso;
	Sym *cache_sym;
	Phdr *phdr;
	int phnum;
	size_t phentsize;
	Sym *syms;
	Elf_Symndx *hashtab;
	uint32_t *ghashtab;
	int16_t *versym;
	Verdef *verdef;
	Verneed *verneed;
	char *strings;
	struct dso *syms_next, *lazy_next;
	size_t modifier_begin;
	size_t modifier_end;
	struct icall_item *item;
	size_t *lazy, lazy_cnt;
	unsigned char *map;
	size_t map_len;
	dev_t dev;
	ino_t ino;
	uint64_t file_offset;
	pthread_t ctor_visitor;
	char *rpath_orig, *rpath;
	struct tls_module tls;
	size_t tls_id;
	size_t relro_start, relro_end;
	uintptr_t *new_dtv;
	unsigned char *new_tls;
	struct td_index *td_index;
	struct dso *fini_next;
	char *shortname;
#if DL_FDPIC
	unsigned char *base;
#else
	struct fdpic_loadmap *loadmap;
#endif
	struct funcdesc {
		void *addr;
		size_t *got;
	} *funcdescs;
	size_t *got;
	struct dso **deps, *needed_by;
	/* only assigned when a thread local destructor is added */
	struct dso **deps_all;
	uint16_t ndeps_direct;
	uint16_t next_dep;
	uint16_t parents_count;
	uint16_t parents_capacity;
	struct dso **parents;
	struct dso **reloc_can_search_dso_list;
	uint16_t reloc_can_search_dso_count;
	uint16_t reloc_can_search_dso_capacity;
	/* mark the dso status */
	uint32_t flags;
	uint32_t nr_dlopen;

	struct adlt *adlt;
	ssize_t adlt_ndso_index;
	char *phdr_map;
	uint32_t phdr_count;
	size_t init_array_off;
	size_t fini_array_off;
	dev_t ldev; // lstat() dev 
	ino_t lino; // lstat() ino
	char* adlt_ndso_name;
	struct adlt_dso_sym_index sym_idx;

	bool is_global;
	bool is_preload;
	bool is_reloc_head_so_dep;
	char relocated;
	char constructed;
	char kernel_mapped;
	char mark;
	char bfs_built;
	char deps_all_built;
	char runtime_loaded;
	char by_dlopen;
	bool is_mapped_to_shadow;
	struct dso_debug_info *debug_info;
	/* do not add new elements after buf[]*/
	char buf[];
};

struct dso_debug_info {
#if DL_FDPIC
	struct fdpic_loadmap *loadmap;
#else
	unsigned char *base;
#endif
	char *name;
	size_t *dynv;
	struct dso_debug_info *next, *prev;
};

struct symdef {
	Sym *sym;
	struct dso *dso;
};

struct dlopen_time_info {
	int entry_header_time;
	int deps_header_time;
	int map_so_time;
	int reloc_time;
	int map_cfi_time;
	int init_time;
	int total_time;
};
#endif

struct notify_dso {
	struct dso **dso_list;
	size_t capacity;
	size_t length;
};

struct fdpic_loadseg {
	uintptr_t addr, p_vaddr, p_memsz;
};

struct fdpic_loadmap {
	unsigned short version, nsegs;
	struct fdpic_loadseg segs[];
};

struct fdpic_dummy_loadmap {
	unsigned short version, nsegs;
	struct fdpic_loadseg segs[1];
};

#include "reloc.h"

#ifndef FDPIC_CONSTDISP_FLAG
#define FDPIC_CONSTDISP_FLAG 0
#endif

#ifndef DL_FDPIC
#define DL_FDPIC 0
#endif

#ifndef DL_NOMMU_SUPPORT
#define DL_NOMMU_SUPPORT 0
#endif

#ifndef TLSDESC_BACKWARDS
#define TLSDESC_BACKWARDS 0
#endif

#if !DL_FDPIC
#define IS_RELATIVE(x,s) ( \
	(R_TYPE(x) == REL_RELATIVE) || \
	(R_TYPE(x) == REL_SYM_OR_REL && !R_SYM(x)) )
#else
#define IS_RELATIVE(x,s) ( ( \
	(R_TYPE(x) == REL_FUNCDESC_VAL) || \
	(R_TYPE(x) == REL_SYMBOLIC) ) \
	&& (((s)[R_SYM(x)].st_info & 0xf) == STT_SECTION) )
#endif

#ifndef NEED_MIPS_GOT_RELOCS
#define NEED_MIPS_GOT_RELOCS 0
#endif

#ifndef DT_DEBUG_INDIRECT
#define DT_DEBUG_INDIRECT 0
#endif

#ifndef DT_DEBUG_INDIRECT_REL
#define DT_DEBUG_INDIRECT_REL 0
#endif

#define AUX_CNT 32
#ifndef __LITEOS__
#define DYN_CNT 37

#define DT_ANDROID_REL (DT_LOOS + 2)
#define DT_ANDROID_RELSZ (DT_LOOS + 3)

#define DT_ANDROID_RELA (DT_LOOS + 4)
#define DT_ANDROID_RELASZ (DT_LOOS + 5)

#define DT_AARCH64_AUTH_RELRSZ 0x70000011
#define DT_AARCH64_AUTH_RELR 0x70000012
#define DT_AARCH64_AUTH_RELRENT 0x70000013
#define APIA 0
#define APIB 1
#define APDA 2
#define APDB 3

#define ANDROID_REL_SIGN_SIZE 4

#define RELOCATION_GROUPED_BY_INFO_FLAG 1
#define RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG 2
#define RELOCATION_GROUPED_BY_ADDEND_FLAG 4
#define RELOCATION_GROUP_HAS_ADDEND_FLAG 8

#define CLOCK_NANO_TO_MILLI 1000000
#define CLOCK_SECOND_TO_MILLI 1000
#define DLOPEN_TIME_THRESHOLD 1000

typedef void (*stage2_func)(unsigned char *, size_t *);

#if DL_FDPIC
void *laddr(const struct dso *p, size_t v);
#endif

#ifdef UNIT_TEST_STATIC
    #define UT_STATIC
#else
    #define UT_STATIC static
#endif

void *addr2dso(size_t a);
UT_STATIC size_t count_syms(struct dso *p);
struct sym_info_pair gnu_hash(const char *s0);
struct symdef find_sym_impl(
	struct dso *dso, struct verinfo *verinfo, struct sym_info_pair s_info_p, int need_def, ns_t *ns);

hidden void *__dlsym(void *restrict, const char *restrict, void *restrict);
hidden void *__dlvsym(void *restrict, const char *restrict, const char *restrict, void *restrict);
hidden int __dlclose(void *p);

hidden ssize_t get_adlt_library_ph(struct adlt *adlt, ssize_t library_index, adlt_phindex_t **ph_indexes);
hidden ssize_t get_adlt_common_ph(struct adlt *adlt, adlt_phindex_t **ph_indexes);

#else
#define DYN_CNT 37

typedef void (*stage2_func)(unsigned char *, size_t *);

hidden void *__dlsym(void *restrict, const char *restrict, void *restrict);
#endif
hidden void __dl_seterr(const char *, ...);
hidden int __dl_invalid_handle(void *);
hidden void __dl_vseterr(const char *, va_list);
typedef void(*notify_call)(uintptr_t dso_base, size_t dso_len, const char *dso_path);
int register_ldso_func_for_add_dso(notify_call callback);
hidden ptrdiff_t __tlsdesc_static(), __tlsdesc_dynamic();

hidden extern int __malloc_replaced;
hidden extern int __aligned_alloc_replaced;
hidden void __malloc_donate(char *, char *);
hidden int __malloc_allzerop(void *);

#endif
