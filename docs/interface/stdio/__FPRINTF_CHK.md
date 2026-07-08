## __FPRINTF_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __fprintf_chk - Write output to the given output stream.

#### **SYNOPSIS**

       ​#include <stdio.h>

       int __fprintf_chk(FILE* fp, int flags, const char* format, ...);

#### **DESCRIPTION**

​       Write the output to the specified output stream and perform parameter checking. 

​​       It expects as input a writable FILE pointer, an int-type flags is used to specify the security check level, a "format string" that specifies what to print, and zero or more subsequent arguments. The format string can optionally contain "conversion specifications",placeholders that begin with % that specify how to format the function’s subsequent arguments, if any. For instance, if file is a pointer to a FILE and c is a char, ​​this function can print the latter to the former as follows using %c:

       __fprintf_chk(file, 0, "%c\n", c);

​       If flags is set to 0 or a negative value, verification is not enabled; if flags is greater than 0, verification is enabled. When enabled, format string validation is performed: if positional parameter references (e.g., %n$d) are non-contiguous, the program will abort with a "Musl Fortify runtime error: invalid specified parameter" message.

#### **RETURN VALUE**

​​       Upon successful return, this function return the number of characters printed (excluding the null byte used to end output to strings).

​​       If an output error is encountered, a negative value is returned.

#### **ERRORS**

​       The following error codes may be set in errno:  

​       **EINVAL**: format is NULL. The function sets errno to EINVAL and returns -1.

​       **EBADF**: fp is not writable.

​       **EOVERFLOW**: formatted output length or width calculation overflows.

​       If fp is NULL, the function aborts with the message "Musl Fortify runtime error: the file point is NULL".

​       When flags is greater than 0 (fortify mode), if the format string contains positional parameter references that are non-contiguous (e.g., "%3$d" skips positions 1 and 2), the program will abort with the message "Musl Fortify runtime error: invalid specified parameter".

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __fprintf_chk() | Thread safety | MT-Safe |
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

int main(void)
{
    const char *s = "This is test";
    int ret = 0;
    FILE *file = fopen("test.txt", "w");
    if (file == NULL) {
        return -1;
    }

    ret = __fprintf_chk(file, 0, "%s\n", s);
    if (ret < 0) {
        perror("__fprintf_chk failed.");
        fclose(file);
        return -1;
    }

    ret = __fprintf_chk(file, 1, "%s\n", s);
    if (ret < 0) {
        perror("__fprintf_chk failed.");
        fclose(file);
        return -1;
    }

    /* WARNING: the following call causes abort when flags>0.
       "%3$d" references position 3 while positions 1 and 2 are missing. */
    /* __fprintf_chk(file, 1, "%3$d\n", 0); */

    fclose(file);
    return 0;
}
```


#### COLOPHON

​​       This page is part of the C library user-space interface documentation.
​​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).