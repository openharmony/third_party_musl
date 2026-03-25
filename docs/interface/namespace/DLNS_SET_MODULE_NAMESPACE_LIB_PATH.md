## DLNS_SET_MODULE_NAMESPACE_LIB_PATH &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       dlns_set_module_namespace_lib_path - Set lib_path for non-config namespace

#### **SYNOPSIS**

```c
​       #include <dlfcn.h>
       int dlns_set_module_namespace_lib_path(const char *name, const char *lib_path);
```

#### **DESCRIPTION**

Set the library search path for a non-config namespace.

This function is only used for namespaces that are not in the built-in permitted namespace list. Namespaces in the built-in permitted namespace list are not allowed to be modified through this interface.

For parameter `name`: The target namespace must already exist. This function does not create a namespace.

For parameter `lib_path`: If there are multiple paths, you need to use `:` to separate them. It can not start with a `:`or end with a `:`. Every path can not end with a `/`. For example: `/a/b` is ok but `/a/b/` is not a correct param. The maximum length of each path cannot exceed [PATH_MAX](https://pubs.opengroup.org/onlinepubs/9799919799/functions/fpathconf.html). The total length is limited.

The caller so must have permission to modify namespace configuration. Otherwise, the operation will fail.

Note: This function is MT-Safe(multi-thread safe) and not signal-safe. This function is mutually exclusive with other namespaces and linker write operations.

#### **RETURN VALUE**

On success, dlns_set_module_namespace_lib_path() return 0. Otherwise, an error code will be returned directly.

#### **ERRORS**

The following error codes may be returned:  
1. **EINVAL**: `name` or `lib_path` is NULL or other invalid input.
2. **EACCES**: `name` is in the built-in permitted namespace list, and this interface cannot modify it.
3. **EPERM**: caller so is not permitted to perform this operation.
4. **ENOKEY**: The specified namespace was not found.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| int dlns_set_module_namespace_lib_path(const char *name, const char *lib_path) | Thread safety | MT-Safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2026 

#### NOTES

​      This interface only modifies `lib_path` for an existing non-config namespace. It does not create or inherit namespaces.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
// This testcase must be compile and execute in system-side
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>

int main(void)
{
    Dl_namespace ns;
    dlns_init(&ns, "example_nonconfig_ns");

    int ret = dlns_create(&ns, NULL);
    if (ret != 0) {
        printf("dlns_create failed ret=%d\n", ret);
        return ret;
    }

    ret = dlns_set_module_namespace_lib_path("example_nonconfig_ns", "/data/tests/libc-test/src");
    if (ret != 0) {
        printf("dlns_set_module_namespace_lib_path failed ret=%d\n", ret);
        return ret;
    }

    void *handle = dlopen_ns(&ns, "set_lib_path_0200.so", RTLD_LAZY);
    if (!handle) {
        printf("dlopen_ns failed\n");
        return -1;
    }

    dlclose(handle);
    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
