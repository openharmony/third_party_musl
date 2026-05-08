## __OPEN_2 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __open_2 - open a file through the fortified two-argument open path.

#### **SYNOPSIS**

       #include <fcntl.h>

       int __open_2(const char *pathname, int flags);

#### **DESCRIPTION**

​       The __open_2() function is a fortify helper used for the two-argument form
​       of open(). It is primarily intended for compiler-generated fortify call paths
​       rather than direct application use.

​       When **flags** does not require a file mode, __open_2() forwards the request
​       to open() and returns the resulting file descriptor or error.

​       When **flags** contains **O_CREAT** or **O_TMPFILE**, the call is considered
​       invalid because the two-argument form of open() does not provide the required
​       file mode. In that case, __open_2() reports a Musl Fortify runtime error and
​       aborts the process instead of returning an error code.

#### **PARAMETERS**

​       **pathname**: Path of the file to be opened.

​       **flags**: Open flags passed to open(). The two-argument fortified path is
​       valid only when the selected flags do not require a mode argument.

#### **RETURN VALUE**

​       On success, __open_2() returns a nonnegative file descriptor.

​       On failure from the underlying open() call, -1 is returned and errno is set
​       as described for open().

​       If musl is built without **musl_extended_function**, the current
​       implementation returns -1 and sets errno to **ENOSYS**.

#### **ERRORS**

​       The function may fail with any error reported by open().

​       **ENOSYS**: The fortified external interface is not enabled in the current
​       musl build.

​       If **flags** contains **O_CREAT** or **O_TMPFILE**, __open_2() does not
​       return an errno value. It emits a Musl Fortify runtime error and terminates
​       the process.

#### **ATTRIBUTES**

| Interface       | Attribute     | Value    |
| --------------- | ------------- | -------- |
| __open_2()      | Thread safety | MT-safe  |
|                 | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

​       This feature is designed specifically for when musl_extended_function is
​       true.

​       This interface is mainly a glibc-compatible fortify helper. Normal
​       application code should prefer calling open() directly.

#### **CONFORMING TO**

​       This is a platform-specific extension and is not part of any POSIX
​       standard.

#### **EXAMPLES**

```c
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd = __open_2("/dev/null", O_RDONLY);
    if (fd < 0) {
        perror("__open_2");
        return 1;
    }
    close(fd);

    /*
     * Invalid usage: the two-argument fortified path cannot be used when
     * O_CREAT or O_TMPFILE requires a mode argument.
     * Expected result: Musl Fortify runtime error followed by process abort.
     */
    /* __open_2("/tmp/example", O_CREAT); */

    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
