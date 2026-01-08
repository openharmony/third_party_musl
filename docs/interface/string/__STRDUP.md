## __STRDUP &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __strdup - duplicate a string.

#### **SYNOPSIS**
       
       #include <string.h>

       char *__strdup (const char *s);

#### **DESCRIPTION**

​       The __strdup() function is similar to strdup, returns a pointer to a new string which is a duplicate of the string s.  Memory for the new string is obtained with malloc, and can be freed with free.

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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
        ret = NULL;
    } else {
        printf("__strdup verify fail, Memory allocation fail\n");
    }
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)