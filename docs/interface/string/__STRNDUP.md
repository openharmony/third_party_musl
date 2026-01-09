## __STRNDUP &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       __strndup - search for a character in the specified memory block.

#### **SYNOPSIS**

       #include <string.h>

       char *__strndup(const char *s, size_t n);


#### **DESCRIPTION**

​       The __strndup() function is similar to strdup, but copies at most n bytes.  If s is longer than n, only n bytes are copied, and a terminating null byte ('\0') is added.

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

​       -- 2025

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      The __strndup() conforms to POSIX.1-2008.

#### EXAMPLES

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char *str = __strndup("hello world", 5); // Copy the first 5 characters
    printf("Copied string: %s\n", str);
    free(str);
    return 0;
}
```

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)