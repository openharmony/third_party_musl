#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include "locale_impl.h"

// #include <musl_log.h>
// #include <stdio.h>

extern void *find_hmicui18n_symbol(const char *symbol_name);

static void *(*g_icu18n_u_strFromUTF32)(void *, int32_t, int32_t *, void *, int32_t, void *);


extern char *get_valid_icu_locale_name(const char *name);


extern void *icu_unum_open(char *icu_locale_name, int *cur_flag);

static void icu_wchar_trans(wchar_t *src, void *des, int des_size, int *cur_flag) {
	if (!g_icu18n_u_strFromUTF32) {
		typedef void *(*f_icu18n_u_strFromUTF32)(void *, int32_t, int32_t *, void *, int32_t, void *);
		g_icu18n_u_strFromUTF32 = (f_icu18n_u_strFromUTF32)find_hmicui18n_symbol("u_strFromUTF32_72");
	}

	if (!g_icu18n_u_strFromUTF32) {
		// MUSL_LOGE("[wzx_strtod_l] dlsym u_strFromUTF32_72 fail\n");
		*cur_flag = 1;
		return;
	}

	int icu_status = 0;
	void *res_tmp = g_icu18n_u_strFromUTF32(des, des_size, NULL, src, -1, &icu_status);
	if (icu_status > 0) {
		// MUSL_LOGE("[wzx_strtod_l] u_strFromUTF32_72 fail %{public}d\n", icu_status);
		*cur_flag = -1;
		return;
	}

	return;
}

extern double icu_parse_double(void *fmt, void *ustr, int32_t *parse_pos, int *cur_flag);

double icu_wcstod_l(char *loc_name, wchar_t *s, int *flag, int *parse_pos) {
	char *icu_locale_name = NULL;
	icu_locale_name = get_valid_icu_locale_name(loc_name);

	void *fmt = icu_unum_open(icu_locale_name, flag);
	if (*flag == 1 || *flag == -1) {
		return 0;
	}

	int n = wcslen(s);
	int ustr_size = n*sizeof(wchar_t);
	void *ustr = (void *)malloc(ustr_size);
	icu_wchar_trans(s, ustr, ustr_size, flag);
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

double wcstod_l (const wchar_t *restrict s, wchar_t **restrict p, locale_t l)
{
	// printf("[wzx_strtod_l] l->cat[LC_NUMERIC_MASK]->name is %s\n", l->cat[LC_NUMERIC]->name);
	// wprintf(L"[wzx_strtod_l] original wcstod_l string is |%ls|\n", s);
	int flag = 1;
	if (l->cat[LC_NUMERIC]->flag == ICU_VALID) {
		flag = 0;
		int sign = 1;
		int parse_pos = 0;
		wchar_t *tmp_s = (wchar_t *)s;
		while(iswspace(*tmp_s)) ++tmp_s;
		if (*tmp_s==L'+' || *tmp_s==L'-') {
			sign -= 2 * (*tmp_s==L'-');
			++tmp_s;
		}

		// wprintf("[wzx_strtod_l] wcstod_l string is |%ls|\n", tmp_s);
		int i = 0;
		for (i=0; i<8 && towlower(*(tmp_s+i))==L"infinity"[i]; i++);
		if (i==3 || i==8 || i>3) {
			if (i != 8) {
				*p = tmp_s + 3;
			} else {
				*p = tmp_s + i;
			}
			return sign * INFINITY;
		}

		double res = icu_wcstod_l(l->cat[LC_NUMERIC]->name, tmp_s, &flag, &parse_pos);
		if (flag == 0 || flag == -1) {
			*p = parse_pos ? tmp_s + parse_pos : (char *)s;
			return sign * res;
		}
	}

	return wcstod(s, p);
}
