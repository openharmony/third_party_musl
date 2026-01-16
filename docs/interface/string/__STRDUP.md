## __STRDUP &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __strdup - duplicate a string.

#### **SYNOPSIS**
       
       #include <string.h>

       char *__strdup(const char *s);

#### **DESCRIPTION**

​       The __strdup() function is similar to strdup, returns a pointer to a new string which is a duplicate of the string s.  Memory for the new string is obtained with malloc, and can be freed with free. If the argument s is NULL, __strdup() shall return NULL and set errno to EINVAL to indicate an invalid argument error.

#### **RETURN VALUE**

​       On success, the strdup() function returns a pointer to the duplicated string.  It returns NULL if insufficient memory was available, with errno set to indicate the cause of the error.

#### **ERRORS**

​       **ENOMEM**: Insufficient memory available to allocate duplicate string.

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
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

char *safe_strdup(const char *s) {
    if (s == NULL) {
        errno = EINVAL;
        return NULL;
    }
    return __strdup(s);
}

int main(void) {
    const char *str = "strduptest";
    char *ret = safe_strdup(str);
    if (ret != NULL) {
        if (strcmp(ret, str) == 0) {
            printf("Test 1 (normal string): __strdup verify success, result = %s, expect = %s\n", ret, str);
        } else {
            printf("Test 1 (normal string): __strdup verify fail, result = %s, expect = %s\n", ret, str);
        }
        free(ret);
        ret = NULL;
    } else {
        printf("Test 1 (normal string): __strdup verify fail, errno = %d (%s)\n", errno, strerror(errno));
    }

    errno = 0;
    ret = safe_strdup(NULL);
    if (ret == NULL) {
        if (errno == EINVAL) {
            printf("Test 2 (s = NULL): __strdup behavior is correct - return NULL, errno = EINVAL (%d)\n", errno, strerror(errno));
        } else {
            printf("Test 2 (s = NULL): __strdup return NULL but errno is wrong - errno = %d (%s)\n", errno, strerror(errno));
        }
    } else {
        printf("Test 2 (s = NULL): __strdup behavior is wrong - return non-NULL pointer: %p\n", ret);
        free(ret);
    }

    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).