## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       dlns_add_plugin_default_ld_dictionary - Add ld dictionary

#### **SYNOPSIS**

       int dlns_add_plugin_default_ld_dictionary(char *path);

#### **DESCRIPTION**

Add single lib_path for **moduleNs_plugin_default_namespace**.

For parameter `path`: 
1. It can not end with a `/`. For example: `/a/b` is ok but `/a/b/` is not a correct param.
2. It can not contains `:`.
3. Special substring(like `..`, `~`, `//`) is not permitted.
4. It is also restricted by [dlns_set_ld_permitted_path](./DLNS_SET_LD_PERMITTED_PATH.md).
5. `path`'s length is restricted by [PATH_MAX](https://pubs.opengroup.org/onlinepubs/9799919799/functions/fpathconf.html).

Note: This function is MT-Safe(multi-thread safe) and not signal-safe. This function is mutually exclusive with other namespaces and linker operations.

#### **RETURN VALUE**

​       On success, dlns_add_plugin_default_ld_dictionary() return 0. Otherwise, -1 will be returned, errno is set to indicate the error.

#### **ERRORS**

The following error codes may be set in errno:  
1. **EACCES**: `path` is not permitted.
2. **EINVAL**: Input `path` is NULL.
3. **EAGAIN**: The system side has not yet set the permitted path in **moduleNs_plugin_default_namespace**.
4. **ENOSYS**: There is currently no **moduleNs_plugin_default_namespace**.
5. **ENOMEM**: There is currently not enough memory.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| int dlns_add_plugin_default_ld_dictionary(char *path) | Thread safety | MT-safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2025 

#### NOTES

​      Refers to [dlns_set_ld_permitted_path](./DLNS_SET_LD_PERMITTED_PATH.md).

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
typedef int (test_func)(char *path);

int test()
{
    test_func *dlns_add_plugin_default_ld_dictionary = (test_func *)dlsym(RTLD_DEFAULT, "dlns_add_plugin_default_ld_dictionary");
    if (!dlns_add_plugin_default_ld_dictionary) {
        return -1;
    }
    int result = dlns_add_plugin_default_ld_dictionary("/test/path/p1");
    if (result == -1) {
        printf("result=%d errno=%d\n", result, errno);
    }
    return result;
}

int main()
{
    printf("result=%d\n", test());
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).