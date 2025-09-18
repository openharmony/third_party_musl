## MALLOC_CHECK_FROM_PTR &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       restrace - resource usage trace stack.

#### **SYNOPSIS**

​       #include <memory_trace.h>

       void restrace(unsigned long long mask, void* addr, size_t size, const char* tag, bool is_using);

#### **DESCRIPTION**

​       This function inject a hook point from which stack can be unwind
       when using Hiprofiler (ref: https://gitcode.com/openharmony/docs/blob/master/zh-cn/application-dev/dfx/hiprofiler.md).

​       Note: This function is MT-Safe(multi-thread safe) but not signal-safe.

#### ATTRIBUTES

       | Attribute     | Value    |
       | ------------- | -------- |
       | Thread safety | MT-safe  |
       | Signal safety | Not Safe |

#### **ERRORS**

​       The following error codes may be set in errno:  
​         **ENOSYS**: The current program does not support this interface.  


#### HISTORY

​       -- 2025 

#### NOTES

​      If errno is set to ENOSYS, it indicates that nativehook is not enabled.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)