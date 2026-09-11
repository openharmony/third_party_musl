## REGISTER_PRINTF_SPECIFIER &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       register_printf_specifier - register a custom conversion specifier for the printf family.

#### **SYNOPSIS**

```c
#include <printf.h>

int register_printf_specifier(int spec, printf_function *func,
                              printf_arginfo_size_function *arginfo);
```

#### **DESCRIPTION**

​The register_printf_specifier() function binds the conversion specifier character spec to the user-supplied callbacks func and arginfo.

After a successful registration, whenever the printf family (printf, fprintf, sprintf, snprintf, dprintf, asprintf and their v-variants) or the wide-character family (wprintf, fwprintf, swprintf and their v-variants) encounters spec at a conversion position, and spec is not a built-in conversion specifier, output for that conversion is delegated to func.

The func callback produces the output for one conversion:

```c
typedef int printf_function(FILE *, const struct printf_info *,
                            const void *const *);
```

It receives the output stream, a printf_info structure describing the parsed flags, width, precision and length modifiers, and an argument vector. It returns the number of characters written, a negative value to report an error, or -2 to decline the conversion (the conversion is then treated as an invalid format).

The arginfo callback declares how many arguments the conversion consumes and their types:

```c
typedef int printf_arginfo_size_function(const struct printf_info *,
                                         size_t n, int *, int *);
```

It is called in two phases. In the first phase the library calls it with n set to 0 and NULL array pointers; only the return value is used, to obtain the number of arguments the conversion consumes. If that number is positive, the library calls it a second time with n set to that number and the argtypes and size arrays; in this call it fills up to n entries of both arrays and returns the number of arguments actually consumed. Each argtypes entry is a PA_* type code, optionally combined with PA_FLAG_* bits; a value at or above PA_LAST without flag bits identifies a type registered through register_printf_type(). Entries of size are only consulted for such user-registered types and carry the storage size in bytes.

Before the handler is invoked, all declared types are validated; if any entry is not a usable type, the conversion fails with EINVAL and no argument is consumed from the argument list.

A registered specifier never overrides a built-in conversion specifier: built-in specifiers are always processed by the C library itself. Registration of a built-in character succeeds but has no effect at format-processing time.

Registering the same spec again replaces the previous binding.

This interface is available only when the C library is built with musl_extended_function support. When that support is absent, the <printf.h> header and this symbol are not provided by the toolchain, so programs that use the interface fail to compile or link; there is no runtime fallback that reports an error.

#### **RETURN VALUE**

​On success, register_printf_specifier() returns 0.

On failure, it returns -1 and sets errno to indicate the error.

#### **ERRORS**

**EINVAL**: spec is out of the range [0, 255], or func or arginfo is NULL.

**ENOMEM**: Insufficient memory to allocate the registration table.

#### ATTRIBUTES

| Interface | Attribute     | Value          |
| --------- | ------------- | -------------- |
| register_printf_specifier() | Thread safety | MT-Safe (registration only) |
|             | Signal safety | Not Safe       |

Registrations are serialized among themselves, but the printf family reads the registered tables without locking; see NOTES.

#### HISTORY

2026

#### NOTES

​This feature is designed specifically for when musl_extended_function is true.

Registrations should be completed during single-threaded initialization, before any printf-family calls that use the registered conversions.

A single conversion may declare at most 32 arguments.

Positional argument specifications (%N$) cannot be combined with registered specifiers in the same format string; such a format is rejected with EINVAL.

The conversion specifier must not be a built-in specifier; registering one has no effect at format-processing time.

The arginfo callback is part of the trusted boundary: it must not write more entries than the capacity passed in n, and it must not touch the arrays when n is 0, since the first-phase call passes NULL pointers. Writing past n overflows internal stack arrays; the C library cannot defend against a misbehaving or hostile callback, and such callbacks are outside the scope of this interface.

#### IMPLEMENTATION DIFFERENCES

The following behaviors are deliberate implementation choices:

Built-in specifiers take precedence. Registering a built-in character (for example 'd') succeeds but has no effect at format-processing time. Rationale: printf is process-global infrastructure; allowing one module to silently redefine the basic conversions would change the output of every other module in the process and bypass hardening checks. Custom (non-built-in) specifiers are unaffected.

Positional argument specifications (%N$) cannot be combined with registered specifiers in the same format string; EINVAL is returned. Rationale: positional processing requires every conversion's argument types to be known during a preliminary scan, and the types of a registered conversion are only reported by its arginfo callback at processing time. Positional specifications in general support single-digit indexes in the range %1$ through %9$ on this platform, for both built-in and registered conversions.

The i18n and is_binary128 fields of struct printf_info are always 0. Rationale: this platform provides no locale-driven digit grouping mechanism and no binary128 floating-point support, so there is no condition under which these flags could be set. Handlers ported from other environments must not rely on them.

A conversion may declare at most 32 arguments. Rationale: fixed upper bound keeps argument collection on the stack, avoiding allocation on the printf hot path.

#### CONFORMING TO​

​This interface is a C library extension. It is provided for source compatibility with code ported from other C library implementations.

#### EXAMPLES

```c
#include <printf.h>
#include <stdio.h>

static int temp_printf(FILE *fp, const struct printf_info *info,
                       const void *const *args)
{
    double value = *(const double *)args[0];
    return fprintf(fp, "%g degrees", value);
}

static int temp_arginfo(const struct printf_info *info, size_t n,
                        int *argtypes, int *size)
{
    if (n > 0) {
        argtypes[0] = PA_DOUBLE;
        size[0] = sizeof(double);
    }
    return 1;
}

int main(void)
{
    register_printf_specifier('T', temp_printf, temp_arginfo);
    printf("now %T\n", 36.5);   /* prints: now 36.5 degrees */
    return 0;
}
```

#### COLOPHON

​This page is part of the C library user-space interface documentation.
Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
