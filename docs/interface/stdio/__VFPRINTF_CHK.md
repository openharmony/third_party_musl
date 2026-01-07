## __VFPRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __vfprintf_chk - Write output to the given output stream.

#### **SYNOPSIS**

       #include <stdio.h>

       int __vfprintf_chk(FILE* fp, int flags, const char* format, va_list va);

#### **DESCRIPTION**

​       Write the output to the specified output stream and perform parameter checking. 

​       It expects as input the pointer to a FILE that was returned by fopen, an int-type flags is used to specify the security check level, a "format string" ​       that specifies what to print, and va_list type variable parameter list. The format string can optionally contain "conversion specifications",
​       placeholders that begin with % that specify how to format the function’s subsequent arguments, if any.

​       If flags is set to 0, verification is not enabled; if greater than 0, verification is enabled.  

#### **RETURN VALUE**

​       Upon successful return, this function return the number of bytes printed (excluding the null byte used to end output to strings).

​       If an output error is encountered, a negative value is returned.

#### **ERRORS**

​       The following error codes may be set in errno: 

​       EINVAL: point is invalid.

​       EBADF: Bad file descriptor.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __vfprintf_chk() | Thread safety | MT-Safe |
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
#include <stdarg.h>

static int VfprintfHelper(FILE* fp, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __vfprintf_chk(fp, 0, format, args);
    va_end(args);
    return result;
}

int main(void)
{
    char *s = "This is test";
    int i = 50;
    float f = 50.0f;
    FILE *file = fopen("test.txt", "w");
    if (file == NULL) {
        return 0;
    }

    VfprintfHelper(file, "%s\n", s);
    VfprintfHelper(file, "This is test %d\n", i);
    VfprintfHelper(file, "This is test %.0f\n", f);

    fclose(file);
    return 0;
}
```


#### COLOPHTON

​​       this page is part of the C library user-space interface documentation.
​​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)