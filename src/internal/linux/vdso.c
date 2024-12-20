/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <elf.h>
#include <link.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "libc.h"
#include "syscall.h"

#ifdef VDSO_USEFUL

#if ULONG_MAX == 0xffffffff
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
typedef Elf32_Sym Sym;
typedef Elf32_Verdef Verdef;
typedef Elf32_Verdaux Verdaux;
#else
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
typedef Elf64_Sym Sym;
typedef Elf64_Verdef Verdef;
typedef Elf64_Verdaux Verdaux;
#endif

static int checkver(Verdef *def, int vsym, const char *vername, char *strings)
{
	vsym &= 0x7fff;
	for (;;) {
		if (!(def->vd_flags & VER_FLG_BASE)
		  && (def->vd_ndx & 0x7fff) == vsym)
			break;
		if (def->vd_next == 0)
			return 0;
		def = (Verdef *)((char *)def + def->vd_next);
	}
	Verdaux *aux = (Verdaux *)((char *)def + def->vd_aux);
	return !strcmp(vername, strings + aux->vda_name);
}

#define OK_TYPES (1<<STT_NOTYPE | 1<<STT_OBJECT | 1<<STT_FUNC | 1<<STT_COMMON)
#define OK_BINDS (1<<STB_GLOBAL | 1<<STB_WEAK | 1<<STB_GNU_UNIQUE)

struct __vdso_info {
	size_t base;
	char *strings;
	Sym *syms;
	Elf_Symndx *hashtab;
	uint16_t *versym;
	Verdef *verdef;
} vdso_info = {-1, 0, 0, 0, 0, 0};

void __get_vdso_info()
{
	if (vdso_info.base != -1) {
		return ;
	}
	size_t i;
	for (i=0; libc.auxv[i] != AT_SYSINFO_EHDR; i+=2)
		if (!libc.auxv[i]) return ;
	if (!libc.auxv[i+1]) return ;
	Ehdr *eh = (void *)libc.auxv[i+1];
	Phdr *ph = (void *)((char *)eh + eh->e_phoff);
	size_t *dynv=0, base=-1;
	for (i=0; i<eh->e_phnum; i++, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type == PT_LOAD)
			base = (size_t)eh + ph->p_offset - ph->p_vaddr;
		else if (ph->p_type == PT_DYNAMIC)
			dynv = (void *)((char *)eh + ph->p_offset);
	}
	if (!dynv || base==(size_t)-1) return ;
	
	for (i=0; dynv[i]; i+=2) {
		void *p = (void *)(base + dynv[i+1]);
		switch(dynv[i]) {
		case DT_STRTAB: vdso_info.strings = p; break;
		case DT_SYMTAB: vdso_info.syms = p; break;
		case DT_HASH: vdso_info.hashtab = p; break;
		case DT_VERSYM: vdso_info.versym = p; break;
		case DT_VERDEF: vdso_info.verdef = p; break;
		}
	}
    vdso_info.base = base;
    return ;
}

void *__get_vdso_addr(const char *vername, const char *name)
{
	if (!vdso_info.strings || !vdso_info.syms || !vdso_info.hashtab) return 0;
	if (!vdso_info.verdef) vdso_info.versym = 0;

    size_t i;
	for (i=0; i<vdso_info.hashtab[1]; i++) {
		if (!(1<<(vdso_info.syms[i].st_info&0xf) & OK_TYPES)) continue;
		if (!(1<<(vdso_info.syms[i].st_info>>4) & OK_BINDS)) continue;
		if (!vdso_info.syms[i].st_shndx) continue;
		if (strcmp(name, vdso_info.strings+vdso_info.syms[i].st_name)) continue;
		if (vdso_info.versym && !checkver(vdso_info.verdef, vdso_info.versym[i], vername, vdso_info.strings))
			continue;
		return (void *)(vdso_info.base + vdso_info.syms[i].st_value);
	}

	return 0;
}

#endif
