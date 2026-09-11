## REGISTER_PRINTF_MODIFIER &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       register_printf_modifier - register a custom length modifier for the printf family.

#### **SYNOPSIS**

```c
#include <printf.h>

int register_printf_modifier(const wchar_t *str);
```

#### **DESCRIPTION**

​The register_printf_modifier() function registers str as a custom length modifier and returns a bit value identifying it.

After a successful registration, when the printf family or the wide-character family parses a conversion and the characters following the flags, field width and precision match str, the modifier is consumed and the returned bit is recorded in the user field of the struct printf_info passed to the handler of the conversion. Parsing then continues normally with the conversion specifier, which may be either a built-in specifier or a specifier registered through register_printf_specifier().

A handler can inspect info->user with the returned bit to determine which modifier was present in the format:

```c
if (info->user & my_bit) { /* the custom modifier was used */ }
```

The string must be non-empty, at most 255 characters long, and consist only of characters representable as unsigned char (value at most 0xFF). Matching is performed on the exact character sequence; when several registered modifiers share the same first character, the longest match wins.

The modifier itself consumes no arguments.

At most 16 modifiers can be registered; the user field of struct printf_info is 16 bits wide.

This interface is available only when the C library is built with musl_extended_function support. When that support is absent, the <printf.h> header and this symbol are not provided by the toolchain, so programs that use the interface fail to compile or link; there is no runtime fallback that reports an error.

#### **RETURN VALUE**

​On success, register_printf_modifier() returns a positive value with exactly one bit set, corresponding to the position reserved in the user field for this modifier.

On failure, it returns -1 and sets errno to indicate the error.

#### **ERRORS**

**EINVAL**: str is NULL, empty, longer than 255 characters, or contains a character not representable as unsigned char.

**ENOSPC**: The modifier bit space is exhausted.

**ENOMEM**: Insufficient memory to allocate the modifier table.

#### ATTRIBUTES

| Interface | Attribute     | Value          |
| --------- | ------------- | -------------- |
| register_printf_modifier() | Thread safety | MT-Safe (registration only) |
|             | Signal safety | Not Safe       |

#### HISTORY

2026

#### NOTES

​This feature is designed specifically for when musl_extended_function is true.

Registrations should be completed during single-threaded initialization, before any printf-family calls that use the registered conversions.

Registered modifiers are matched before the built-in length modifiers and take precedence for an exact spelling collision. This allows an application to deliberately provide an alternative interpretation for a specific character sequence while leaving unrelated built-in behavior intact.

#### IMPLEMENTATION DIFFERENCES

At most 16 modifiers can be registered. Rationale: the user field of struct printf_info is 16 bits wide, so at most 16 single-bit values can be handed to a handler.

#### CONFORMING TO​

​This interface is a C library extension. It is provided for source compatibility with code ported from other C library implementations.

#### EXAMPLES

```c
#include <printf.h>
#include <stdio.h>

static int width_printf(FILE *fp, const struct printf_info *info,
                        const void *const *args)
{
    if (info->user & fixed_bit) {
        const long *lp = *(const long *const *)args[0];
        return fprintf(fp, "%ld", *lp);
    }
    return -2;
}

static int width_arginfo(const struct printf_info *info, size_t n,
                         int *argtypes, int *size)
{
    if (info->user & fixed_bit) {
        if (n > 0) {
            argtypes[0] = PA_POINTER;
            size[0] = sizeof(long *);
        }
        return 1;
    }
    return 0;
}

int fixed_bit;

int main(void)
{
    fixed_bit = register_printf_modifier(L"I32");
    register_printf_specifier('R', width_printf, width_arginfo);

    long value = 42;
    printf("%I32R\n", &value);   /* prints: 42 */
    return 0;
}
```

#### COLOPHON

​This page is part of the C library user-space interface documentation.
Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
