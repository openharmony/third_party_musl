## __ASPRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __asprintf_chk - print to allocated string.

#### **SYNOPSIS**

       #include <stdio.h> 

       int __asprintf_chk(char **strp, int flag, const char *fmt, ...);

#### **DESCRIPTION**

​       Write output to a dynamically allocated string and perform parameter checking.

​       The __asprintf_chk() function allocates a sufficiently large string to store the output, including the terminating null byte ('0'), and returns a pointer to this string through the first argument. When it is no longer needed, this pointer should be passed to free to release the allocated memory.

​       If flag is set to 0, verification is not enabled; if greater than 0, verification is enabled.

#### **RETURN VALUE**


​       When successful, these functions return the number of characters printed (excluding the null byte used to end output to strings), just like sprintf.

​       If memory allocation wasn't possible, or some other error occurs, these functions will return -1, and the contents of strp are undefined.

#### **ERRORS**

​       The following error codes may be set in errno:

​       **EINVAL**: point is invalid.

​       **ENOMEM**: Insufficient memory.

​       **EOVERFLOW**: Calculate buffer overflow.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __asprintf_chk()        | Thread safety | MT-Safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *s = "This is test";
    char *buf = NULL;
    int ret = 0;

    ret = __asprintf_chk(&buf, 0, "%s\n", s);
    if (ret > 0) {
        printf("Test output: %s", buf);
        free(buf);
        buf = NULL;
    }

    ret = __asprintf_chk(&buf, 1, "%s\n", s);
    if (ret > 0) {
        printf("Test output: %s", buf);
        free(buf);
        buf = NULL;
    }

    ret = __asprintf_chk(&buf, 0, NULL);
    if (ret == -1) {
        printf("Test (error case): format=NULL, ret=%d (expected -1)\n", ret);
    }

    ret = __asprintf_chk(&buf, 1, NULL);
    if (ret == -1) {
        printf("Test (error case): format=NULL, ret=%d (expected -1)\n", ret);
    }

    return 0;
}
```


#### COLOPHON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).