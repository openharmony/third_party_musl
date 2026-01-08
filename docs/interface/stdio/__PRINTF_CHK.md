## __PRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __printf_chk - Write output to stdout.

#### **SYNOPSIS**


       #include <stdio.h>

       int __printf_chk(int flags, const char* format, ...);

#### **DESCRIPTION**

​       Write output to stdout and perform parameter checking. 

​       It expects as input an int-type flags is used to specify the security check level, a "format string" that specifies what to print, and zero or more ​       subsequent arguments. The format string can optionally contain "conversion specifications", placeholders that begin with % that specify how to format ​       the function’s subsequent arguments, if any.

​       If flags is set to 0, verification is not enabled; if greater than 0, verification is enabled.  

#### **RETURN VALUE**

​       Upon successful return, this function return the number of bytes printed (excluding the null byte used to end output to strings).

​       If an output error is encountered, a negative value is returned.

#### **ERRORS**

​       The following error codes may be set in errno:

​       EINVAL: point is invalid.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __printf_chk() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>

int main(void)
{
    char *s = "This is test";
    int i = 50;
    float f = 50.0f;

    __printf_chk(0, "%s\n", s);
    __printf_chk(0, "This is test %d\n", i);
    __printf_chk(0, "This is test %.0f\n", f);

    return 0;
}
```


#### COLOPHTON

​​​       this page is part of the C library user-space interface documentation.
​​​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)