#include <wctype.h>
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#include <string.h>
#include "locale_impl.h"
#endif
#endif

int iswxdigit(wint_t wc)
{
	return (unsigned)(wc-'0') < 10 || (unsigned)((wc|32)-'a') < 6;
}

int __iswxdigit_l(wint_t c, locale_t l)
{
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
	if (icu_locale_wctype_enable && l && l->cat[LC_CTYPE]
		&& l->cat[LC_CTYPE]->flag == ICU_VALID) {
		char* type_name = (char*)(l->cat[LC_CTYPE]->name);
		if (!strcmp(type_name, "zh_CN") || !strcmp(type_name, "en_US.UTF-8")) {
			return g_icu_opt_func.u_isxdigit(c);
		}
	}
#endif
#endif
	return iswxdigit(c);
}

weak_alias(__iswxdigit_l, iswxdigit_l);
