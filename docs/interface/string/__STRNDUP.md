## __STRNDUP &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __strndup - copy the first n characters of the string and add a terminator.

#### **SYNOPSIS**

       #include <string.h>

       char *__strndup(const char *s, size_t n);


#### **DESCRIPTION**

​       The __strndup() function is similar to strdup, but copies at most n bytes.  If s is longer than n, only n bytes are copied, and a terminating null byte ('\0') is added. If the input parameter s is NULL, the indirectly called interface will report a segment error.

#### **RETURN VALUE**

​       The __strndup() function returns a pointer to the duplicated string, or NULL if insufficient memory was available.

#### **ERRORS**
​
The following error codes may be set in errno:
**ENOMEM**: Insufficient memory available to allocate duplicate string.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __strndup()             | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The __strndup() conforms to POSIX.1-2008.

#### EXAMPLES

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    const char *original = "hello world";
    size_t copy_length = 5;
    char *str = __strndup(original, copy_length);
    if (!str) {
        printf("Failed to execute __strndup(): %s\n", strerror(errno));
        return 1;
    }

    free(str);
    return 0;
}
```

#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).