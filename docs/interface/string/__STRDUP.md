## __STRDUP &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __strdup - duplicate a string.

#### **SYNOPSIS**
       
       #include <string.h>

       char *__strdup(const char *s);

#### **DESCRIPTION**

​       The __strdup() function returns a pointer to a new string which is a duplicate of the string s. The argument s must point to a null-terminated string. Memory for the new string is obtained with malloc, and can be freed with free.

​       The duplicated string includes the terminating null byte. The caller owns the returned buffer and must release it with free().

#### **RETURN VALUE**

​       On success, the __strdup() function returns a pointer to the duplicated string. It returns NULL if insufficient memory was available.

#### **ERRORS**

​       **ENOMEM**: Insufficient memory available to allocate duplicate string.

​       Passing NULL as the argument s results in undefined behavior. The caller must ensure s is a valid pointer to a null-terminated string.

#### ATTRIBUTES

| Interface  | Attribute     | Value    |
| -----------| ------------- | -------- |
| __strdup() | Thread safety | MT-safe  |
|            | Signal safety | Not Safe |

#### HISTORY

​       -- 2026 

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard. 

#### EXAMPLES

```c
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(void) {
    const char *str = "strduptest";
    char *ret = __strdup(str);

    if (ret != NULL) {
        if (strcmp(ret, str) == 0) {
            printf("__strdup verify success, result = %s, expect = %s\n", ret, str);
        } else {
            printf("__strdup verify fail, result = %s, expect = %s\n", ret, str);
        }
        free(ret);
    } else {
        printf("__strdup failed, errno = %d (%s)\n", errno, strerror(errno));
    }

    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).