// feature ADLT segment type
#define PT_ADLT                  0x6788FC61

#if UINTPTR_MAX == 0xffffffff
typedef Elf32_Off adlt_index_t;
#else
// feature ADLT index in section type
typedef Elf64_Off adlt_index_t;
#endif

// // feature ADLT section .rela.dyn/plt index type
// typedef uint32_t adlt_relindex_t;
// // feature ADLT section index type
// typedef uint32_t adlt_secindex_t;
#define ADLT_MAX_NSO              (32)
#define BITS_IN_BYTE 8
#define ADLT_UPALIGN(x, div) (((x) + (div) - 1) / (div))

typedef enum {
    FIRST_RELOCATED,  
    SECOND_RELOCATED,
	NOT_GOT
} Relocated_Flag;

static struct adlt *g_adlt_ptr = NULL;

#if defined(__aarch64__)

/* feature ADLT function */
static struct dso *find_library_by_adlt_lstat(
    const struct stat *st, const ns_t *ns, bool check_inherited, uint64_t file_offset);
static struct dso *find_library_by_adlt_index(
    const struct adlt *adlt, const ns_t *ns, bool check_inherited, ssize_t library_index);
static void free_adlt(struct adlt *adlt);
static void init_adlt(struct adlt *adlt);
static void unmap_adlt_library(struct dso *dso);
ssize_t get_adlt_common_ph(struct adlt *adlt, adlt_phindex_t **ph_indexes);
ssize_t get_adlt_sym_dso_map(struct adlt *adlt, uint8_t **sym_dso_idx_map);
static ssize_t get_adlt_library_index(unsigned char *strtab_addr, unsigned char *sa_addr, const char *pathname);
static ssize_t get_adlt_library_index2(struct adlt *adlt, const char *pathname);
static adlt_psod_t *get_adlt_library_entry(struct adlt *adlt, ssize_t library_index, char **blob);
ssize_t get_adlt_library_ph(struct adlt *adlt, ssize_t library_index, adlt_phindex_t **ph_indexes);
static ssize_t get_adlt_library_dt_needed(struct adlt *adlt, ssize_t library_index, adlt_dt_needed_index_t **dt_needed);
static ssize_t get_adlt_library_rela_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_dyn);
static ssize_t get_adlt_library_rela_plt(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_plt);
static ssize_t get_adlt_library_relr_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **relr_dyn);
static size_t get_adlt_library_init_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **init_cross);
static inline bool adlt_ndso_index_check(ssize_t ndso_index) {
	if (ndso_index < 0 || ndso_index >= ADLT_MAX_NSO) {
		return false;
	}
	return true;
}
static void adlt_clean_gotentry(struct dso *p, struct adlt *adlt);
static size_t get_adlt_library_fini_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **fini_cross);
static adlt_secindex_t get_adlt_symbol_sec_index(struct adlt *adlt);
static ssize_t get_adlt_objects_num(struct adlt *adlt);
static ssize_t merge_sort_arrays(adlt_phindex_t **ph_indexes, ssize_t ph_count, 
		adlt_phindex_t **phl_indexes, ssize_t phl_count, 
		adlt_phindex_t **phc_indexes, ssize_t phc_count);
static bool replace_to_adlt_duplicate(struct dso *dso, Sym **sym, const char **name);
static bool replace_to_adlt_duplicate2(struct dso *dso, Sym **sym, const char **name);
static bool is_adlt_partly_load();
static bool is_adlt_dso_sym(struct dso *dso, uint32_t idx);
static void add_adlt(struct adlt* adlt);
static void del_adlt_from_gadlt(struct adlt* adlt);
static bool is_same_adlt(struct adlt* adlt, struct stat* st);
static struct adlt* find_adlt_by_fstat(struct stat* st);
static char *create_realpath_from_fd(int fd);


static struct dso *search_dso_by_adlt_lstat(const struct stat *st, const ns_t *ns, uint64_t file_offset)
{
	LD_LOGD("search_dso_by_adlt_lstat ns_name:%{public}s", ns ? ns->ns_name : "NULL");
	for (size_t i = 0; i < ns->ns_dsos->num; i++){
		struct dso *p = ns->ns_dsos->dsos[i];
		if (p->adlt && p->ldev == st->st_dev && p->lino == st->st_ino) {
			LD_LOGD("search_dso_by_adlt_lstat found dev:%{public}lu, ino:%{public}lu, ns_name:%{public}s",
				st->st_dev, st->st_ino, ns ? ns->ns_name : "NULL");
			return p;
		}
	}
	return NULL;
}


static struct dso *find_library_by_adlt_lstat(const struct stat *st, const ns_t *ns, bool check_inherited, uint64_t file_offset)
{
	LD_LOGD("find_library_by_adlt_lstat ns_name:%{public}s, check_inherited:%{public}d",
		ns ? ns->ns_name : "NULL",
		!!check_inherited);
	struct dso *p = search_dso_by_adlt_lstat(st, ns, file_offset);
	if (p) return p;
	if (check_inherited && ns->ns_inherits) {
		for (size_t i = 0; i < ns->ns_inherits->num; i++) {
			ns_inherit *inherit = ns->ns_inherits->inherits[i];
			p = search_dso_by_adlt_lstat(st, inherit->inherited_ns, file_offset);
			if (p && is_sharable(inherit, p->shortname)) return p;
		}
	}
	return NULL;
}

static struct dso *search_dso_by_adlt_index(const struct adlt *adlt, const ns_t *ns, ssize_t library_index)
{
	LD_LOGD("search_dso_by_adlt_index ns_name:%{public}s", ns ? ns->ns_name : "NULL");
	if (adlt && ns && library_index >= 0) {
		for (size_t i = 0; i < ns->ns_dsos->num; i++){
			struct dso *p = ns->ns_dsos->dsos[i];
			if (p->adlt == adlt && p->adlt_ndso_index == library_index) {
				LD_LOGD("search_dso_by_adlt_index found index:%{public}ld, ns_name:%{public}s",
					library_index, ns->ns_name);
				return p;
			}
		}
	}
	return NULL;
}

static struct dso *find_library_by_adlt_index(
    const struct adlt *adlt, const ns_t *ns, bool check_inherited, ssize_t library_index)
{
    LD_LOGD("find_library_by_adlt_index ns_name:%{public}s, check_inherited:%{public}d, library_index:%{public}ld",
		ns ? ns->ns_name : "NULL", !!check_inherited, library_index);
	if (adlt && ns && library_index >= 0) {
		struct dso *p = search_dso_by_adlt_index(adlt, ns, library_index);
		if (p) return p;
		if (!check_inherited || !ns->ns_inherits) {
			return NULL;
		}
		for (size_t i = 0; i < ns->ns_inherits->num; i++) {
			ns_inherit *inherit = ns->ns_inherits->inherits[i];
			p = search_dso_by_adlt_index(adlt, inherit->inherited_ns, library_index);
			if (p && is_sharable(inherit, p->shortname)) return p;
		}
	}
	return NULL;
}

static void init_adlt(struct adlt *adlt)
{
	if (adlt) {
		adlt->map = MAP_FAILED;
		adlt->sa_map = MAP_FAILED;
		adlt->strtab_map = MAP_FAILED;
		adlt->android_rel.map = MAP_FAILED;
		adlt->android_rela.map = MAP_FAILED;
		adlt->relr_rel.map = MAP_FAILED;
		adlt->bsl_syms_map = MAP_FAILED;
		adlt->bsl_strings_map = MAP_FAILED;
		adlt->name = NULL;
		adlt->npsod_load = 0;
		adlt->sym_dso_Idx_map = NULL;
	}
}

static void free_record_entry(struct adlt_got_entry *entry)
{
	if (entry->record_entry != NULL) {
		free(entry->record_entry);
		entry->record_entry = NULL;
	}
	if (entry->got_map != MAP_FAILED) {
		munmap(entry->got_map, entry->got_map_len);
		entry->got_map = NULL;
	}
}

static void free_adlt(struct adlt *adlt)
{
	if (!adlt) {
		return;
	}
	if (adlt->sa_map != MAP_FAILED) {
		munmap(adlt->sa_map, adlt->sa_map_len);
	}
	if (adlt->strtab_map != MAP_FAILED) {
		munmap(adlt->strtab_map, adlt->strtab_map_len);
	}
	if (adlt->map != MAP_FAILED) {
		munmap(adlt->map, adlt->map_len);
    }
	if (adlt->android_rel.map != MAP_FAILED) {
		munmap(adlt->android_rel.map, adlt->android_rel.map_len);
	}
	if (adlt->android_rela.map != MAP_FAILED) {
		munmap(adlt->android_rela.map, adlt->android_rela.map_len);
	}
	if (adlt->relr_rel.map != MAP_FAILED) {
		munmap(adlt->relr_rel.map, adlt->relr_rel.map_len);
	}
	if (adlt->bsl_syms_map != MAP_FAILED) {
		munmap(adlt->bsl_syms_map, adlt->bsl_syms_map_len);
	}
	if (adlt->bsl_strings_map != MAP_FAILED) {
		munmap(adlt->bsl_strings_map, adlt->bsl_strings_map_len);
	}
	if (adlt->bolt_remap != MAP_FAILED) {
		munmap(adlt->bolt_remap, adlt->bolt_remap_len);
	}
	free_record_entry(&adlt->gotEntry);
	free_record_entry(&adlt->gotPltEntry);
	del_adlt_from_gadlt(adlt);
	free(adlt->name);
	free(adlt);
}

static void unmap_adlt_library(struct dso *dso)
{
	if (!dso) {
		return;
	}
	struct adlt *adlt = dso->adlt;
	adlt_clean_gotentry(dso, adlt);
	adlt->npsod_load--;
	dso->adlt = NULL;
	if (adlt->npsod_load > 0) {
		return;
	}

	if (!is_dlclose_debug_enable()) {
		free_adlt(adlt);
		dso->adlt = NULL;
	} else {
		if (adlt->sa_map != MAP_FAILED) {
			mprotect(adlt->sa_map, adlt->sa_map_len, PROT_NONE);
		}
		if (adlt->strtab_map != MAP_FAILED) {
			mprotect(adlt->strtab_map, adlt->strtab_map_len, PROT_NONE);	
		}
		if (adlt->map != MAP_FAILED) {
			mprotect(adlt->map, adlt->map_len, PROT_NONE);
		}
		if (adlt->android_rel.map != MAP_FAILED) {
			mprotect(adlt->android_rel.map, adlt->android_rel.map_len, PROT_NONE);
		}
		if (adlt->android_rela.map != MAP_FAILED) {
			mprotect(adlt->android_rela.map, adlt->android_rela.map_len, PROT_NONE);
		}
		if (adlt->relr_rel.map != MAP_FAILED) {
			mprotect(adlt->relr_rel.map, adlt->relr_rel.map_len, PROT_NONE);
		}
		if (adlt->bsl_syms_map != MAP_FAILED) {
			mprotect(adlt->bsl_syms_map, adlt->bsl_syms_map_len, PROT_NONE);
		}
		if (adlt->bsl_strings_map != MAP_FAILED) {
			mprotect(adlt->bsl_strings_map, adlt->bsl_strings_map_len, PROT_NONE);
		}
		if (adlt->bolt_remap != MAP_FAILED) {
			mprotect(adlt->bolt_remap, adlt->bolt_remap_len, PROT_NONE);
		}
		if (adlt->gotEntry.got_map != MAP_FAILED) {
			mprotect(adlt->gotEntry.got_map, adlt->gotEntry.got_map_len, PROT_NONE);
		}
		if (adlt->gotPltEntry.got_map != MAP_FAILED) {
			mprotect(adlt->gotPltEntry.got_map, adlt->gotPltEntry.got_map_len, PROT_NONE);
		}
		free(adlt);
	}
}

/*
   next_link_name() - get a basename for next hop in chain of links
   Input:
       fullpath - fullpath to current file (or link)
       buf, bufsize - caller's buffer and bufsize for readlink result
   Return:
       pointer to basename of resolved link (fullpath points to resolved object)
       NULL - error or not a link (fullpath not changed)
*/

char *next_link_name(char **fullpath, char *buf, ssize_t bufsize)
{
	ssize_t ret;
	char *p;
	ret = readlink(*fullpath, buf, bufsize - 1);
	if (ret < 0) {
		return NULL;
	}

	*fullpath = buf;
	buf[ret] = '\0';
	p = buf + ret;
	while (p != buf) {
		if (*p == '/') {
			return p + 1;
		}
		p--;
	}
	if (*p == '/') {
		p++;
	}
	return p;
}

static ssize_t get_adlt_library_index(unsigned char *strtab_addr, unsigned char *sa_addr, const char *pathname)
{
	ssize_t i;
	char *soname;
	char *fullpath;
	char *psod_soname = NULL;
	char *psod_sofilename = NULL;
	adlt_psod_t *psod = NULL;
	adlt_section_header_t *sah = (adlt_section_header_t*)sa_addr;
	char block[PATH_MAX]; // PATH_MAX from limits.h == block size

	soname = (char*)pathname;
	// get soname - lib.so
	for (i = 0; i < strlen(pathname); i++) {
		if (pathname[i] == '/') {
			soname = (char *)&pathname[i + 1];
		}
	}
	// loop deals with case when libname != soname
	fullpath = (char*) pathname;
	do {
	    // index search
	    for (i = 0; i < sah->sharedObjectsNum; i++) {
		psod = (adlt_psod_t*)(sa_addr + sah->schemaHeaderSize + (i * sah->schemaPSODSize));
		psod_soname = (char*)(strtab_addr + psod->soName);
		psod_sofilename = (char*)(strtab_addr + psod->soFileName);
		if (strcmp(psod_soname, soname) == 0)
		  return i;
		if (strcmp(psod_sofilename, soname) == 0)
		  return i;
		}
	} while ((soname = next_link_name(&fullpath, block, sizeof(block))) != NULL);

	return -1;
}

static ssize_t get_adlt_library_index2(struct adlt *adlt, const char *pathname)
{
	return adlt ? get_adlt_library_index(adlt->strtab_addr, adlt->sa_addr, pathname) : -1;
}

static adlt_psod_t *get_adlt_library_entry(struct adlt *adlt, ssize_t library_index, char **blob)
{
    if (!adlt || (library_index < 0)) {
        if (blob) {
            *blob = NULL;
        }
        return NULL;
    }
    adlt_section_header_t *sah = (void *)adlt->sa_addr;
    if (!sah || (library_index >= sah->sharedObjectsNum)) {
        if (blob) {
            *blob = NULL;
        }
        return NULL;
    }
    if (blob) {
        *blob = (char *)adlt->sa_addr + sah->blobStart;
    }
    return (void *)((char *)adlt->sa_addr + sah->schemaHeaderSize + library_index * sah->schemaPSODSize);
}

ssize_t get_adlt_common_ph(struct adlt *adlt, adlt_phindex_t **ph_indexes)
{
	if (!adlt) {
		if (ph_indexes) *ph_indexes = NULL;
		return -1;
	}
	adlt_section_header_t *sah = (void *)adlt->sa_addr;
	if (!sah) {
		if (ph_indexes) *ph_indexes = NULL;
		return -1;
	}
	char *blob = (void *)((char *)adlt->sa_addr + sah->blobStart);
	if (ph_indexes)
		*ph_indexes = (void *)((char *)blob + sah->phIndexes.offset);
	return sah->phIndexes.size / sizeof(adlt_phindex_t);
}

ssize_t get_adlt_library_ph(struct adlt *adlt, ssize_t library_index, adlt_phindex_t **ph_indexes)
{
	char *blob;
	adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, &blob);
	if (!psod) {
		if (ph_indexes) *ph_indexes = NULL;
		return -1;			
	}
	if (ph_indexes) {
		*ph_indexes = (void *)((char *)blob + psod->phIndexes.offset);
	}
	return psod->phIndexes.size / sizeof(adlt_phindex_t);
}

ssize_t get_adlt_sym_dso_map(struct adlt *adlt, uint8_t **sym_dso_idx_map) {
	if (!adlt) {
		if (sym_dso_idx_map) *sym_dso_idx_map = NULL;
		return -1;
	}
	adlt_section_header_t *sah = (void *)adlt->sa_addr;
	if (!sah) {
		if (sym_dso_idx_map) *sym_dso_idx_map = NULL;
		return -1;
	}
	char *blob = (void *)((char *)adlt->sa_addr + sah->blobStart);
	if (sym_dso_idx_map)
		*sym_dso_idx_map = (void *)((char *)blob + sah->symIdxToSoIdx.offset);
	return sah->symIdxToSoIdx.size / sizeof(uint8_t);
}

static ssize_t get_adlt_library_dt_needed(struct adlt *adlt, ssize_t library_index, adlt_dt_needed_index_t **dt_needed)
{
    char *blob;
	adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, &blob);
	if (!psod) {
		if (dt_needed) *dt_needed = NULL;
		return -1;			
	}
	if (dt_needed)
		*dt_needed = (void *)((char *)blob + psod->dtNeeded.offset);
	return psod->dtNeeded.size / sizeof(adlt_dt_needed_index_t);
}

static ssize_t get_adlt_library_rela_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_dyn)
{
	char *blob;
	adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, &blob);
	if (!psod) {
        if (rela_dyn) {
            *rela_dyn = NULL;
        }
        return 0;
	}
	if (rela_dyn)
		*rela_dyn = (void *)((char *)blob + psod->relaDynIndx.offset);
	return psod->relaDynIndx.size / sizeof(adlt_relindex_t);
}

static ssize_t get_adlt_library_rela_plt(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_plt)
{
    char *blob;
    adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, &blob);
    if (!psod) {
        if (rela_plt) {
            *rela_plt = NULL;
        }
        return 0;
    }
    if (rela_plt) {
        *rela_plt = (void *)((char *)blob + psod->relaPltIndx.offset);
    }
    return psod->relaPltIndx.size / sizeof(adlt_relindex_t);
}

static bool replace_to_adlt_duplicate(struct dso *dso, Sym **sym, const char **name){
	char *name_idx;
	Sym *sym_idx;
	struct dso *p;
	ssize_t idx;

	// Skip dso without dependencies
	if (!(dso->deps)){
		return false;
	}

	// Search dyplicate symbol in adlt lib.
	name_idx = malloc((strlen(*name) + 10) * sizeof(char));
	for (int i = 0; dso->deps[i]; i++) {
		p = dso->deps[i];
        if (__predict_true(!(p->adlt))) {
            continue;
        }

        idx = p->adlt_ndso_index;
        if (idx == -1) {
            continue;
        }

        // Get name with postfix: name + __ + hex(order index)
	    (void)sprintf(name_idx, "%s__%x", *name, idx);
		sym_idx = find_sym(p, name_idx, 1).sym;
		if (sym_idx) {
			*sym = sym_idx;
			*name = name_idx;
			return true;
		}
	}
	free(name_idx);
	return false;
}

static bool replace_to_adlt_duplicate2(struct dso *dso, Sym **sym, const char **name)
{
    // Skip found or aren't global symbols.
    if (!(dso->deps) || (((*sym)->st_info) >> 4) != STB_GLOBAL || find_sym(dso, *name, 1).sym) {
        return false;
    }
    for (int i = 0; dso->deps[i]; i++) {
        if (find_sym(dso->deps[i], *name, 1).sym) {
            return false;
        }
    }
    // Search duplicate symbol in adlt lib.
    return replace_to_adlt_duplicate(dso, sym, name);
}

static ssize_t get_adlt_library_relr_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **relr_dyn)
{
    char *blob;
    adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, &blob);
    if (!psod) {
        if (relr_dyn) {
            *relr_dyn = NULL;
        }
        return 0;
    }
    if (relr_dyn) {
        *relr_dyn = (void *)((char *)blob + psod->relrDynIndx.offset);
    }
    return psod->relrDynIndx.size / sizeof(adlt_relindex_t);
}

static size_t get_adlt_library_init_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **init_cross)
{
    adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, NULL);
	if (!psod) {
		if (init_cross) *init_cross = NULL;
		return -1;			
	}
	if (init_cross)
		*init_cross = (void *)&psod->initArray;
	return psod->initArray.size / sizeof(size_t);
}

static size_t get_adlt_library_fini_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **fini_cross)
{
    adlt_psod_t *psod = get_adlt_library_entry(adlt, library_index, NULL);
    if (!psod) {
        if (fini_cross) {
            *fini_cross = NULL;
        }
        return -1;
    }
    if (fini_cross) {
        *fini_cross = (void *)&psod->finiArray;
    }

    return psod->finiArray.size / sizeof(size_t);
}

static void adlt_reclaim_gaps(struct dso *dso)
{
	Phdr *phdr = dso->phdr;
	if (!phdr) {
		return;
	}
	size_t entsz = dso->phentsize;
	adlt_phindex_t *ph_indexes, *phc_indexes, *phl_indexes;
	ssize_t phl_count = get_adlt_library_ph(dso->adlt, dso->adlt_ndso_index, &phl_indexes);
	ssize_t phc_count = get_adlt_common_ph(dso->adlt, &phc_indexes);
	int loop = 2;
	Phdr *ph;
	ssize_t ph_num;
	size_t i, q;
	while(loop--) {
		if (loop) {
			if (phc_count <= 0 || !phc_indexes) {
				continue;
			}
			ph_indexes = phc_indexes;
			ph_num = phc_count;
			i = ph_indexes[0];
		} else {
			if (phl_count < 0 || !phl_indexes) {
				continue;
			}
			ph_indexes = phl_indexes;
			ph_num = phl_count;
			i = ph_indexes[0];
		}

		q = 0;
		for (ph = (void *)((char *)phdr + i * entsz); ph_num;
	  	  ph_num--, i = (size_t)ph_indexes[++q], ph = (void *)((char *)phdr + i * entsz)) {
			if (ph->p_type != PT_LOAD) continue;
			if ((ph->p_flags & (PF_R | PF_W)) != (PF_R | PF_W)) continue;
			reclaim(dso, ph->p_vaddr & -PAGE_SIZE, ph->p_vaddr);
			reclaim(dso, ph->p_vaddr + ph->p_memsz,
			  ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1 & -PAGE_SIZE);
		}
	}
}

static void get_adlt_symindex_map_to_library(struct dso *p)
{
	struct adlt *adlt = p->adlt;
	if (!adlt) {
		return;
	}
	size_t nsym = 0;
    if (adlt->sym_dso_Idx_map) {
		return;
	}
	struct adlt_dso_sym_index *sym_idx = &p->sym_idx;
	ssize_t sym_so_map_count = get_adlt_sym_dso_map(adlt, &adlt->sym_dso_Idx_map);
	if (sym_so_map_count > sym_idx->size) {
		LD_LOGE("sym_so_map_count not match");
		return;
	}
	return;
}

/* 
 * ADLT
 */
static adlt_secindex_t get_adlt_symbol_sec_index(struct adlt *adlt)
{
	if (adlt && adlt->sa_addr) {
		adlt_section_header_t *sah = (void *)adlt->sa_addr;
		return (adlt_secindex_t)sah->sharedEndGlobalSymbolIndex.secIndex;
	} 
	return (adlt_secindex_t)-1;
}

static ssize_t get_adlt_objects_num(struct adlt *adlt)
{
	if (adlt) {
		adlt_section_header_t *sah = (void *)adlt->sa_addr;
		if (sah) {
			return (ssize_t)sah->sharedObjectsNum;
		}
	}
	return -1;
}

static bool is_adlt_dso_sym(struct dso *dso, uint32_t idx)
{
	// struct adlt_dso_sym_index *sym_idx;
	// uint64_t i;

    if (__predict_true(!dso->adlt)) {
        return true;
    }
    return dso->adlt->sym_dso_Idx_map[idx] == (uint8_t)dso->adlt_ndso_index;
}

static void add_adlt(struct adlt *adlt)
{
    if (!g_adlt_ptr) {
        g_adlt_ptr = adlt;
    } else {
        for (struct adlt *adlt_ptr = g_adlt_ptr; adlt_ptr; adlt_ptr = adlt_ptr->next) {
            if (!adlt_ptr->next) {
          adlt_ptr->next = adlt;
          adlt->prev = adlt_ptr;
          break;
            }
        }
    }
}

static void del_adlt_from_gadlt(struct adlt *adlt)
{
	if (!adlt) {
		return;
	}
	if (adlt == g_adlt_ptr) {
		g_adlt_ptr = adlt->next;
		if (g_adlt_ptr) {
			g_adlt_ptr->prev = NULL;
		}
	} else {
		if (adlt->prev) {
			adlt->prev->next = adlt->next;
		}
		if (adlt->next) {
			adlt->next->prev = adlt->prev;
		}
	}
	return;
}

static bool is_same_adlt(struct adlt* adlt, struct stat* st)
{
	return (adlt->dev == st->st_dev && adlt->ino == st->st_ino && adlt->file_size == st->st_size);
}
 
static struct adlt* find_adlt_by_fstat(struct stat* st)
{
	struct adlt* adlt;
	for (adlt = g_adlt_ptr; adlt; adlt = adlt->next) {
		if (is_same_adlt(adlt, st)) {
			return adlt;
		}
	}
	return NULL;
}


static Relocated_Flag check_and_set_bit(struct adlt_got_entry *et, size_t reloc_addr , struct dso *dso)
{
	if (!et->entry_addr) {
        return NOT_GOT;
    }
	ssize_t ndso_index = dso->adlt_ndso_index;
	uintptr_t reloc_ptr = (uintptr_t)reloc_addr;
	uintptr_t got_start = (uintptr_t)et->entry_addr;
	uintptr_t got_end = got_start + (et->entry_len * sizeof(unsigned char));
	size_t *reloc_va = (size_t*)laddr(dso, reloc_addr);
	if ((reloc_ptr >= got_start) && (reloc_ptr < got_end))
	{
		size_t index;
		index = (reloc_ptr - (uintptr_t)et->entry_addr) / sizeof(size_t);
		if (et->record_entry[index] == 0) {
			et->record_entry[index] |= (1UL << ndso_index);
			*reloc_va = et->got_entry[index];
			return FIRST_RELOCATED;
		}
		et->record_entry[index] |= (1UL << ndso_index);
		return SECOND_RELOCATED;
	}
	return NOT_GOT; // not in got/gotplt range
}

static bool has_relocation(struct dso *dso, size_t reloc_addr)
{
    if (__predict_true(!dso->adlt)) {
        return false;
    }
	if (!adlt_ndso_index_check(dso->adlt_ndso_index)) {
		LD_LOGE("adlt_ndso_index is invalid, index=%{public}zd", dso->adlt_ndso_index);
		return false;
	}

    if (check_and_set_bit(&dso->adlt->gotEntry, reloc_addr, dso) == SECOND_RELOCATED) {
        return true;
    }

    return (check_and_set_bit(&dso->adlt->gotPltEntry, reloc_addr, dso) == SECOND_RELOCATED);
}

static void adlt_do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride, adlt_relindex_t *rel_index)
{
	int skip_relative = 0, reuse_addends = 0, save_slot = 0;
	int type;
	size_t addend;
	size_t *reloc_addr;
	size_t *rel_start = rel;

	if (dso == &ldso) {
		/* Only ldso's REL table needs addend saving/reuse. */
		if (rel == apply_addends_to)
			reuse_addends = 1;
		skip_relative = 1;
	}

	if (!rel_index) {
		return;
	}
	rel = rel_start + *rel_index++ * stride;

	for (; rel_size; 
	  rel = rel_start + *rel_index++ * stride, rel_size -= stride * sizeof(size_t)) {
		if (skip_relative && IS_RELATIVE(rel[1], dso->syms)) continue;
		type = R_TYPE(rel[1]);
		if (type == REL_NONE) continue;
		if (has_relocation(dso, rel[0])) {
			continue;
		}
		reloc_addr = laddr(dso, rel[0]);
		if (stride > 2) {
			addend = rel[2];
		} else if (type==REL_GOT || type==REL_PLT|| type==REL_COPY) {
			addend = 0;
		} else if (reuse_addends) {
			/* Save original addend in stage 2 where the dso
			 * chain consists of just ldso; otherwise read back
			 * saved addend since the inline one was clobbered. */
			if (head==&ldso){
				saved_addends[save_slot] = *reloc_addr;
			}
			addend = saved_addends[save_slot++];
		} else {
			addend = *reloc_addr;
		}
		do_one_reloc(dso, rel, stride, addend);
	}
}

static void adlt_do_android_relocs(
    struct dso *p, size_t dt_name, size_t dt_size, size_t rel_cnt, adlt_relindex_t *rel_index)
{
	struct unpack_reloc *relocs = NULL;
	size_t android_rel_addr = 0;

	relocs =(dt_name == DT_ANDROID_REL ? &(p->adlt->android_rel) : &(p->adlt->android_rela));
	search_vec(p->dynv, &android_rel_addr, dt_name);
	if (!android_rel_addr) {
		return;
	}
	if (relocs->map == MAP_FAILED) {
		(void)decode_android_relocs(p, dt_name, dt_size, (size_t **)&relocs->map, &relocs->map_len);
	}
    if (relocs->map != MAP_FAILED) {
		if (dt_name == DT_ANDROID_REL) {
			adlt_do_relocs(p, (size_t *)relocs->map, rel_cnt * sizeof(size_t) * 2, 2, rel_index);
		} else {
			adlt_do_relocs(p, (size_t *)relocs->map, rel_cnt * sizeof(size_t) * 3, 3, rel_index);
		}
	}
}


static void adlt_reloc(struct dso *p, size_t dyn[],const dl_extinfo *extinfo, ssize_t *relro_fd_offset)
{
	size_t plt_stride;
	// OHOS_LOCAL ADLT begin
	adlt_relindex_t *adlt_rel_dyn, *adlt_rel_plt;
	size_t rel_dyn_cnt, rel_plt_sz;

	if (head != &ldso && p->relro_start != p->relro_end) {
		if (mprotect(laddr(p, p->relro_start), p->relro_end-p->relro_start, PROT_READ | PROT_WRITE)
			&& errno != ENOSYS) {
			LD_LOGE("Error relocating %{public}s: RELRO protection failed: %{public}d", p->name, errno);
			if (runtime) longjmp(*rtld_fail, 1);
		}
	}

	/* For adlt mode, it is assumed that there are sections of type either
		* SHT_REL, SHT_RELA, SHT_ANDROID_REL, or SHT_ANDROID_RELA. That is, it
		* is impossible to combine them. So we can use the same reloc list for
		* them (relaDynIndx). */
	rel_dyn_cnt = (size_t)get_adlt_library_rela_dyn(p->adlt, p->adlt_ndso_index, &adlt_rel_dyn);
	plt_stride = 2 + (dyn[DT_PLTREL] == DT_RELA);
	rel_plt_sz = (size_t)get_adlt_library_rela_plt(p->adlt, p->adlt_ndso_index, &adlt_rel_plt) * 
		plt_stride * sizeof(size_t);

	adlt_do_relocs(p, laddr(p, dyn[DT_JMPREL]), rel_plt_sz, plt_stride, adlt_rel_plt);

	if (dyn[DT_RELSZ]) {
		adlt_do_relocs(p, laddr(p, dyn[DT_REL]), rel_dyn_cnt * 2 * sizeof(size_t), 2, adlt_rel_dyn);
	} 
	if (dyn[DT_RELASZ]){
		adlt_do_relocs(p, laddr(p, dyn[DT_RELA]), rel_dyn_cnt * 3 * sizeof(size_t), 3, adlt_rel_dyn);
	}

	if (!DL_FDPIC) {
		adlt_relindex_t *adlt_rel_relr;
		size_t rel_relr_cnt = (size_t)get_adlt_library_relr_dyn(p->adlt, p->adlt_ndso_index, &adlt_rel_relr);
		do_relr_relocs(p, laddr(p, dyn[DT_RELR]), dyn[DT_RELRSZ], rel_relr_cnt, adlt_rel_relr);
	}

	adlt_do_android_relocs(p, DT_ANDROID_REL, DT_ANDROID_RELSZ, rel_dyn_cnt, adlt_rel_dyn);
	adlt_do_android_relocs(p, DT_ANDROID_RELA, DT_ANDROID_RELASZ, rel_dyn_cnt, adlt_rel_dyn);

	if (head != &ldso && p->relro_start != p->relro_end &&
		mprotect(laddr(p, p->relro_start), p->relro_end-p->relro_start, PROT_READ)
		&& errno != ENOSYS) {
		LD_LOGE("Error relocating %{public}s: RELRO protection failed: %{public}d",
			p->name, errno);
		if (runtime) longjmp(*rtld_fail, 1);
	}
	/* Handle serializing/mapping the RELRO segment */
	handle_relro_sharing(p, extinfo, relro_fd_offset);

	p->relocated = 1;
	free_reloc_can_search_dso(p);
}

static size_t bolt_remap_addr_func(size_t in_addr, struct adlt *adlt) {
	if (!adlt || adlt->bolt_remap_len == 0) {
		return in_addr;
	}

	struct bolt_remap_data* remap_data = (struct bolt_remap_data*)adlt->bolt_remap_addr;
	ssize_t min  = 1;
	ssize_t max  =   ((size_t *)adlt->bolt_remap_addr)[1];
	size_t minAddr =  remap_data[min].new_addr;
	size_t maxAddr =  ((size_t *)adlt->bolt_remap_addr)[0];
	if (in_addr < minAddr || in_addr >= maxAddr) {
		return in_addr;
	}

	while (min <= max) {
		ssize_t mid = min + (max - min) / 2;
		size_t addr = remap_data[mid].new_addr;

		if (in_addr == addr) {
			return  remap_data[mid].old_addr;
		} else if (in_addr < addr) {
			max = mid - 1;
		} else {
			min = mid + 1;
		}
	}
	return  remap_data[max].old_addr;
}

void *bolt_remap_addr_func_adlt(size_t a)
{
	struct adlt* adlt;
	for (adlt = g_adlt_ptr; adlt; adlt = adlt->next) {
		if (adlt->pgbo_addr_start == 0 || adlt->pgbo_addr_end == 0) {
			continue;
		}
		if (a < (size_t)(adlt->pgbo_addr_start + adlt->base) || a >= (size_t)(adlt->pgbo_addr_end + adlt->base)) {
			continue;
		}
		break;
	}
	if (!adlt) {
		return a;
	}

	size_t base = (size_t)adlt->base;
	size_t elf_addr = a - base;
	size_t bolt_remap = bolt_remap_addr_func(elf_addr, adlt);
	if (bolt_remap != elf_addr) {
		return bolt_remap + base;
	}
	return a;
}

static bool adlt_find_in_dso(struct dso *p, Phdr *phdr, struct adlt *adlt, size_t entsz, size_t addr)
{
	Phdr *ph;
	adlt_phindex_t *phc_indexes, *phl_indexes;
	ssize_t phl_count = get_adlt_library_ph(p->adlt, p->adlt_ndso_index, &phl_indexes);
	ssize_t phc_count = get_adlt_common_ph(p->adlt, &phc_indexes);

	if (p->adlt != adlt && phc_indexes) {
		while(phc_count-- > 0) {
			ph = (void *)((char *)phdr + (size_t)(*phc_indexes++) * entsz);
			if (ph->p_type != PT_LOAD) continue;
			if (addr - ph->p_vaddr < ph->p_memsz) {
				return true;
			}
		}
		adlt = p->adlt;
	}
	if (phl_indexes) {
		while(phl_count-- > 0) {
			ph = (void *)((char *)phdr + (size_t)(*phl_indexes++) * entsz);
			if (ph->p_type != PT_LOAD) continue;
			if (addr - ph->p_vaddr < ph->p_memsz){
				return true;
			}
		}
	}
	return false;
}

static int handle_adlt_phdr(struct dso *current, struct dl_phdr_info *info)
{
    Phdr *ph;
    char *ph_addr;
    ssize_t ph_count = 0, ph_count_common = 0, ph_count_library = 0;
    adlt_phindex_t *ph_indexes_common, *ph_indexes_library;
    size_t entsz = current->phentsize;
    Phdr *phdr = current->phdr;
	current->phdr_map = NULL;
	size_t adlt_ndso = 0;
	int adlt_cpy_ret = 0;
	char *temp;

	if (!phdr) {
		return -1;
	}
	size_t cnt = current->phnum;
	current->phdr_map = malloc(entsz * cnt);
	info->dlpi_phdr = current->phdr_map;
	if (current->phdr_map == NULL) {
		LD_LOGW("malloc error at handle_adlt_phdr ! dso name: %{public}s.", current->name);
		return -1;
	}
	ph_addr = current->phdr_map;
	for (ph = phdr; cnt--; ph = (void *)((char *)ph + entsz)) {
        switch (ph->p_type) {
            case PT_LOAD:
            case PT_TLS:
            case PT_GNU_EH_FRAME:
                continue;
            default:
                break;
        }
        memcpy(ph_addr, ph, entsz);
        ph_addr += entsz;
        ++adlt_ndso;
    }

    ph_count_common = get_adlt_common_ph(current->adlt, &ph_indexes_common);
	if (ph_count_common > 0 && ph_indexes_common) {
		ph_count = ph_count_common;
    }

	ph_count_library = get_adlt_library_ph(current->adlt, current->adlt_ndso_index, &ph_indexes_library);
	if (ph_count_library > 0 && ph_indexes_library) {
		ph_count += ph_count_library;
    }

	temp = realloc(current->phdr_map, entsz * (adlt_ndso + ph_count));
	if (temp == NULL) {
		LD_LOGW("realloc error at adlt_cpy_ph ! dso name: %{public}s.", current->name);
		return -1;
	}
	current->phdr_map = temp;
	info->dlpi_phdr = current->phdr_map;
	info->dlpi_phnum = adlt_ndso + (size_t)ph_count;
	current->phdr_count = info->dlpi_phnum;
	if (ph_count > 0) {
		ph_addr = current->phdr_map + (adlt_ndso) * entsz;
		while (ph_count_common--) {
			memcpy(ph_addr, (char *)current->phdr + (*ph_indexes_common) * entsz, entsz);
			ph_addr += entsz;
			++ph_indexes_common;
		}
		while (ph_count_library--) {
			memcpy(ph_addr, (char *)current->phdr + (*ph_indexes_library) * entsz, entsz);
			ph_addr += entsz;
			++ph_indexes_library;
		}
	}
    return 0;
}

void init_entry(struct adlt_got_entry *entry, Shdr *sh, struct loadtask *task)
{
	entry->entry_addr = sh->sh_addr;
	entry->entry_len = sh->sh_size;
	entry->record_entry = (uint8_t *)calloc(ADLT_UPALIGN(sh->sh_size, sizeof(uint64_t)) * ADLT_UPALIGN(ADLT_MAX_NSO, BITS_IN_BYTE), sizeof(uint8_t));
	if (!entry->record_entry) {
		LD_LOGE("Error mapping header : failed to allocate gotEntry");
		return;
	}
	off_t off_start;
	off_start = sh->sh_offset;
	off_start &= -PAGE_SIZE;
	entry->got_map_len = sh->sh_size + (sh->sh_offset - off_start);
	entry->got_map = mmap(0, entry->got_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
	if (entry->got_map == MAP_FAILED) {
		LD_LOGE("Error mapping got entry %{public}s: failed to map got/got.plt section", task->name);
		return;
	}
	entry->got_entry = (size_t *)(entry->got_map + sh->sh_offset - off_start);
}

void init_gotentry(struct adlt *adlt, Shdr *sh, struct loadtask *task, unsigned char *shstr_addr)
{
	if (!adlt) {
		return;
	}
	// init for gotEntry/got.pltEntry
	Shdr *sh3 = (void *)((char *)sh + (task->eh->e_shnum - 1) * task->eh->e_shentsize);
	for (size_t i = task->eh->e_shnum; i; --i, sh3 = (void *)((char *)sh3 - task->eh->e_shentsize)) 
	{
		if (sh3->sh_type != SHT_PROGBITS) {
			continue;
		}
		const char *section_header_name = (const char *)(shstr_addr + sh3->sh_name);
		if (!strcmp(section_header_name, ".got")) {
			init_entry(&adlt->gotEntry, sh3, task);
		}
		if (!strcmp(section_header_name, ".got.plt")) {
			init_entry(&adlt->gotPltEntry, sh3, task);
		}
	}
}

static void adlt_clean_dso_flag(struct adlt_got_entry *entry, ssize_t ndso_index)
{
	size_t got_num = ADLT_UPALIGN(entry->entry_len, sizeof(uint64_t));
	for  (size_t i = 0; i < got_num; i++) {
		uint64_t mask = ~(1UL << ndso_index);
		entry->record_entry[i] &= mask;
	}
}

static void adlt_clean_gotentry(struct dso *p, struct adlt *adlt)
{
	if (!adlt_ndso_index_check(p->adlt_ndso_index)) {
		return;
	}

	adlt_clean_dso_flag(&adlt->gotEntry, p->adlt_ndso_index);
	adlt_clean_dso_flag(&adlt->gotPltEntry, p->adlt_ndso_index);
}

#ifdef LOAD_ORDER_RANDOMIZATION
static bool is_adlt_plus_merge_so(const char *so_name){
	const char *colon_p = strchr(so_name, ':');
	const char *adlt_p = strstr(so_name, "libadlt");
	if (colon_p && adlt_p && adlt_p < colon_p){
		return true;
	}
	return false;
}

static bool adlt_load_library_header_extra(struct loadtask *task, ns_t *namespace)
{
	//if exist "libadlt" and ":" in task->name, delete the characters before ":" and reload
	const char *task_name = task->name;
	if (is_adlt_plus_merge_so(task->name)) {
		task->namespace = namespace;
		task->check_inherited = true;
		task->name = strchr(task->name, ':') + 1;
		if (load_library_header(task)){
			task->name = task_name;
			return true;
		}
		else{
			LD_LOGE("adlt_load_library_header_extra: load %{public}s failed, namespace=%{public}s",
			 task->name, namespace->ns_name);
		}
	}
	task->name = task_name;
	return false;
}

static bool parse_adlt_library_header(struct loadtask *task, Phdr *ph)
{
	struct adlt *adlt = calloc(1, sizeof(struct adlt));
	if (!adlt) {
		LD_LOGE("Error mapping header %{public}s: failed to allocate adlt", task->name);
		return false;
	}
	init_adlt(adlt);
	task->adlt = adlt;

	struct stat st;
	if (fstat(task->fd, &st) < 0) {
		LD_LOGE("Error mapping header %{public}s: failed to get file state", task->name);
		return false;
	}
	adlt->dev = st.st_dev;
	adlt->ino = st.st_ino;
	adlt->file_size = st.st_size;
	adlt->file_offset = task->file_offset;
	// Resolve adlt so real path+name
	adlt->name = create_realpath_from_fd(task->fd);
	if (!adlt->name) {
		LD_LOGE("Error mapping header %{public}s: failed to resolve adlt name", task->name);
		return false;
	}
	// Map .adlt section
	off_t off_start = ph->p_offset;
	off_start &= -PAGE_SIZE;
	adlt->sa_map_len = ph->p_filesz + (ph->p_offset - off_start);
	adlt->sa_map = mmap(0, adlt->sa_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
	if (adlt->sa_map == MAP_FAILED) {
		LD_LOGE("Error mapping header %{public}s: failed to map adlt section", task->name);
		return false;
	}
	adlt->sa_addr = (void *)((char *)(adlt->sa_map) + ph->p_offset - off_start);
	ssize_t sym_so_map_count = get_adlt_sym_dso_map(adlt, &adlt->sym_dso_Idx_map);
	add_adlt(adlt);
	return true;
}

static void set_start_end_addr(struct loadtask *task, Shdr *sh)
{
	uint64_t _end = sh->sh_addr + sh->sh_size;
	if (task->adlt->pgbo_addr_start == 0) {
		task->adlt->pgbo_addr_start = sh->sh_addr;
		task->adlt->pgbo_addr_end = _end;
	} else {
		task->adlt->pgbo_addr_start =
			(task->adlt->pgbo_addr_start < sh->sh_addr) ? task->adlt->pgbo_addr_start : sh->sh_addr;
		task->adlt->pgbo_addr_end =
			(task->adlt->pgbo_addr_end > _end) ? task->adlt->pgbo_addr_end : _end;
	}
}

static bool map_remap_strtab(struct loadtask *task, Shdr *sh2, size_t bsl_bolt_remap_index, size_t strtab_sec_index)
{
	Shdr *sh;
	off_t off_start;
	if (!strtab_sec_index) {
		LD_LOGE("Error mapping header %{public}s: failed to find adlt strtab section(s)", task->name);
		return false;
	}

	if (bsl_bolt_remap_index) {
		/* .bolt.remap mapping */
		sh = (void *)((char *)sh2 + bsl_bolt_remap_index * task->eh->e_shentsize);
		off_start = sh->sh_offset;
		off_start &= -PAGE_SIZE;
		task->adlt->bolt_remap_len = sh->sh_size + (sh->sh_offset - off_start);
		task->adlt->bolt_remap =
			mmap(0, task->adlt->bolt_remap_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
		if (task->adlt->bolt_remap == MAP_FAILED) {
			LD_LOGE("Error mapping header %{public}s: failed to map adlt .bolt.remap section", task->name);
			return false;
		}
		task->adlt->bolt_remap_addr = (void *)(task->adlt->bolt_remap + sh->sh_offset - off_start); 
	} else {
		task->adlt->bolt_remap_len = 0;
	}

	/* .adlt.strtab mapping */
	sh = (void *)((char *)sh2 + strtab_sec_index * task->eh->e_shentsize);
	off_start = sh->sh_offset;
	off_start &= -PAGE_SIZE;
	task->adlt->strtab_map_len = sh->sh_size + (sh->sh_offset - off_start);
	task->adlt->strtab_map =
		mmap(0, task->adlt->strtab_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
	if (task->adlt->strtab_map == MAP_FAILED) {
		LD_LOGE("Error mapping header %{public}s: failed to map adlt strtab section", task->name);
		return false;
	}
	task->adlt->strtab_addr = (void *)(task->adlt->strtab_map + sh->sh_offset - off_start);

	return true;
}

static bool lookup_strtab_map(struct loadtask *task, Shdr *sh2)
{
	size_t i;
	/* Looking for .adlt.strtab, .strtab, .symtab */
	if (task->adlt->strtab_map != MAP_FAILED) return true;
	/* By current design, the .adlt.strtab section and the strings section
		for the backtrace procedure are searched by name */
	if (task->eh->e_shstrndx >= task->eh->e_shnum) {
		LD_LOGE("Error mapping header %{public}s: sections strtab section not found", task->name);
		return false;
	}
	/* sections name section mapping */
	Shdr *shstr = (void *)((char *)sh2 + task->eh->e_shstrndx * task->eh->e_shentsize);
	off_t off_start = shstr->sh_offset;
	off_start &= -PAGE_SIZE;
	size_t shstr_map_len = shstr->sh_size + (shstr->sh_offset - off_start);
	unsigned char *shstr_map = mmap(0, shstr_map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
	if (shstr_map == MAP_FAILED) {
		LD_LOGE("Error mapping header %{public}s: failed to map sections strtab section", task->name);
		return false;
	}

	unsigned char *shstr_addr = (void *)(shstr_map + shstr->sh_offset - off_start);
	size_t strtab_sec_index = 0;
	size_t bsl_bolt_remap_index = 0;
	size_t text_remap_index = 0;
	size_t text_cold_remap_index = 0;

	Shdr *sh = (void *)((char *)sh2 + (task->eh->e_shnum - 1) * task->eh->e_shentsize);
	for (i = task->eh->e_shnum;
			i && (!strtab_sec_index || !bsl_bolt_remap_index || !text_remap_index || !text_cold_remap_index);
			--i, sh = (void *)((char *)sh - task->eh->e_shentsize)) {
		const char *name_ = (const char *)(shstr_addr + sh->sh_name);
		if (sh->sh_type == SHT_STRTAB && !strcmp(name_, ".adlt.strtab")) {
			strtab_sec_index = i - 1;
			continue;
		} else if (sh->sh_type == SHT_PROGBITS){
			if (!strcmp(name_, ".bolt.remap")) {
				bsl_bolt_remap_index = i - 1;
			} else if (!strcmp(name_, ".text")) {
				set_start_end_addr(task, sh);
				text_remap_index = i - 1;
			} else if (!strcmp(name_, ".text.cold")) {
				set_start_end_addr(task, sh);
				text_cold_remap_index = i - 1;
			}
		}
	}

	init_gotentry(task->adlt, sh2, task, shstr_addr);

	munmap(shstr_map, shstr_map_len);

	if (!map_remap_strtab(task, sh2, bsl_bolt_remap_index, strtab_sec_index)) return false;

	return true;
}

static bool lookup_ndso(struct loadtask *task, Shdr *sh2)
{
	Shdr *sh;
	if (task->adlt_ndso_index < 0) {
		/* There are cases where task->pathname points to the actual path+name
		 * of the adlt library file. So we use task->name to calculate the
		 * index of nDSO.*/
		task->adlt_ndso_index = get_adlt_library_index2(task->adlt, task->fullname);
	}
	/* Looking for nDSO init_array and fini_array */
	if (task->adlt_ndso_index >= 0) {
		adlt_cross_section_array_t *cross;

		get_adlt_library_init_array(task->adlt, task->adlt_ndso_index, &cross);
		if (!cross || cross->secIndex >= task->eh->e_shnum) {
			LD_LOGE("Error mapping header %{public}s: failed to find init array", task->name);
			return false;
		}
		sh = (void *)((char *)sh2 + cross->secIndex * task->eh->e_shentsize);
		task->init_array_off = sh->sh_addr + cross->offset;

		get_adlt_library_fini_array(task->adlt, task->adlt_ndso_index, &cross);
		if (!cross || cross->secIndex >= task->eh->e_shnum) {
			LD_LOGE("Error mapping header %{public}s: failed to find fini array", task->name);
			return false;
		}
		sh = (void *)((char *)sh2 + cross->secIndex * task->eh->e_shentsize);
		task->fini_array_off = sh->sh_addr + cross->offset;
	}
	return true;
}

static void lookup_tls(struct loadtask *task, bool tls_appeared)
{
	Phdr *ph;
	/* Looking for nDSO TLS */
	if (!tls_appeared) return;
	task->tls_image = 0;
	task->tls.align = 0;
	task->tls.len = 0;
	task->tls.size = 0;

	adlt_phindex_t *ph_indexes, *phc_indexes, *phl_indexes;
	ssize_t phl_count = get_adlt_library_ph(task->adlt, task->adlt_ndso_index, &phl_indexes);
	ssize_t phc_count = get_adlt_common_ph(task->adlt, &phc_indexes);
	ssize_t ph_count;

	size_t z = 2;
	while(z--) {
		ph_count = z > 0 ? phc_count : phl_count;
		ph_indexes = z > 0 ? phc_indexes : phl_indexes;
		if (ph_count <= 0 || ph_indexes <= 0) {
			continue;
		}
		for (ph = (void *)((char *)task->ph0 + (*ph_indexes) * task->eh->e_phentsize); ph_count;
			ph_count--, ph_indexes++, ph = (void *)((char *)task->ph0 + (*ph_indexes) * task->eh->e_phentsize)) {
			if (ph->p_type == PT_TLS) {
				task->tls_image = ph->p_vaddr;
				task->tls.align = ph->p_align;
				task->tls.len = ph->p_filesz;
				task->tls.size = ph->p_memsz;
				z = 0;
				break;
			}
		}
	}
}

static bool lookup_adlt_library(struct loadtask *task, Shdr *sh2, bool tls_appeared)
{
	if (__predict_true(!task->adlt)) return true;
	if (!lookup_strtab_map(task, sh2)) return false;
	if (!lookup_ndso(task, sh2)) return false;
	lookup_tls(task, tls_appeared);
	return true;
}

static bool adlt_init_rw_seg(struct loadtask *task)
{
	size_t off_start;
	size_t addr_max;
	size_t addr_min;
	size_t map_len;
	unsigned char *map;
	unsigned int relro_flag;
	struct dso *p = task->p;
	unsigned char *base = NULL;
	adlt_phindex_t *ph_indexes = NULL;
	ssize_t ph_count = get_adlt_library_ph(p->adlt, p->adlt_ndso_index, &ph_indexes);
	if (ph_count < 0 || !ph_indexes) {
		return false;
	}
	base = p->adlt->base;
	for (size_t i = 0; i < ph_count; i++) {
		const Phdr *ph = &p->phdr[ph_indexes[i]];
		if (ph->p_type == PT_LOAD && (ph->p_flags & PF_W)) {
			unsigned char *cur_beg = base + ph->p_vaddr;
			if (DL_NOMMU_SUPPORT) {
				memset((void *)cur_beg, 0, ph->p_memsz);
				continue;
			}
			relro_flag = ((p->relro_start) <= (ph)->p_vaddr)&&(((ph)->p_vaddr + (ph)->p_memsz) <= p->relro_end)? 1 : 0;
			if (relro_flag && mprotect((base + ph->p_vaddr), ph->p_memsz, PROT_READ | PROT_WRITE)
				&& errno != ENOSYS) {
				error("Error relocating %s: RELRO protection failed", p->name);
				longjmp(*rtld_fail, 1);
			}
			off_start = ph->p_offset & -PAGE_SIZE;
			/* align up page size */
			addr_max = (ph->p_vaddr + ph->p_filesz + PAGE_SIZE - 1) & (-PAGE_SIZE);
			/* align down page size */
			addr_min = ph->p_vaddr & -PAGE_SIZE;
			map_len = addr_max - addr_min;
			map = mmap((void*)NULL, map_len, PROT_READ, MAP_PRIVATE, task->fd, off_start + task->file_offset);
			if (map == MAP_FAILED) {
				error("Mapping loadtask %s failed , err = %d\n", task->name, errno);
				longjmp(*rtld_fail, 1);
			}
			/* load segment always page size align */
			memcpy((base + addr_min), map, map_len);
			if (ph->p_memsz > ph->p_filesz) {
				size_t brk = (size_t)base + ph->p_vaddr + ph->p_filesz;
				size_t this_max = (ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1) & (-PAGE_SIZE);
				size_t zeromap_size = (size_t)base + this_max - brk;
				/* zeropadding the remaining memory with page size alignment*/
				memset((void *)brk, 0, zeromap_size);
			}
			munmap(map, map_len);

		}
	}
	return true;
}


static char *create_realpath_from_fd(int fd)
{
	void *buf = malloc(PATH_MAX + 1);
	if (buf) {
		ssize_t len = fd_to_realpath(fd, buf, PATH_MAX + 1);
		if (len >= 0) {
			void *buf_new = realloc(buf, len + 1);
			return buf_new ? buf_new : buf; 
		}
		free(buf); 
	}
	return NULL;
}

#endif

static void adlt_find_and_set_bss_name(struct dso *p, Phdr *phdr)
{
	size_t entsz = p->phentsize;
	adlt_phindex_t *ph_indexes, *phc_indexes, *phl_indexes;
	ssize_t phl_count = get_adlt_library_ph(p->adlt, p->adlt_ndso_index, &phl_indexes);
	ssize_t phc_count = get_adlt_common_ph(p->adlt, &phc_indexes);
	int loop = 2;
	ssize_t ph_num;
	size_t i, q;
	Phdr *ph;
	while(loop--) {
		if (loop) {
			if (phc_count <= 0 || !phc_indexes) {
				continue;
			}
			ph_indexes = phc_indexes;
			ph_num = phc_count;
			i = ph_indexes[0];
		} else {
			if (phl_count < 0 || !phl_indexes) {
				continue;
			}
			ph_indexes = phl_indexes;
			ph_num = phl_count;
			i = ph_indexes[0];
		}

		q = 0;
		for (ph = (void *)((char *)phdr + i * entsz); ph_num;
	  	  ph_num--, i = ph_indexes ? (size_t)ph_indexes[++q] : i + 1, ph = (void *)((char *)phdr + i * entsz)) {
			if (ph->p_type != PT_LOAD) continue;
			size_t seg_start = p->base + ph->p_vaddr;
			size_t seg_file_end = seg_start + ph->p_filesz + PAGE_SIZE - 1 & -PAGE_SIZE;
			size_t seg_max_addr = seg_start + ph->p_memsz + PAGE_SIZE - 1 & -PAGE_SIZE;
			size_t zeromap_size = seg_max_addr - seg_file_end;
			if (zeromap_size > 0 && (ph->p_flags & PF_W)) {
				set_bss_vma_name(p->name, (void *)seg_file_end, zeromap_size);
			}
		}
	}
}

static inline Sym *adlt_lookup_unique_sym(struct dso *dso, struct verinfo *verinfo)
{
	char *name = verinfo->s;
	Sym *sym = NULL;
 
	char *name_idx = malloc((strlen(name) + 10) * sizeof(char));
	if (!name_idx) {
		return NULL;
	}
 
	ssize_t idx = dso->adlt_ndso_index;
	if (idx < 0) {
		return NULL;
	}
	// Get name with postfix: name + __ + hex(order index)
	(void)sprintf(name_idx, "%s__%x", name, idx);
	verinfo->s = name_idx;
	do {
		struct sym_info_pair s_info_g = gnu_hash(verinfo->s);
		struct sym_info_pair s_info_s = {0, 0};
		uint32_t gh = s_info_g.sym_h, gho = gh / (8 * sizeof(size_t)), *ght;
		size_t ghm = 1ul << gh % (8 * sizeof(size_t));
		if ((ght = dso->ghashtab)) {
			if (gnu_hash_filter(ght, ghm, gho, gh)) {
				sym = gnu_lookup(s_info_g, ght, dso, verinfo);
			}
		} else {
			if (!s_info_s.sym_h) s_info_s = sysv_hash(verinfo->s);
			sym = sysv_lookup(verinfo, s_info_s, dso);
		}
	} while(0);
 
	verinfo->s = name;
	free(name_idx);
	return sym;
}

#else

static struct dso *find_library_by_adlt_lstat(
    const struct stat *st, const ns_t *ns, bool check_inherited, uint64_t file_offset) {return NULL;};
static struct dso *find_library_by_adlt_index(
    const struct adlt *adlt, const ns_t *ns, bool check_inherited, ssize_t library_index) {return NULL;};
static void free_adlt(struct adlt *adlt) {return; };
static void init_adlt(struct adlt *adlt){return ;};
static void unmap_adlt_library(struct dso *dso) {return ;};
ssize_t get_adlt_common_ph(struct adlt *adlt, adlt_phindex_t **ph_indexes) {return 0;};
static ssize_t get_adlt_library_index(unsigned char *strtab_addr, unsigned char *sa_addr, const char *pathname) {return 0;};
static ssize_t get_adlt_library_index2(struct adlt *adlt, const char *pathname) {return 0;};
static adlt_psod_t *get_adlt_library_entry(struct adlt *adlt, ssize_t library_index, char **blob) {return NULL;};
ssize_t get_adlt_library_ph(struct adlt *adlt, ssize_t library_index, adlt_phindex_t **ph_indexes) {return 0;};
ssize_t get_adlt_sym_dso_map(struct adlt *adlt, uint8_t **sym_dso_idx_map) {return 0;};
static ssize_t get_adlt_library_dt_needed(struct adlt *adlt, ssize_t library_index, adlt_dt_needed_index_t **dt_needed) {return 0;};
static ssize_t get_adlt_library_rela_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_dyn) {return 0;};
static ssize_t get_adlt_library_rela_plt(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **rela_plt) {return 0;};
static ssize_t get_adlt_library_relr_dyn(struct adlt *adlt, ssize_t library_index, adlt_relindex_t **relr_dyn) {return 0;};
static size_t get_adlt_library_init_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **init_cross) {return 0;};

static void adlt_clean_gotentry(struct dso *p, struct adlt *adlt) {return;};
static size_t get_adlt_library_fini_array(
    struct adlt *adlt, ssize_t library_index, adlt_cross_section_array_t **fini_cross) {return 0;};
static ssize_t get_adlt_library_final_sym_indexes(
    struct adlt *adlt, ssize_t library_index, struct adlt_dso_sym_index *idx) {return 0;};
static adlt_secindex_t get_adlt_symbol_sec_index(struct adlt *adlt) {return NULL;};
static ssize_t get_adlt_objects_num(struct adlt *adlt) {return 0;};
static ssize_t merge_sort_arrays(adlt_phindex_t **ph_indexes, ssize_t ph_count, 
		adlt_phindex_t **phl_indexes, ssize_t phl_count, 
		adlt_phindex_t **phc_indexes, ssize_t phc_count) {return 0;};
static bool replace_to_adlt_duplicate(struct dso *dso, Sym **sym, const char **name) {return false;};
static bool replace_to_adlt_duplicate2(struct dso *dso, Sym **sym, const char **name) {return false;};
static bool is_adlt_partly_load() {return false;};
static bool is_adlt_dso_sym(struct dso *dso, uint32_t idx) {return true;};
static void add_adlt(struct adlt* adlt) {return ;};
static void del_adlt_from_gadlt(struct adlt* adlt) {return ;};
static bool is_same_adlt(struct adlt* adlt, struct stat* st) {return false;};
static struct adlt* find_adlt_by_fstat(struct stat* st) {return NULL;};
static char *create_realpath_from_fd(int fd) {return NULL;};
static void adlt_reclaim_gaps(struct dso *dso) {return ;};
static void adlt_reloc(struct dso *p, size_t dyn[],const dl_extinfo *extinfo, ssize_t *relro_fd_offset) {return ;};
static void adlt_find_and_set_bss_name(struct dso *p, Phdr *phdr) {return ;};
void *bolt_remap_addr_func_adlt(size_t a) {return a;};
static bool adlt_find_in_dso(struct dso *p, Phdr *phdr, struct adlt *adlt, size_t entsz, size_t addr) {return false;};
static int handle_adlt_phdr(struct dso *current, struct dl_phdr_info *info){return 0;};
static struct dso *search_dso_by_adlt_lstat(const struct stat *st, const ns_t *ns, uint64_t file_offset) {return NULL;};
static struct dso *search_dso_by_adlt_index(const struct adlt *adlt, const ns_t *ns, ssize_t library_index) {return NULL;};
static void free_record_entry(struct adlt_got_entry *entry) {return;};
char *next_link_name(char **fullpath, char *buf, ssize_t bufsize) {return NULL;};
static void set_adlt_symindex_map_to_library(struct dso *p) {return;};
static Relocated_Flag check_and_set_bit(struct adlt_got_entry *et, size_t *reloc_addr , ssize_t ndso_index) {return NOT_GOT;};
static bool has_relocation(struct dso *dso, size_t *reloc_addr) {return false;};
static void adlt_do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride, adlt_relindex_t *rel_index) {return;};
static void adlt_do_android_relocs( 
    struct dso *p, size_t dt_name, size_t dt_size, size_t rel_cnt, adlt_relindex_t *rel_index) {return;};

static size_t bolt_remap_addr_func(size_t in_addr, struct adlt *adlt) {return 0;};
static inline Sym *adlt_lookup_unique_sym(struct dso *dso, struct verinfo *verinfo) {return NULL;};

void init_entry(struct adlt_got_entry *entry, Shdr *sh) {return;};
void init_gotentry(struct adlt *adlt, Shdr *sh, struct loadtask *task, unsigned char *shstr_addr) {return;};
static void adlt_clean_dso_flag(struct adlt_got_entry *entry, ssize_t ndso_index) {return;};

#ifdef LOAD_ORDER_RANDOMIZATION
static bool parse_adlt_library_header(struct loadtask *task, Phdr *ph) {return true;};
static void set_start_end_addr(struct loadtask *task, Shdr *sh) {return;};
static bool map_remap_strtab(struct loadtask *task, Shdr *sh2, size_t bsl_bolt_remap_index, size_t strtab_sec_index) {return true;};
static bool lookup_strtab_map(struct loadtask *task, Shdr *sh2) {return true;};
static bool lookup_ndso(struct loadtask *task, Shdr *sh2) {return true;};
static void lookup_tls(struct loadtask *task, bool tls_appeared) {return;};
static bool lookup_adlt_library(struct loadtask *task, Shdr *sh2, bool tls_appeared) {return true;};
static bool adlt_init_rw_seg(struct loadtask *task) {return true;};

#endif

#endif