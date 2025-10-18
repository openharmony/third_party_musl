## DLNS_SET_LD_PERMITTED_PATH &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       dlns_set_ld_permitted_path - Set ld permitted_path

#### **SYNOPSIS**

​       #include <dlfcn.h>

       int dlns_set_ld_permitted_path(char *path, Dl_namespace *ns);

#### **DESCRIPTION**

Set the directory permission path in the specified namespace. 

For parameter `path`: If there are multiple paths, you need to use `:` to separate them. It can not start with a `:`or end with a `:`. Every path can not end with a `/`. For example: `/a/b` is ok but `/a/b/` is not a correct param. The maximum length of each path cannot exceed [PATH_MAX](https://pubs.opengroup.org/onlinepubs/9799919799/functions/fpathconf.html). The total length is limited.

When the parameter `ns` points to **moduleNs_plugin_default_namespace**, this function can restrict the behavior of `dlns_add_plugin_default_ld_dictionary`.

Note: This function is MT-Safe(multi-thread safe) and not signal-safe. This function is used in permission management. This function is mutually exclusive with other namespaces and linker operations

#### **RETURN VALUE**

On success, dlns_set_ld_permitted_path() return 0. Otherwise, -1 will be returned, errno is set to indicate the error.

#### **ERRORS**

The following error codes may be set in errno:  
1. **EACCES**:  caller so is not permitted.  
2. **EINVAL**:  path is invalid.
3. **ENOMEM**:  Storage space available is insufficient.
4. **ENOENT**:  The specified namespace was not found.
#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| int dlns_set_ld_permitted_path(char *path, Dl_namespace *ns) | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2025 

#### NOTES

​      This function cannot currently be used with `dlns_add_plugin_default_ld_dictionary` in multi-threaded mode at the same time when parameter `namespace` is indicated to **moduleNs_plugin_default_namespace**; otherwise, it may cause unpredictable behavior in the `dlns_add_plugin_default_ld_dictionary`  function.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
// This testcase must be compile and execute in system-side
#include <dlfcn.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>

char PLUGIN_DEFAULT_NS[] = "moduleNs_plugin_default_namespace\0";
char libpath_name[] = "/test/path\0";

bool test()
{
    Dl_namespace plugin_ns;
    dlns_init(&plugin_ns, PLUGIN_DEFAULT_NS);
    int result = dlns_create2(&plugin_ns, NULL, LOCAL_NS_PREFERED);
    if (result) {
        printf("dlns_create2 failed result=%d\n", result);
        return false;
    }
    Dl_namespace ns;
    result = dlns_get("moduleNs_default", &ns);
    if (result) {
        printf("dlns_get moduleNs failed result=%d\n", result);
        return false;
    }
    result = dlns_inherit(&ns, &plugin_ns, ALLOW_ALL_SHARED_LIBS);
    if (result) {
        printf("dlns_inherit failed result=%d\n", result);
        return false;
    }
    result = dlns_set_ld_permitted_path(libpath_name, &plugin_ns);
    if (result) {
        printf("dlns_set_ld_permitted_path failed result=%d errno=%d\n", result, errno);
        return false;
    }
    return true;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)