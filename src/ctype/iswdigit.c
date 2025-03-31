#include <wctype.h>
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#include <string.h>
#include "locale_impl.h"
#endif
#endif

#undef iswdigit

int iswdigit(wint_t wc)
{
	return (unsigned)wc-'0' < 10;
}

int __iswdigit_l(wint_t c, locale_t l)
{
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
	if (icu_locale_wctype_enable && l && l->cat[LC_CTYPE]
		&& l->cat[LC_CTYPE]->flag == ICU_VALID) {
		char* type_name = (char*)(l->cat[LC_CTYPE]->name);
		if (!strcmp(type_name, "zh_CN") || !strcmp(type_name, "en_US.UTF-8")) {
			return g_icu_opt_func.u_isdigit(c);
		}
	}
#endif
#endif
	return iswdigit(c);
}

weak_alias(__iswdigit_l, iswdigit_l);
