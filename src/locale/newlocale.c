#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "locale_impl.h"
#include "lock.h"
#include <dlfcn.h>
// #include "musl_log.h"


#define malloc __libc_malloc
#define calloc undef
#define realloc undef
#define free undef

static int default_locale_init_done;
static struct __locale_struct default_locale, default_ctype_locale;

int __loc_is_allocated(locale_t loc)
{
	return loc && loc != C_LOCALE && loc != UTF8_LOCALE
		&& loc != &default_locale && loc != &default_ctype_locale;
}

static locale_t do_newlocale(int mask, const char *name, locale_t loc)
{
	struct __locale_struct tmp;

	for (int i=0; i<LC_ALL; i++) {
		tmp.cat[i] = (!(mask & (1<<i)) && loc) ? loc->cat[i] :
			__get_locale(i, (mask & (1<<i)) ? name : "");
		if (tmp.cat[i] == LOC_MAP_FAILED || tmp.cat[i] && tmp.cat[i]->flag == INVALID) {
			if (tmp.cat[i]->flag == INVALID) {
				// MUSL_LOGE("[wzxcheck] INVALID");
			} else {
				// MUSL_LOGE("[wzxcheck] LOC_MAP_FAILED");
			}
			return 0;
		}
			
	}

	/* For locales with allocated storage, modify in-place. */
	if (__loc_is_allocated(loc)) {
		*loc = tmp;
		return loc;
	}

	/* Otherwise, first see if we can use one of the builtin locales.
	 * This makes the common usage case for newlocale, getting a C locale
	 * with predictable behavior, very fast, and more importantly, fail-safe. */
	if (!memcmp(&tmp, C_LOCALE, sizeof tmp)) return C_LOCALE;
	if (!memcmp(&tmp, UTF8_LOCALE, sizeof tmp)) return UTF8_LOCALE;

	/* And provide builtins for the initial default locale, and a
	 * variant of the C locale honoring the default locale's encoding. */
	if (!default_locale_init_done) {
		for (int i=0; i<LC_ALL; i++)
			default_locale.cat[i] = __get_locale(i, "");
		default_ctype_locale.cat[LC_CTYPE] = default_locale.cat[LC_CTYPE];
		default_locale_init_done = 1;
	}
	if (!memcmp(&tmp, &default_locale, sizeof tmp)) return &default_locale;
	if (!memcmp(&tmp, &default_ctype_locale, sizeof tmp))
		return &default_ctype_locale;

	/* If no builtin locale matched, attempt to allocate and copy. */
	if ((loc = malloc(sizeof *loc))) *loc = tmp;

	return loc;
}

static void *g_hmicuuc_handle = NULL;
static void *g_hmicui18n_handle = NULL;

void *find_hmicuuc_symbol(const char *symbol_name) {
  if (!g_hmicuuc_handle) {
	g_hmicuuc_handle = dlopen("libhmicuuc.z.so", RTLD_LOCAL);
	// MUSL_LOGE("[wzxcheck] dlopen libhmicuuc.z.so success");
  }
  return g_hmicuuc_handle ? dlsym(g_hmicuuc_handle, symbol_name) : NULL;
}

void *find_hmicui18n_symbol(const char *symbol_name) {
  if (!g_hmicui18n_handle) {
	g_hmicui18n_handle = dlopen("libhmicui18n.z.so", RTLD_LOCAL);
	// MUSL_LOGE("[wzxcheck] dlopen libhmicui18n.z.so success");
  }
  return g_hmicui18n_handle ? dlsym(g_hmicui18n_handle, symbol_name) : NULL;
}


locale_t __newlocale(int mask, const char *name, locale_t loc)
{	
	LOCK(__locale_lock);
	// MUSL_LOGE("[wzxcheck] start __newlocale !!!! %{public}d, %{public}s", mask, name);
	loc = do_newlocale(mask, name, loc);
	UNLOCK(__locale_lock);
	return loc;
}

weak_alias(__newlocale, newlocale);
