## __FPRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __fprintf_chk - Write output to the given output stream.

#### **SYNOPSIS**

       ​#include <stdio.h>

       int __fprintf_chk(FILE* fp, int flags, const char* format, ...);

#### **DESCRIPTION**

​       Write the output to the specified output stream and perform parameter checking. 

​​       It expects as input the pointer to a FILE that was returned by fopen, an int-type flags is used to specify the security check level, a "format string" ​​       that specifies what to print, and zero or more subsequent arguments. The format string can optionally contain "conversion specifications",placeholders ​​       that begin with % that specify how to format the function’s subsequent arguments, if any. For instance, if file is a pointer to a FILE and c is a ​       char, ​​this function can print the latter to the former as follows using %c:

       __fprintf_chk(file, 0, "%c\n", c);

#### **RETURN VALUE**

​​       Upon successful return, this function return the number of bytes printed (excluding the null byte used to end output to strings).

​​       If an output error is encountered, a negative value is returned.

#### **ERRORS**

​       The following error codes may be set in errno:  

​       EINVAL: point is invalid.

​       EBADF: Bad file descriptor.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __fprintf_chk() | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>

int main(void)
{
    char *s = "This is test";
    int i = 50;
    float f = 50.0f;
    FILE *file = fopen("test.txt", "w");
    if (file == NULL) {
        return 0;
    }

    __fprintf_chk(file, 0, "%s\n", s);
    __fprintf_chk(file, 0, "This is test %d\n", i);
    __fprintf_chk(file, 0, "This is test %.0f\n", f);

    fclose(file);
    return 0;
}
```


#### COLOPHTON

​​       this page is part of the C library user-space interface documentation.
​​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)