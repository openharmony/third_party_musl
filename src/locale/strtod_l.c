#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "locale_impl.h"

// #include <musl_log.h>
// #include <stdio.h>

extern void *find_hmicuuc_symbol(const char *symbol_name);
extern void *find_hmicui18n_symbol(const char *symbol_name);
static void *(*g_icu18n_unum_open)(int, void *, int32_t, const char *, void *, void *);
static void *(*g_icu18n_u_strFromUTF8)(void *, int32_t, int32_t *, const char *, int32_t, void *);
static double (*g_icu18n_unum_parseDouble)(void *, void *, int32_t, int32_t *, void*);


extern char *get_valid_icu_locale_name(const char *name);


float strtof_l(const char *restrict s, char **restrict p, locale_t l)
{
	return strtof(s, p);
}

void *icu_unum_open(char *icu_locale_name, int *cur_flag) {
	if (!g_icu18n_unum_open) {
		typedef void *(*f_icu18n_unum_open)(int, void *, int32_t, const char *, void *, void *);
		g_icu18n_unum_open = (f_icu18n_unum_open)find_hmicui18n_symbol("unum_open_72");
	}

	if (!g_icu18n_unum_open) {
		// MUSL_LOGE("[wzx_strtod_l] dlsym unum_open_72 fail");
		*cur_flag = 1;
		return NULL;
	}

	int icu_status = 0;
	void *fmt = g_icu18n_unum_open(1, NULL, -1, icu_locale_name, NULL, &icu_status);
	if (icu_status > 0) {
		// MUSL_LOGE("[wzx_strtod_l] unum_open fail %{public}d", icu_status);
		*cur_flag = -1;
		return NULL;
	}

	return fmt;
}

static void icu_char_trans(char *src, void *des, int des_size, int *cur_flag) {
	if (!g_icu18n_u_strFromUTF8) {
		typedef void *(*f_icu18n_u_strFromUTF8)(void *, int32_t, int32_t *, const char *, int32_t, void *);
		g_icu18n_u_strFromUTF8 = (f_icu18n_u_strFromUTF8)find_hmicui18n_symbol("u_strFromUTF8_72");
	}

	if (!g_icu18n_u_strFromUTF8) {
		// MUSL_LOGE("[wzx_strtod_l] dlsym u_strFromUTF8_72 fail");
		*cur_flag = 1;
		return;
	}

	int icu_status = 0;
	void *res_tmp = g_icu18n_u_strFromUTF8(des, des_size, NULL, src, -1, &icu_status);
	if (icu_status > 0) {
		// MUSL_LOGE("[wzx_strtod_l] u_strFromUTF8_72 fail %{public}d", icu_status);
		*cur_flag = -1;
		return;
	}

	return;
}

double icu_parse_double(void *fmt, void *ustr, int32_t *parse_pos, int *cur_flag) {
	if(!g_icu18n_unum_parseDouble) {
		typedef double (*f_icu18n_unum_parseDouble)(void *, void *, int32_t, int32_t *, void *);
		g_icu18n_unum_parseDouble = (f_icu18n_unum_parseDouble)find_hmicui18n_symbol("unum_parseDouble_72");
	}

	if (!g_icu18n_unum_parseDouble) {
		// MUSL_LOGE("[wzx_strtod_l] dlsym unum_parseDouble_72 fail");
		*cur_flag = 1;
		return 0;
	}

	int32_t icu_status = 0;
	double res = g_icu18n_unum_parseDouble(fmt, ustr, -1, parse_pos, &icu_status);
	if (icu_status > 0) {
		// MUSL_LOGE("[wzx_strtod_l] unum_parseDouble_72 fail %{public}d", icu_status);
		*cur_flag = -1;
		return 0;
	}
	
	return res;
}

static double icu_strtod_l(char *loc_name, char *s, int *flag, int *parse_pos) {
	char *icu_locale_name = NULL;
	icu_locale_name = get_valid_icu_locale_name(loc_name);

	void *fmt = icu_unum_open(icu_locale_name, flag);
	if (*flag == 1 || *flag == -1) {
		return 0;
	}

	int n = strlen(s);
	int ustr_size = 2*n*sizeof(char);
	void *ustr = (void *)malloc(ustr_size);
	icu_char_trans(s, ustr, ustr_size, flag);
	if (*flag == 1 || *flag == -1) {
		free(ustr);
		return 0;
	}


	double res = icu_parse_double(fmt, ustr, parse_pos, flag);
	if (*flag == 1 || *flag == -1) {
		free(ustr);
		return 0;
	}
	free(ustr);
	return res;
}

double strtod_l(const char *restrict s, char **restrict p, locale_t l)
{
	// MUSL_LOGE("[wzx_strtod_l] start strtod_l !!!! locale: %{public}s string: %{public}s", l->cat[LC_NUMERIC]->name, s);
	// printf("[wzx_strtod_l] l->cat[LC_NUMERIC_MASK]->name is %s\n", l->cat[LC_NUMERIC_MASK]->name);
	// printf("[wzx_strtod_l] original strtod_l string is |%s|\n", s);
	int flag = 1;
	if (l->cat[LC_NUMERIC]->flag == ICU_VALID) {
		flag = 0;
		int sign = 1;
		int parse_pos = 0;
		char *tmp_s = (char *)s;
		while(isspace(*tmp_s)) ++tmp_s;
		if (*tmp_s=='+' || *tmp_s=='-') {
			sign -= 2 * (*tmp_s=='-');
			++tmp_s;
		}

		// printf("[wzx_strtod_l] strtod_l string is |%s|\n", tmp_s);
		int i = 0;
		for (i=0; i<8 && (*(tmp_s+i)|32)=="infinity"[i]; i++);
		if (i==3 || i==8 || i>3) {
			if (i != 8) {
				*p = tmp_s + 3;
			} else {
				*p = tmp_s + i;
			}
			return sign * INFINITY;
		}

		double res = icu_strtod_l(l->cat[LC_NUMERIC]->name, tmp_s, &flag, &parse_pos);
		if (flag == 0 || flag == -1) {
			*p = parse_pos ? tmp_s + parse_pos : (char *)s;
			return sign * res;
		}
	}

	return strtod(s, p);
}

long double strtold_l(const char *restrict s, char **restrict p, locale_t l)
{
	return strtold(s, p);
}

weak_alias(strtof_l, __strtof_l);
weak_alias(strtod_l, __strtod_l);
weak_alias(strtold_l, __strtold_l);
