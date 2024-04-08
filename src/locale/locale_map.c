#include <locale.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "locale_impl.h"
#include "libc.h"
#include "lock.h"
#include "fork_impl.h"
#include <musl_log.h>

#define malloc __libc_malloc
#define calloc undef
#define realloc undef
#define free undef

const char *__lctrans_impl(const char *msg, const struct __locale_map *lm)
{
	const char *trans = 0;
	if (lm) trans = __mo_lookup(lm->map, lm->map_size, msg);
	return trans ? trans : msg;
}

#ifndef __LITEOS__
static const char envvars[][18] = {
	"LC_CTYPE",
	"LC_NUMERIC",
	"LC_TIME",
	"LC_COLLATE",
	"LC_MONETARY",
	"LC_MESSAGES",
	"LC_PAPER",
	"LC_NAME",
	"LC_ADDRESS",
	"LC_TELEPHONE",
	"LC_MEASUREMENT",
	"LC_IDENTIFICATION",
};
#else
static const char envvars[][12] = {
	"LC_CTYPE",
	"LC_NUMERIC",
	"LC_TIME",
	"LC_COLLATE",
	"LC_MONETARY",
	"LC_MESSAGES",
};
#endif

volatile int __locale_lock[1];
volatile int *const __locale_lockptr = __locale_lock;

static const char *valid_locale_table[] = {"zh_CN", "zh_CN.UTF-8"};
static int tableSize = sizeof(valid_locale_table) / sizeof(valid_locale_table[0]);

#define VALID_ICU_NAME_LEN 5
static char valid_icu_locale_name[VALID_ICU_NAME_LEN+1];
char *get_valid_icu_locale_name(const char *name) {
	char *res = strncpy(valid_icu_locale_name, name, VALID_ICU_NAME_LEN);
	valid_icu_locale_name[VALID_ICU_NAME_LEN] = '\0';
	return valid_icu_locale_name;
}

extern void *find_hmicuuc_symbol(const char *symbol_name);
static void (*g_icuuc_set_icu_directory)(void);


const struct __locale_map *__get_locale(int cat, const char *val)
{
	static void *volatile loc_head;
	const struct __locale_map *p;
	struct __locale_map *new = 0;
	const char *path = 0, *z;
	char buf[256];
	size_t l, n;

	if (!*val) {
		(val = getenv("LC_ALL")) && *val ||
		(val = getenv(envvars[cat])) && *val ||
		(val = getenv("LANG")) && *val ||
		(val = "C.UTF-8");
	}

	/* Limit name length and forbid leading dot or any slashes. */
	for (n=0; n<LOCALE_NAME_MAX && val[n] && val[n]!='/'; n++);
	if (val[0]=='.' || val[n]) val = "C.UTF-8";
	int builtin = (val[0]=='C' && !val[1])
#ifndef __LITEOS__
		|| !strcmp(val, "POSIX")
		|| !strcmp(val, "en_US");
#else
		|| !strcmp(val, "C.UTF-8")
		|| !strcmp(val, "POSIX");
#endif

	if (builtin) {
		if (cat == LC_CTYPE && val[1]=='.')
			return (void *)&__c_dot_utf8;
		return 0;
	}

	for (p=loc_head; p; p=p->next)
		if (!strcmp(val, p->name)) return p;

	if (!strcmp(val, "en_US.UTF-8") || !strcmp(val, "C.UTF-8")) {
		/* If no locale definition was found, make a locale map
		* object anyway to store the name, which is kept for the
		* sake of being able to do message translations at the
		* application level. */
		if (!new && (new = malloc(sizeof *new))) {
			new->map = __c_dot_utf8.map;
			new->map_size = __c_dot_utf8.map_size;
			memcpy(new->name, val, n);
			new->name[n] = 0;
			new->next = loc_head;
			new->flag = VALID;
			loc_head = new;
		}
	}

	if (!libc.secure) path = getenv("MUSL_LOCPATH");
	/* FIXME: add a default path? */

	if (path) for (; *path; path=z+!!*z) {
		z = __strchrnul(path, ':');
		l = z - path;
		if (l >= sizeof buf - n - 2) continue;
		memcpy(buf, path, l);
		buf[l] = '/';
		memcpy(buf+l+1, val, n);
		buf[l+1+n] = 0;
		size_t map_size;
		const void *map = __map_file(buf, &map_size);
		if (map) {
			new = malloc(sizeof *new);
			if (!new) {
				__munmap((void *)map, map_size);
				break;
			}
			new->map = map;
			new->map_size = map_size;
			memcpy(new->name, val, n);
			new->name[n] = 0;
			new->next = loc_head;
			new->flag = VALID;
			loc_head = new;
			break;
		}
	}

	/* If no locale definition was found, make a locale map
	 * object anyway to store the name, which is kept for the
	 * sake of being able to do message translations at the
	 * application level. */
	if (!new && (new = malloc(sizeof *new))) {
		new->map = __c_dot_utf8.map;
		new->map_size = __c_dot_utf8.map_size;
		memcpy(new->name, val, n);
		new->name[n] = 0;
		new->next = loc_head;
		new->flag = INVALID;
		loc_head = new;
	}

	if (new->flag == INVALID) {
		MUSL_LOGE("[wzxcheck] start check invalid locale");
		for (int i = 0; i < tableSize; i++) {
			if (strcmp(valid_locale_table[i], val) == 0) {
				new->flag = ICU_VALID;
				MUSL_LOGE("[wzxcheck] set new->flag as %{public}d", new->flag);
				break;
			}
		}

		if (new->flag == ICU_VALID) {
			if (!g_icuuc_set_icu_directory) {
				typedef void (*f_set_icu_directory)(void);
				g_icuuc_set_icu_directory = (f_set_icu_directory)find_hmicuuc_symbol("_Z17SetHwIcuDirectoryv");
				if (g_icuuc_set_icu_directory) {
					g_icuuc_set_icu_directory();
					MUSL_LOGE("[wzxcheck] SetHwIcuDirectory success");
				}
			}
		}
	}

	/* For LC_CTYPE, never return a null pointer unless the
	 * requested name was "C" or "POSIX". */
	if (!new && cat == LC_CTYPE) new = (void *)&__c_dot_utf8;

	return new;
}
