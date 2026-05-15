## QSORT_R        OHOS Programmer's Manual

#### **NAME**

qsort_r - sort an array with a user-supplied comparator context.

#### **SYNOPSIS**

```c
#define _GNU_SOURCE
#include <stdlib.h>

void qsort_r(void *base, size_t nel, size_t width,
    int (*cmp)(const void *, const void *, void *), void *arg);
```

#### **DESCRIPTION**

The qsort_r() function sorts the array pointed to by base, which contains nel elements, each of width bytes.

The comparison function cmp is called with pointers to two array elements and the user-provided context pointer arg. The comparison function must return an integer less than, equal to, or greater than zero if the first element is considered respectively less than, equal to, or greater than the second element.

If nel is 0 or 1, qsort_r() returns immediately and the comparison function is not called.

The relative order of elements that compare equal is unspecified. This implementation does not provide a stable sort.

This implementation exposes the GNU-style qsort_r() interface, where the context pointer is passed as the last argument to cmp. BSD systems commonly provide a different qsort_r() prototype.

If the implementation is built without musl_extended_function support, qsort_r() does not perform sorting and sets errno to ENOSYS.

#### **RETURN VALUE**

The qsort_r() function returns no value.

#### **ERRORS**

The following error code may be set in errno:

**ENOSYS**: The interface is unavailable because musl_extended_function is disabled.

#### ATTRIBUTES

| Interface | Attribute     | Value    |
| --------- | ------------- | -------- |
| qsort_r() | Thread safety | MT-safe  |
|           | Signal safety | Not Safe |

#### HISTORY

2026

#### NOTES

This feature is designed specifically for when musl_extended_function is true.

To make the declaration visible from `<stdlib.h>`, define `_GNU_SOURCE` or `_BSD_SOURCE` before including the header.

The comparison function must impose a consistent ordering on the elements being sorted. Inconsistent comparator results lead to unspecified behavior.

#### CONFORMING TO

qsort_r() is a GNU extension and is not specified by ISO C or POSIX.

#### EXAMPLES

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

struct sort_ctx {
    int descending;
};

static int compare_ints(const void *left, const void *right, void *arg)
{
    const int *lhs = left;
    const int *rhs = right;
    const struct sort_ctx *ctx = arg;
    int ret = (*lhs > *rhs) - (*lhs < *rhs);
    return ctx->descending ? -ret : ret;
}

int main(void)
{
    int values[] = {9, 2, 7, 11, 3};
    struct sort_ctx ctx = { .descending = 1 };
    size_t i;

    qsort_r(values, sizeof(values) / sizeof(values[0]), sizeof(values[0]), compare_ints, &ctx);

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        printf("%d ", values[i]);
    }
    printf("\n");
    return 0;
}
```

#### COLOPHON

This page is part of the C library user-space interface documentation.
Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
