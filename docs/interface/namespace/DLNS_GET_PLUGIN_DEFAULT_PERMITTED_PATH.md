## DLNS_GET_PLUGIN_DEFAULT_PERMITTED_PATH &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       dlns_get_plugin_default_permitted_path - Get **moduleNs_plugin_default_namespace** permitted_path

#### **SYNOPSIS**

​       #include <dlfcn.h>

       char *dlns_get_plugin_default_permitted_path();

#### **DESCRIPTION**

Get **moduleNs_plugin_default_namespace** permitted_path

Note: This function is MT-Safe(multi-thread safe) but not signal-safe. This function returns a copy of **moduleNs_plugin_default_namespace** permitted_path. Therefore, when the caller does not use the content of this string, they should release it using [free](https://pubs.opengroup.org/onlinepubs/9799919799/functions/free.html). This function is mutually exclusive with other namespaces and linker **write logic** operations

#### **RETURN VALUE**

On success, dlns_get_plugin_default_permitted_path() return a string pointer. Otherwise, NULL will be returned, errno is set to indicate the error.

#### **ERRORS**

The following error codes may be set in errno:  
1. **ENOSYS**: There is currently no **moduleNs_plugin_default_namespace**.
2. **ENOENT**: The permitted_path is NULL.
3. **ENOMEM**: Storage space available is insufficient.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| char *dlns_get_plugin_default_permitted_path() | Thread safety | MT-safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2025 

#### NOTES

​      None.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

typedef char*(func_t)(void);

int main()
{
    func_t *func = (func_t *)dlsym(RTLD_DEFAULT, "dlns_get_plugin_default_permitted_path");
    if (!func) {
        return -1;
    }
    char *str = func();
    if (!str) {
        printf("get moduleNs_plugin_default_namespace permitted path failed, errno=%d\n", errno);
    } else {
        printf("get moduleNs_plugin_default_namespace succeed permitted path=%s\n", str);
        free((void *)str);
    }
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)