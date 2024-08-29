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
#define ICU_GET_VERSION_NUM_SYMBOL "GetIcuVersion"
#define ICU_SET_DATA_DIRECTORY_SYMBOL "SetOhosIcuDirectory"
#define ICU_UNUM_OPEN_SYMBOL "unum_open"
#define ICU_UNUM_CLOSE_SYMBOL "unum_close"
#define ICU_STR_FROM_UTF8_SYMBOL "u_strFromUTF8"
#define ICU_STR_FROM_UTF32_SYMBOL "u_strFromUTF32"
#define ICU_UNUM_PARSE_DOUBLE_SYMBOL "unum_parseDouble"
#define ICU_UNUM_GET_SYMBOL_SYMBOL "unum_getSymbol"
#define ICU_AUSTRNCPY_SYMBOL "u_austrncpy"
#ifdef FEATURE_ICU_LOCALE_TMP
#define ICU_UCHAR_ISALNUM_SYMBOL	"u_isalnum"
#define ICU_UCHAR_ISALPHA_SYMBOL 	"u_isalpha"
#define ICU_UCHAR_ISBLANK_SYMBOL 	"u_isblank"
#define ICU_UCHAR_ISCNTRL_SYMBOL 	"u_iscntrl"
#define ICU_UCHAR_ISDIGIT_SYMBOL 	"u_isdigit"
#define ICU_UCHAR_ISGRAPH_SYMBOL 	"u_isgraph"
#define ICU_UCHAR_ISLOWER_SYMBOL 	"u_islower"
#define ICU_UCHAR_ISPRINT_SYMBOL 	"u_isprint"
#define ICU_UCHAR_ISPUNCT_SYMBOL 	"u_ispunct"
#define ICU_UCHAR_ISSPACE_SYMBOL 	"u_isspace"
#define ICU_UCHAR_ISUPPER_SYMBOL 	"u_isupper"
#define ICU_UCHAR_ISXDIGIT_SYMBOL 	"u_isxdigit"
#define ICU_UCHAR_TOLOWER_SYMBOL 	"u_tolower"
#define ICU_UCHAR_TOUPPER_SYMBOL 	"u_toupper"
#endif

typedef enum {
	ICU_UC = 0,
	ICU_I18N,
} icu_so_type;

#define MAX_VALID_ICU_NAME_LEN 8
typedef uint16_t u_char;

hidden void set_icu_directory();
hidden void get_icu_symbol(icu_so_type type, void **icu_symbol_handle, const char *symbol_name);
hidden void get_valid_icu_locale_name(const char *name, const char *icu_name, int icu_name_len);
hidden void *icu_unum_open(char *icu_locale_name, int *cur_status);
hidden void icu_unum_close(void *fmt);
hidden double icu_parse_double(void *fmt, u_char *ustr, int32_t *parse_pos, int *cur_status);

typedef char *(*f_icuuc_get_icu_version)(void);
typedef void (*f_icuuc_u_set_data_directory)(void);
typedef void *(*f_icu18n_unum_open)(int, void *, int32_t, const char *, void *, void *);
typedef void (*f_icu18n_unum_close)(void *);
typedef void *(*f_icu18n_u_str_from_utf8)(u_char *, int32_t, int32_t *, const char *, int32_t, int *);
typedef void *(*f_icu18n_u_str_from_utf32)(u_char *, int32_t, int32_t *, const wchar_t *, int32_t, int *);
typedef double (*f_icu18n_unum_parse_double)(void *, u_char *, int32_t, int32_t *, int *);
typedef int32_t(*f_icu18n_unum_get_symbol)(const void *, int, u_char *, int32_t, int *);
typedef char *(*f_icuuc_u_austrncpy)(char *, const u_char *, int32_t);
#ifdef FEATURE_ICU_LOCALE_TMP
typedef int(*f_icu18n_u_isalnum)(int c);
typedef int(*f_icu18n_u_isalpha)(int c);
typedef int(*f_icu18n_u_isblank)(int c);
typedef int(*f_icu18n_u_iscntrl)(int c);
typedef int(*f_icu18n_u_isdigit)(int c);
typedef int(*f_icu18n_u_isgraph)(int c);
typedef int(*f_icu18n_u_islower)(int c);
typedef int(*f_icu18n_u_isprint)(int c);
typedef int(*f_icu18n_u_ispunct)(int c);
typedef int(*f_icu18n_u_isspace)(int c);
typedef int(*f_icu18n_u_isupper)(int c);
typedef int(*f_icu18n_u_isxdigit)(int c);
typedef int(*f_icu18n_u_tolower)(int c);
typedef int(*f_icu18n_u_toupper)(int c);
#endif

struct icu_opt_func {
	f_icuuc_get_icu_version get_icu_version;
	f_icuuc_u_set_data_directory set_data_directory;
	f_icu18n_unum_open unum_open;
	f_icu18n_unum_close unum_close;
	f_icu18n_u_str_from_utf8 u_str_from_utf8;
	f_icu18n_u_str_from_utf32 u_str_from_utf32;
	f_icu18n_unum_parse_double unum_parse_double;
	f_icu18n_unum_get_symbol unum_get_symbol;
	f_icuuc_u_austrncpy u_austrncpy;
#ifdef FEATURE_ICU_LOCALE_TMP
	f_icu18n_u_isalnum u_isalnum;
	f_icu18n_u_isalpha u_isalpha;
	f_icu18n_u_isblank u_isblank;
	f_icu18n_u_iscntrl u_iscntrl;
	f_icu18n_u_isdigit u_isdigit;
	f_icu18n_u_isgraph u_isgraph;
	f_icu18n_u_islower u_islower;
	f_icu18n_u_isprint u_isprint;
	f_icu18n_u_ispunct u_ispunct;
	f_icu18n_u_isspace u_isspace;
	f_icu18n_u_isupper u_isupper;
	f_icu18n_u_isxdigit u_isxdigit;
	f_icu18n_u_tolower u_tolower;
	f_icu18n_u_toupper u_toupper;
#endif
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
