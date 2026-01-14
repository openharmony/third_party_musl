## __VASPRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __vasprintf_chk - print to allocated string

#### **SYNOPSIS**

       #include <stdio.h> 

       int __vasprintf_chk(char **strp, int flag, const char *fmt, va_list ap);

#### **DESCRIPTION**

​       Write output to stdout and perform parameter checking. 

​       The __vasprintf_chk() function allocates a sufficiently large string to store the output, including the terminating null byte ('0'), and returns a pointer to this string through the first argument. When it is no longer needed, this pointer should be passed to free to release the allocated memory.

​       If flag is set to 0, verification is not enabled; if greater than 0, verification is enabled.  

#### **RETURN VALUE**


​       When  successful,  these functions return the number of bytes printed, just like sprintf.  If memory allocation wasn't possible, or some other error occurs, these functions will return -1, and the contents of strp are undefined.

#### **ERRORS**

​       The following error codes may be set in errno:

​       **EINVAL**: point is invalid.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __vasprintf_chk()       | Thread safety | MT-Safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static int VasprintfHelper(char** strp, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __vasprintf_chk(strp, 0, format, args);
    va_end(args);
    return result;
}

int main(void)
{
    char *s = "This is test";
    int i = 50;
    float f = 50.0f;
    char *buf = NULL;
    int ret = 0;

    ret = VasprintfHelper(&buf, "%s\n", s);
    if (ret > 0) {
        printf("Test output: %s", buf);
        free(buf);
        buf = NULL;
    }

    ret = VasprintfHelper(&buf, NULL, s);
    if (ret == -1) {
        printf("Test (error case): format=NULL, ret=%d (expected -1)\n", ret);
    }
    return 0;
}
```


#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)