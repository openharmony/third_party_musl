## __REALPATH_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __realpath_chk - resolve and canonicalize pathname with buffer overflow protection.

#### **SYNOPSIS**

       #include <stdlib.h>

       char* __realpath_chk(const char* path, char* resolved, size_t resolvedlen);

#### **DESCRIPTION**

​       The __realpath_chk() function is a fortified version of the realpath() function.
#### **PARAMETERS**

​       **path**:        The pathname to be resolved. This can be a relative or absolute path.

​       **resolved**:    A buffer where the canonicalized absolute pathname will be stored.
                       The buffer must be at least PATH_MAX bytes in size.

​       **resolvedlen**: The size of the resolved buffer in bytes. This parameter is used
                       for buffer overflow checking.

#### **RETURN VALUE**

​       On success, __realpath_chk() returns a pointer to the resolved buffer containing
​       the canonicalized absolute pathname. If an error occurs, NULL is returned and
​       errno is set to indicate the error.

#### **ERRORS**

​       The function may set errno to the following values:

​         **EINVAL**:    The resolved buffer is NULL, or if pathconf() returns a valid
​                       path length and resolvedlen is less than that value.

#### **ATTRIBUTES**

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __realpath_chk()        | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

 #### NOTES
 	 
​       This feature is designed specifically for when musl_extended_function is true.

#### **CONFORMING TO**

​       This is a platform-specific extension and is not part of any POSIX standard.

#### **EXAMPLES**

```c
#define __FORTIFY_COMPILATION

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

int main(void) {
    char path[PATH_MAX];
    
    char* result = __realpath_chk("/etc/passwd", path, PATH_MAX);
    if (result) {
        printf("Path: %s\n", path);
    }
    
    return 0;
}
```

#### **COLOPHTON**

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)