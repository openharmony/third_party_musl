## DLOPEN_CONFIG_ABS_PATH_POLICY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       dlopen_config_abs_path_policy - Set check abs path flag

#### **SYNOPSIS**

       void dlopen_config_abs_path_policy(bool flag);

#### **DESCRIPTION**

During the `dlopen` process, when traversing the namespace inheritance hierarchy, if the opened .so file is an absolute path, it will directly traverse to the next namespace without performing permission checks on the .so file name. Setting this flag to false will ensure that permission checks on the .so file name are performed when opening an .so file with an absolute path.

Note: This function is MT-Safe(multi-thread safe) and not signal-safe.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| int dlns_add_plugin_default_ld_dictionary(char *path) | Thread safety | MT-safe |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2025 

#### NOTES

​      `false` flag is an unique valid param now. `true` flag is invalid now.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
typedef void(Func)(bool flag);

int main()
{
    Func *func = dlsym(RTLD_NEXT, "dlopen_config_abs_path_policy");
    if (!func) {
        perror("can not find this symbol");
    }
    func(false);
    // You can execute other operation here

    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).