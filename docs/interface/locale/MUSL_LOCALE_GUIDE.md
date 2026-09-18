# musl libc Locale Interface Guide

> This document is intended for OpenHarmony application and system developers, describing usage scenarios and notes for musl libc locale-related interfaces.
> Based on ISO C99/C11 and POSIX.1-2008 standards, OpenHarmony extends the standard with an ICU delegation mechanism.

---

## 1. Locale Setting and Management

### setlocale

**Header:** `<locale.h>`

**Function Signature:** `char *setlocale(int category, const char *locale)`

**Usage:**

Sets the global locale for the current process. Applications default to the `"C"` locale at startup. Call this function to switch to a region-specific locale (e.g., `zh_HK.Big5` for Hong Kong/Macau).

- `category` specifies the locale category to affect. Commonly `LC_ALL` (all categories), or individually `LC_CTYPE` (character classification), `LC_NUMERIC` (numeric format), `LC_MONETARY` (monetary format), etc.
- `locale` is the target locale name, such as `"zh_HK.Big5"`, `"zh_CN"`, `"en_US.UTF-8"`, `"C"`, `"POSIX"`
- Returns non-NULL on success (returns the current locale name string), NULL if the locale is unavailable

**Notes:**

- `setlocale` returns NULL on failure. POSIX does not specify errno behavior in this case; do not rely on errno to determine the failure reason
- The application shall not modify the string returned.
- If the locale name contains a slash `/`, it falls back to `C.UTF-8`
- `setlocale(LC_ALL, NULL)` queries the current locale without modifying it

### newlocale

**Header:** `<locale.h>`

**Function Signature:** `locale_t newlocale(int category_mask, const char *locale, locale_t base)`

**Usage:**

Creates an independent locale object (`locale_t`) without affecting the global locale. Suitable for multi-threaded scenarios where each thread uses a different locale, or when switching locale within a function without polluting global state.

- `category_mask` specifies the category mask, commonly `LC_ALL_MASK`
- `locale` is the locale name, such as `"zh_HK.Big5"`
- `base` is typically `NULL` (create from default locale), or an existing locale_t for partial override
- Returns non-NULL on success, NULL on failure

**Notes:**

- The created `locale_t` must be freed with `freelocale` to avoid memory leaks
- The locale_t created by `newlocale` can be passed to all `*_l` suffix functions (e.g., `iswalnum_l`, `strtod_l`)
- Thread-safe: different threads hold independent locale_t objects that do not affect each other

### uselocale

**Header:** `<locale.h>`

**Function Signature:** `locale_t uselocale(locale_t loc)`

**Usage:**

Sets or queries the locale for the current thread. Unlike `setlocale` which operates on the process-level global locale, `uselocale` only affects the current thread.

- Pass a locale_t to set the current thread locale; returns the previous thread locale
- Pass `NULL` to query the current thread locale without modifying it
- Pass `LC_GLOBAL_LOCALE` to restore using the global locale (set by `setlocale`)

**Notes:**

- The locale set by `uselocale` only affects the current thread; other threads are unaffected
- The locale_t set by `uselocale` should be freed with `freelocale` when no longer needed

### freelocale

**Header:** `<locale.h>`

**Function Signature:** `void freelocale(locale_t loc)`

**Usage:**

Frees a locale_t object created by `newlocale` or `duplocale`.

**Notes:**

- Do not free `LC_GLOBAL_LOCALE` (the global locale does not need to be freed)
- Do not free a locale_t currently in use by `uselocale` (restore to `LC_GLOBAL_LOCALE` first, then free)
- Do not double-free the same locale_t

### duplocale

**Header:** `<locale.h>`

**Function Signature:** `locale_t duplocale(locale_t loc)`

**Usage:**

Duplicates a locale_t object, creating an independent copy. Useful when the same locale needs to be used in multiple contexts with independent lifecycle management.

- Pass `LC_GLOBAL_LOCALE` to duplicate the current global locale
- Returns a new locale_t, which must be freed with `freelocale`

---

## 2. Locale Information Query

### localeconv

**Header:** `<locale.h>`

**Function Signature:** `struct lconv *localeconv(void)`

**Usage:**

Queries numeric and monetary formatting information for the current locale. The returned `struct lconv` contains fields such as decimal point symbol, thousands separator, currency symbol, etc., for localized numeric display.

Common fields:
- `decimal_point`: decimal point symbol (e.g., `"."` or `","`)
- `thousands_sep`: thousands separator
- `currency_symbol`: local currency symbol (e.g., `"$"` or `"HK$"`)
- `mon_decimal_point`: monetary decimal point
- `int_curr_symbol`: international currency symbol (e.g., `"HKD "`)

**Notes:**

- The returned pointer points to musl internal static storage; **the caller does not need to free it**
- The pointer may become invalid after the next `setlocale` call (field values may change); **do not cache the pointer across `setlocale` calls**
- Under certain locales (e.g., `zh_HK.Big5`), `localeconv` returns field values reflecting Hong Kong/Macau regional formats

---

## 3. Wide Character Classification and Conversion

The following 12 `isw*_l` functions and 2 `tow*_l` functions may delegate to ICU for wide character classification/conversion under certain locales, supporting correct classification of non-ASCII characters (e.g., Chinese characters in Big5/HKSCS charset). Under other locales, they fall back to the musl built-in character classification table.

### iswalnum_l / iswalpha_l / iswblank_l / iswcntrl_l / iswdigit_l / iswgraph_l / iswlower_l / iswprint_l / iswpunct_l / iswspace_l / iswupper_l / iswxdigit_l

**Header:** `<wctype.h>`

**Function Signature:** `int isw*_l(wint_t wc, locale_t loc)`

**Usage:**

Determines the category attribute of a wide character (letter, digit, punctuation, space, control character, etc.) using the specified locale_t. Suitable for handling non-ASCII wide characters under specific locales.

- Under the `zh_HK.Big5` locale, these functions support classification of Chinese characters, fullwidth punctuation, etc. in the Big5/HKSCS charset
- Under other locales (e.g., `C` locale), the musl built-in character classification table is used, supporting only the ASCII range

**Notes:**

- The `loc` parameter must be created with `newlocale`; passing NULL or an invalid locale_t results in undefined behavior
- The functions do not set errno
- For ASCII characters ('a'-'z', 'A'-'Z', '0'-'9'), classification results are consistent across all locales

### towupper_l / towlower_l

**Header:** `<wctype.h>`

**Function Signature:** `wint_t towupper_l(wint_t wc, locale_t loc)` / `wint_t towlower_l(wint_t wc, locale_t loc)`

**Usage:**

Converts a wide character to uppercase or lowercase using the specified locale_t. Under the `zh_HK.Big5` locale, non-ASCII character case conversion is supported (e.g., fullwidth letters, Greek letters, etc.).

**Notes:**

- Returns the converted wide character; if the character has no case form, the original character is returned
- The functions do not set errno

---

## 4. Numeric Parsing

### strtod_l / wcstod_l

**Header:** `<stdlib.h>` (strtod_l), `<wchar.h>` (wcstod_l)

**Function Signatures:**
- `double strtod_l(const char *restrict s, char **restrict p, locale_t loc)`
- `double wcstod_l(const wchar_t *restrict s, wchar_t **restrict p, locale_t loc)`

**Usage:**

Parses a string (or wide string) as a double value, using the specified locale_t to determine decimal point, digit separators, and other format aspects. Suitable for parsing localized numeric strings under specific locales.

- Under the `zh_HK.Big5` locale, parsing follows Hong Kong/Macau numeric format
- `endptr` (if non-NULL) points to the position after the last parsed character; on complete parsing, it points to the terminating `\0`

**Notes:**

- On success, errno is **not guaranteed to be cleared** — the caller should explicitly set `errno = 0` before the call and check whether errno is `ERANGE` afterward to detect overflow
- Parsing `"inf"` / `"infinity"` returns `INFINITY`
- Parsing `"nan"` may return `0.0` under some locales and `NAN` under others; relying on this behavior is not recommended
- When the parsed result exceeds the double range, `INFINITY` is returned; whether `errno = ERANGE` is set depends on the specific locale
- `wcstod_l` has identical semantics to `strtod_l`, differing only in input type (wide character)

---

## 5. Encoding Conversion

### iconv_open

**Header:** `<iconv.h>`

**Function Signature:** `iconv_t iconv_open(const char *tocode, const char *fromcode)`

**Usage:**

Creates an encoding conversion handle for converting text from one character encoding to another. OpenHarmony musl includes built-in Big5/HKSCS encoding aliases (`big5`/`big5hkscs`/`cp950`, etc., case-insensitive), independent of the locale registration mechanism.

Common encoding names:
- `"UTF-8"`: UTF-8 encoding
- `"big5"` / `"bigfive"` / `"cp950"`: Big5 encoding (Traditional Chinese)
- `"big5hkscs"`: Big5-HKSCS encoding (includes Hong Kong Supplementary Character Set)

**Notes:**

- Returns a valid `iconv_t` handle on success, `(iconv_t)-1` on failure with `errno = EINVAL` (unsupported charset name)
- Compound names with locale prefixes (e.g., `"zh_HK.big5"`) are rejected; use bare charset aliases (e.g., `"big5"`)
- The handle can be reused for multiple conversions until `iconv_close` is called

### iconv

**Header:** `<iconv.h>`

**Function Signature:** `size_t iconv(iconv_t cd, char **restrict inbuf, size_t *restrict inbytesleft, char **restrict outbuf, size_t *restrict outbytesleft)`

**Usage:**

Performs encoding conversion, converting bytes from the source encoding to the target encoding and writing them to the output buffer.

- Returns `0` if conversion is complete with no irreversible conversions
- Returns a positive number if there were irreversible conversions (characters replaced with approximations)
- Returns `(size_t)-1` on error; `errno` distinguishes the error type

**Error Handling:**

| errno | Meaning | inbuf points to | Handling |
|-------|---------|-----------------|----------|
| `EILSEQ` | Invalid byte sequence in source encoding | Start of invalid character | Continue conversion from this position (skip invalid bytes or fix input) |
| `EINVAL` | Incomplete multibyte sequence at end of input | Start of incomplete sequence | Provide more input data and continue conversion |
| `E2BIG` | Output buffer space insufficient | Position of unconverted input | Enlarge output buffer and continue conversion |

**Notes:**

- On conversion failure, `inbuf` points to the error position; the output buffer already contains successfully converted partial data
- For incomplete sequences (`EINVAL`), more input can be provided and `iconv` can be called again from the current `inbuf` position without a new `iconv_open`

### iconv_close

**Header:** `<iconv.h>`

**Function Signature:** `int iconv_close(iconv_t cd)`

**Usage:**

Releases the conversion handle created by `iconv_open` and frees associated resources.

**Notes:**

- Returns `0` on success
- The handle must not be used after closing; using a closed handle is undefined behavior
- Do not close the same handle twice

---

## 6. OpenHarmony-Specific Mechanisms

### ICU Delegation Mechanism

OpenHarmony musl libc enables ICU integration at compile time via the `FEATURE_ICU_LOCALE` macro. Under certain locales, wctype character classification, case conversion, numeric parsing, and formatting operations may be delegated to the ICU library (`libhmicuuc.z.so` / `libhmicui18n.z.so`) to support correct localization behavior for non-ASCII characters.

- ICU version is dynamically obtained at runtime via `GetIcuVersion`; symbol names are concatenated with the version (e.g., `u_isalnum_73`)
- ICU data directory is set via `SetOhosIcuDirectory`, automatically triggered after successful locale registration
- Under locales where ICU delegation is not enabled, all interfaces fall back to musl built-in standard behavior (ASCII character classification table, C locale default formats)

### Locale Name Encoding Suffix Truncation

Before calling ICU functions, OpenHarmony truncates the locale name at the `.` character to extract the language-region name for ICU. For example, `zh_HK.Big5` is truncated to `zh_HK`, and `zh_CN.UTF-8` is truncated to `zh_CN`. This truncation only affects the ICU function input parameter and does not affect the full locale name returned by `setlocale` or the formatting fields returned by `localeconv`.

---

## 7. Typical Usage Examples

### Scenario 1: Setting Locale and Parsing Numeric Values for Hong Kong/Macau Applications

```c
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    /* Set Hong Kong/Macau locale */
    if (setlocale(LC_ALL, "zh_HK.Big5") == NULL) {
        fprintf(stderr, "Failed to set zh_HK.Big5 locale\n");
        return 1;
    }

    /* Create a thread-level locale_t for *_l functions */
    locale_t loc = newlocale(LC_ALL_MASK, "zh_HK.Big5", NULL);
    if (loc == NULL) {
        fprintf(stderr, "Failed to create locale_t\n");
        return 1;
    }

    /* Parse a numeric string */
    errno = 0;
    char *endptr = NULL;
    double value = strtod_l("3.14", &endptr, loc);
    if (errno == ERANGE) {
        fprintf(stderr, "Value out of range\n");
        freelocale(loc);
        return 1;
    }
    printf("Parsed value: %f\n", value);

    /* Query currency symbol */
    struct lconv *lc = localeconv();
    printf("Currency symbol: %s\n", lc->currency_symbol);

    freelocale(loc);
    return 0;
}
```

### Scenario 2: Thread-Level Locale for Wide Character Classification

```c
#include <locale.h>
#include <wctype.h>
#include <stdio.h>

void process_text(locale_t loc)
{
    /* Classify wide characters under the specified locale */
    wint_t ch = 0x4E00;  /* CJK character */
    if (iswalpha_l(ch, loc)) {
        printf("U+4E00 is a letter in this locale\n");
    }
    if (iswpunct_l(0xFF0C, loc)) {
        printf("U+FF0C (fullwidth comma) is punctuation\n");
    }
}

int main(void)
{
    /* Create an independent locale_t without affecting global locale */
    locale_t hk_loc = newlocale(LC_ALL_MASK, "zh_HK.Big5", NULL);
    if (hk_loc == NULL) {
        fprintf(stderr, "Failed to create locale_t\n");
        return 1;
    }

    process_text(hk_loc);

    freelocale(hk_loc);
    return 0;
}
```

### Scenario 3: Big5 and UTF-8 Encoding Conversion

```c
#include <iconv.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int convert_big5_to_utf8(const char *big5_in, size_t in_len, char *utf8_out, size_t out_len)
{
    iconv_t cd = iconv_open("UTF-8", "big5");
    if (cd == (iconv_t)-1) {
        fprintf(stderr, "iconv_open failed: unsupported charset\n");
        return -1;
    }

    char *inptr = (char *)big5_in;
    char *outptr = utf8_out;
    size_t inbytesleft = in_len;
    size_t outbytesleft = out_len;

    size_t result = iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft);
    if (result == (size_t)-1) {
        if (errno == EILSEQ) {
            fprintf(stderr, "Invalid Big5 sequence at offset %td\n", inptr - big5_in);
        } else if (errno == E2BIG) {
            fprintf(stderr, "Output buffer too small\n");
        }
        iconv_close(cd);
        return -1;
    }

    iconv_close(cd);
    return out_len - outbytesleft;  /* Return number of output bytes */
}

int main(void)
{
    /* Big5 encoding of the Chinese character "中" (U+4E2D) */
    char big5_input[] = { (char)0xa4, (char)0xa4, 0 };
    char utf8_output[16] = {0};

    int ret = convert_big5_to_utf8(big5_input, 2, utf8_output, sizeof(utf8_output));
    if (ret < 0) {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }

    printf("Converted %d bytes: %s\n", ret, utf8_output);
    return 0;
}
```

---

## 8. Change History

### 26.2.0

- Added `zh_HK.Big5` and `zh_HK.Big5-HKSCS` locale support
