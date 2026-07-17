## __PRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __printf_chk - Write output to stdout.

#### **SYNOPSIS**


       #include <stdio.h>

       int __printf_chk(int flags, const char* format, ...);

#### **DESCRIPTION**

​       Write output to stdout and perform parameter checking. 

​       It expects as input an int-type flags is used to specify the security check level, a "format string" that specifies what to print, and zero or more subsequent arguments. The format string can optionally contain "conversion specifications", placeholders that begin with % that specify how to format the function’s subsequent arguments, if any.

​       If flags is set to 0 or a negative value, verification is not enabled; if flags is greater than 0, verification is enabled. When enabled, format string validation is performed: if positional parameter references (e.g., %n$d) are non-contiguous, the program will abort with a "Musl Fortify runtime error: invalid specified parameter" message.

#### **RETURN VALUE**

​       Upon successful return, this function return the number of characters printed (excluding the null byte used to end output to strings).

​       If an output error is encountered, a negative value is returned.

#### **ERRORS**

​       The following error codes may be set in errno:

​       **EINVAL**: format is NULL. The function sets errno to EINVAL and returns -1.

​       **EOVERFLOW**: formatted output length or width calculation overflows.

​       When flags is greater than 0 (fortify mode), if the format string contains positional parameter references that are non-contiguous (e.g., "%3$d" skips positions 1 and 2), the program will abort with the message "Musl Fortify runtime error: invalid specified parameter".

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __printf_chk() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>

int main(void)
{
    const char *s = "This is test";

    int ret = __printf_chk(0, "%s\n", s);
    if (ret < 0) {
        perror("__printf_chk failed.");
        return -1;
    }

    ret = __printf_chk(1, "%s\n", s);
    if (ret < 0) {
        perror("__printf_chk failed.");
        return -1;
    }

    /* WARNING: the following call causes abort when flags>0.
       "%3$d" references position 3 while positions 1 and 2 are missing. */
    /* __printf_chk(1, "%3$d\n", 0); */

    return 0;
}
```


#### COLOPHON

​​​       This page is part of the C library user-space interface documentation.
​​​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).