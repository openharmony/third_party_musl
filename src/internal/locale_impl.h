#ifndef _LOCALE_IMPL_H
#define _LOCALE_IMPL_H

#include <locale.h>
#include <stdlib.h>
#include "libc.h"
#include "pthread_impl.h"

#define LOCALE_NAME_MAX 23
#define VALID 2
#define INVALID 1
#define ICU_VALID 3

struct __locale_map {
	const void *map;
	size_t map_size;
	char name[LOCALE_NAME_MAX+1];
	const struct __locale_map *next;
	char flag;
};

extern hidden volatile int __locale_lock[1];

extern hidden const struct __locale_map __c_dot_utf8;
extern hidden const struct __locale_struct __c_locale;
extern hidden const struct __locale_struct __c_dot_utf8_locale;

hidden const struct __locale_map *__get_locale(int, const char *);
hidden const char *__mo_lookup(const void *, size_t, const char *);
hidden const char *__lctrans(const char *, const struct __locale_map *);
hidden const char *__lctrans_cur(const char *);
hidden const char *__lctrans_impl(const char *, const struct __locale_map *);
hidden int __loc_is_allocated(locale_t);
hidden char *__gettextdomain(void);

#ifdef FEATURE_ICU_LOCALE
typedef enum {
	ICU_UC = 0,
	ICU_I18N,
} ICU_SO_TYPE;

typedef uint16_t u_char;

hidden void *get_icu_handle(ICU_SO_TYPE type, const char *symbol_name);
hidden void get_icu_symbol(ICU_SO_TYPE type, void **icu_symbol_handle, const char *symbol_name);
hidden char *get_valid_icu_locale_name(const char *name);
hidden void *icu_unum_open(char *icu_locale_name, int *cur_status);
hidden void icu_unum_close(void *fmt);
hidden double icu_parse_double(void *fmt, u_char *ustr, int32_t *parse_pos, int *cur_status);

#define ICU_SYMBOL(name) #name"_72"
typedef void *(*f_icu18n_unum_open)(int, void *, int32_t, const char *, void *, void *);
typedef void (*f_icu18n_unum_close)(void *);
typedef void *(*f_icu18n_u_str_from_utf8)(u_char *, int32_t, int32_t *, const char *, int32_t, int *);
typedef void *(*f_icu18n_u_str_from_utf32)(u_char *, int32_t, int32_t *, const wchar_t *, int32_t, int *);
typedef double (*f_icu18n_unum_parse_double)(void *, u_char *, int32_t, int32_t *, int *);
typedef int32_t(*f_icu18n_unum_get_symbol)(const void *, int, u_char *, int32_t, int *);
typedef char *(*f_icuuc_u_austrncpy)(char *, const u_char *, int32_t);

struct icu_opt_func {
	f_icu18n_unum_open unum_open;
	f_icu18n_unum_close unum_close;
	f_icu18n_u_str_from_utf8 u_str_from_utf8;
	f_icu18n_unum_parse_double unum_parse_double;
	f_icu18n_u_str_from_utf32 u_strFrom_utf32;
	f_icu18n_unum_get_symbol unum_get_symbol;
	f_icuuc_u_austrncpy u_austrncpy;
};
extern hidden struct icu_opt_func g_icu_opt_func;

#define DLSYM_ICU_SUCC 0
#define DLSYM_ICU_FAIL 1
#define ICU_ERROR (-1)
#endif

#define LOC_MAP_FAILED ((const struct __locale_map *)-1)

#define LCTRANS(msg, lc, loc) __lctrans(msg, (loc)->cat[(lc)])
#define LCTRANS_CUR(msg) __lctrans_cur(msg)

#define C_LOCALE ((locale_t)&__c_locale)
#define UTF8_LOCALE ((locale_t)&__c_dot_utf8_locale)

#define CURRENT_LOCALE (__pthread_self()->locale)

#define CURRENT_UTF8 (!!__pthread_self()->locale->cat[LC_CTYPE])

#undef MB_CUR_MAX
#define MB_CUR_MAX (CURRENT_UTF8 ? 4 : 1)

#endif
