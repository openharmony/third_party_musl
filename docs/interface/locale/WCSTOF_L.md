## WCSTOF_L        OHOS Programmer's Manual

#### **NAME**

wcstof_l - convert a wide-character string to a float; the locale argument is currently ignored.

#### **SYNOPSIS**

```c
#include <wchar.h>
#include <locale.h>

float wcstof_l(const wchar_t *nptr, wchar_t **endptr, locale_t loc);
```

#### **DESCRIPTION**

The wcstof_l() function converts the initial part of the wide-character string pointed to by nptr to a float value.

When musl_extended_function support is enabled, this implementation directly reuses wcstof(). The loc argument is accepted for source compatibility, but it is not consulted during parsing. Locale-specific decimal separators, grouping characters, and alternative digits are not supported.

If endptr is not NULL, wcstof_l() stores in *endptr the address of the first unrecognized wide character. If no conversion is performed, the function returns 0 and stores nptr in *endptr.

#### **RETURN VALUE**

On success, wcstof_l() returns the converted float value.

If no conversion can be performed, 0 is returned.

If the implementation is built without musl_extended_function support, wcstof_l() returns 0.0f and sets errno to ENOSYS.

#### **ERRORS**

The following error codes may be set in errno:

**ENOSYS**: The interface is unavailable because musl_extended_function is disabled.

**ERANGE**: The converted value would overflow or underflow the representable float range.

#### ATTRIBUTES

| Interface   | Attribute     | Value          |
| ----------- | ------------- | -------------- |
| wcstof_l()  | Thread safety | MT-Safe        |
|             | Signal safety | Not Safe       |

#### HISTORY

2026

#### NOTES

This feature is designed specifically for when musl_extended_function is true.

The current implementation is a compatibility wrapper over wcstof(). The parsing result, endptr update, and errno behavior therefore follow wcstof().

#### CONFORMING TO

wcstof_l() is a locale-specific extension and is not part of ISO C. It is commonly provided by GNU and BSD systems.

#### EXAMPLES

```c
#define _GNU_SOURCE
#include <locale.h>
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t *end = NULL;
    locale_t loc = newlocale(LC_ALL_MASK, "C", NULL);
    float value = wcstof_l(L"12.5tail", &end, loc);

    wprintf(L"value=%f rest=%ls\n", value, end);
    freelocale(loc);
    return 0;
}
```

#### COLOPHON

This page is part of the C library user-space interface documentation.
Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
