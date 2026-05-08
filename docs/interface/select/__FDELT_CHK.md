## __FDELT_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __fdelt_chk - validate a file descriptor and compute its fd_set word index.

#### **SYNOPSIS**

       #include <sys/select.h>

       long int __fdelt_chk(long int fd);

#### **DESCRIPTION**

​       The __fdelt_chk() function checks whether **fd** is within the valid
​       **fd_set** range and, if so, returns the array index of the word that
​       contains the corresponding bit.

​       This helper is used by fortified select-related paths and glibc-compatible
​       code. For a valid descriptor, the result is equivalent to the word index
​       computed from the descriptor value.

​       If **fd** is negative or greater than or equal to **FD_SETSIZE**,
​       __fdelt_chk() reports a Musl Fortify runtime error and aborts the process.

#### **PARAMETERS**

​       **fd**: File descriptor whose position inside an **fd_set** is to be
​       checked and translated.

#### **RETURN VALUE**

​       On success, __fdelt_chk() returns the index of the **fds_bits** element that
​       stores the bit for **fd**.

​       If musl is built without **musl_extended_function**, the current
​       implementation returns -1 and sets errno to **ENOSYS**.

#### **ERRORS**

​       **ENOSYS**: The fortified external interface is not enabled in the current
​       musl build.

​       If **fd** is outside the valid **fd_set** range, __fdelt_chk() does not
​       return an errno value. It emits a Musl Fortify runtime error and terminates
​       the process.

#### **ATTRIBUTES**

| Interface         | Attribute     | Value    |
| ----------------- | ------------- | -------- |
| __fdelt_chk()     | Thread safety | MT-safe  |
|                   | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

​       This feature is designed specifically for when musl_extended_function is
​       true.

​       This interface is usually reached through fortified macros or compatibility
​       code rather than by direct application calls.

#### **CONFORMING TO**

​       This is a platform-specific extension and is not part of any POSIX
​       standard.

#### **EXAMPLES**

```c
#include <stdio.h>
#include <sys/select.h>

int main(void)
{
    long int first = __fdelt_chk(0);
    long int last = __fdelt_chk(FD_SETSIZE - 1);

    printf("word index for fd 0: %ld\n", first);
    printf("word index for fd %d: %ld\n", FD_SETSIZE - 1, last);

    /*
     * Invalid usage.
     * Expected result: Musl Fortify runtime error followed by process abort.
     */
    /* __fdelt_chk(FD_SETSIZE); */

    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
