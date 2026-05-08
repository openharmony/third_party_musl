## __EXPLICIT_BZERO_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __explicit_bzero_chk - clear memory with fortify buffer-size validation.

#### **SYNOPSIS**

       #include <string.h>

       void __explicit_bzero_chk(void *dest, size_t len, size_t dst_len);

#### **DESCRIPTION**

​       The __explicit_bzero_chk() function is a fortify-style checked helper that
​       verifies that the requested clear length does not exceed the size of the
​       destination object known by the caller, and then clears the requested number
​       of bytes.

​       Unlike a plain memset() call, explicit_bzero() is intended for clearing
​       sensitive data in a way that is harder for the compiler to optimize away.

​       If **len** is greater than **dst_len**, __explicit_bzero_chk() reports a
​       Musl Fortify runtime error and aborts the process instead of performing the
​       write.

#### **PARAMETERS**

​       **dest**: Destination buffer to be cleared.

​       **len**: Number of bytes to clear starting at **dest**.

​       **dst_len**: Size of the destination object used for runtime overflow
​       checking.

#### **RETURN VALUE**

​       The __explicit_bzero_chk() function does not return a value.

​       If musl is built without **musl_extended_function**, the current
​       implementation leaves the destination unchanged, sets errno to **ENOSYS**,
​       and returns.

#### **ERRORS**

​       **ENOSYS**: The fortified external interface is not enabled in the current
​       musl build.

​       If **len** is greater than **dst_len**, __explicit_bzero_chk() does not
​       return an errno value. It emits a Musl Fortify runtime error and terminates
​       the process.

#### **ATTRIBUTES**

| Interface                  | Attribute     | Value    |
| -------------------------- | ------------- | -------- |
| __explicit_bzero_chk()     | Thread safety | MT-safe  |
|                            | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

​       This feature is designed specifically for when musl_extended_function is
​       true.

​       This interface is mainly provided for glibc-compatible checked-symbol
​       support and for code that explicitly calls the helper.

​       In the current musl tree, normal explicit_bzero() calls are not
​       automatically redirected to __explicit_bzero_chk(). Application code should
​       continue to use explicit_bzero() unless it specifically needs the checked
​       helper symbol.

#### **CONFORMING TO**

​       This is a platform-specific extension and is not part of any POSIX
​       standard.

#### **EXAMPLES**

```c
#include <stdio.h>
#include <string.h>

static void dump_bytes(const unsigned char *buf, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%u ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    unsigned char secret[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    __explicit_bzero_chk(secret, 4, sizeof(secret));
    dump_bytes(secret, sizeof(secret));

    /*
     * Invalid usage.
     * Expected result: Musl Fortify runtime error followed by process abort.
     */
    /* __explicit_bzero_chk(secret, 16, sizeof(secret)); */

    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
