## REGISTER_PRINTF_TYPE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       register_printf_type - register an extraction routine for a user-defined argument type.

#### **SYNOPSIS**

```c
#include <printf.h>

int register_printf_type(printf_va_arg_function *fct);
```

#### **DESCRIPTION**

​The register_printf_type() function registers fct as the extraction routine for a new user-defined argument type and returns a type identifier for it.

The C library only knows how to fetch arguments of the standard types (int, double, pointers and so on) from a variable argument list. A custom conversion specifier whose arguments are of a user-defined struct type must tell the C library how to fetch each argument. This is done by registering an extraction routine:

```c
typedef void printf_va_arg_function(void *, va_list *);
```

When the printf family processes a conversion whose arginfo callback declared a user-defined type, the library allocates a buffer of the size reported by arginfo, then invokes the corresponding extraction routine as fct(buffer, ap). The routine uses va_arg(3) on ap to fetch one or more arguments of the underlying C types and stores the composed value into buffer.

The returned type identifier is used by arginfo callbacks of custom specifiers: filling argtypes[j] with the identifier declares the j-th argument of that conversion to be of this user-defined type. The identifier is also accepted by parse side tooling that understands PA_* codes.

Type identifiers start at PA_LAST and increase with each registration; at most 248 types can be registered.

This interface is available only when the C library is built with musl_extended_function support. When that support is absent, the <printf.h> header and this symbol are not provided by the toolchain, so programs that use the interface fail to compile or link; there is no runtime fallback that reports an error.

#### **RETURN VALUE**

​On success, register_printf_type() returns the newly allocated type identifier, a value at or above PA_LAST.

On failure, it returns -1 and sets errno to indicate the error.

#### **ERRORS**

**EINVAL**: fct is NULL.

**ENOSPC**: The type identifier space is exhausted.

**ENOMEM**: Insufficient memory to allocate the type table.

#### ATTRIBUTES

| Interface | Attribute     | Value          |
| --------- | ------------- | -------------- |
| register_printf_type() | Thread safety | MT-Safe (registration only) |
|             | Signal safety | Not Safe       |

#### HISTORY

2026

#### NOTES

​This feature is designed specifically for when musl_extended_function is true.

Registrations should be completed during single-threaded initialization, before any printf-family calls that use the registered conversions.

The extraction routine receives the va_list by pointer and may consume several arguments (for example a long and a double) into a single value.

The extraction routine is part of the trusted boundary: it must not write past the buffer passed to it (whose size is the value reported by the arginfo callback) and must not retain the va_list beyond the call. A misbehaving routine is outside the scope of this interface.

#### IMPLEMENTATION DIFFERENCES

At most 248 types can be registered. Rationale: type identifiers share the PA_* code space above PA_LAST and are bounded by a compile-time table limit.

#### CONFORMING TO​

​This interface is a C library extension. It is provided for source compatibility with code ported from other C library implementations.

#### EXAMPLES

```c
#include <printf.h>
#include <stdarg.h>
#include <stdio.h>

struct pair {
    long i;
    double d;
};

static void pair_extract(void *mem, va_list *ap)
{
    struct pair *p = mem;
    p->i = va_arg(*ap, long);
    p->d = va_arg(*ap, double);
}

static int pair_printf(FILE *fp, const struct printf_info *info,
                       const void *const *args)
{
    const struct pair *p = *(const struct pair *const *)args[0];
    return fprintf(fp, "(%ld, %f)", p->i, p->d);
}

static int pair_arginfo(const struct printf_info *info, size_t n,
                        int *argtypes, int *size)
{
    if (n > 0) {
        argtypes[0] = pair_type;
        size[0] = sizeof(struct pair);
    }
    return 1;
}

int pair_type;

int main(void)
{
    pair_type = register_printf_type(pair_extract);
    register_printf_specifier('P', pair_printf, pair_arginfo);
    printf("%P\n", 1L, 2.0);   /* prints: (1, 2.000000) */
    return 0;
}
```

#### COLOPHON

​This page is part of the C library user-space interface documentation.
Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
