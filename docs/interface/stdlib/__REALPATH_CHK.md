## __REALPATH_CHK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual


#### **NAME**

​       __realpath_chk - resolve and canonicalize pathname with buffer overflow protection.

#### **SYNOPSIS**

       #include <stdlib.h>

       char* __realpath_chk(const char* path, char* resolved, size_t resolvedlen);

#### **DESCRIPTION**

​       The __realpath_chk() function is a fortified version of the realpath() function. Before calling realpath(), it checks resolvedlen against pathconf(path, _PC_PATH_MAX). If resolvedlen is smaller than that value, the function aborts with a fortify runtime error.

#### **PARAMETERS**

​       **path**:        The pathname to be resolved. This can be a relative or absolute path.

​       **resolved**:    A buffer where the canonicalized absolute pathname will be stored. If resolved is NULL, realpath() allocates the returned buffer with malloc().

​       **resolvedlen**: The caller-supplied size used for buffer overflow checking. It must be no smaller than pathconf(path, _PC_PATH_MAX), even when resolved is NULL.

#### **RETURN VALUE**

​       On success, __realpath_chk() returns a pointer to the canonicalized absolute pathname. If resolved is not NULL, the return value is resolved. If resolved is NULL, the return value is a newly allocated buffer that must be released with free(). If realpath() fails, NULL is returned and errno is set to indicate the error.

#### **ERRORS**

​       The function may set errno to the values reported by realpath().

​       **EINVAL**: path is NULL.

​       If resolvedlen is smaller than pathconf(path, _PC_PATH_MAX), the function aborts with the message "Musl Fortify runtime error: realpath buffer too small".

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
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

int main(void) {
    /* Example 1: Sufficient buffer size. */
    char path[PATH_MAX];
    char* result = __realpath_chk(".", path, PATH_MAX);
    if (result) {
        printf("Path: %s\n", path);
    } else {
        printf("__realpath_chk failed");
    }
    
    /* Example 2: resolved can be NULL when resolvedlen is large enough. */
    result = __realpath_chk(".", NULL, PATH_MAX);
    if (result) {
        printf("Allocated path: %s\n", result);
        free(result);
    }

    /* WARNING: the following call aborts with
       "Musl Fortify runtime error: realpath buffer too small". */
    /*
    char small_buffer[10];
    result = __realpath_chk(".", small_buffer, sizeof(small_buffer));
    */
    
    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).