#include <wchar.h>
#include <wctype.h>
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
#include <string.h>
#include "locale_impl.h"
#endif
#endif

/* Our definition of whitespace is the Unicode White_Space property,
 * minus non-breaking spaces (U+00A0, U+2007, and U+202F) and script-
 * specific characters with non-blank glyphs (U+1680 and U+180E). */

int iswspace(wint_t wc)
{
	static const wchar_t spaces[] = {
		' ', '\t', '\n', '\r', 11, 12,  0x0085,
		0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005,
		0x2006, 0x2008, 0x2009, 0x200a,
		0x2028, 0x2029, 0x205f, 0x3000, 0
	};
	return wc && wcschr(spaces, wc);
}

int __iswspace_l(wint_t c, locale_t l)
{
#ifndef __LITEOS__
#ifdef FEATURE_ICU_LOCALE
	if (icu_locale_wctype_enable && l && l->cat[LC_CTYPE]
		&& l->cat[LC_CTYPE]->flag == ICU_VALID) {
		char* type_name = (char*)(l->cat[LC_CTYPE]->name);
		if (!strcmp(type_name, "zh_CN") || !strcmp(type_name, "en_US.UTF-8")) {
			return g_icu_opt_func.u_isspace(c);
		}
	}
#endif
#endif
	return iswspace(c);
}

weak_alias(__iswspace_l, iswspace_l);
