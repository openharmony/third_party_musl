## MALLOC_CHECK_FROM_PTR &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       malloc_check_from_ptr - determine if a memory block was allocated by Standard C library Memory Allocator.

#### **SYNOPSIS**

​       #include <malloc.h>

       int malloc_check_from_ptr(void *ptr);

#### **DESCRIPTION**

​       The malloc_check_from_ptr() function determines whether the memory block
​       pointed to by ptr was allocated using Standard C library Memory Allocator.

​       Note: This function is MT-Safe(multi-thread safe) but not signal-safe.

#### **RETURN VALUE**

​       On success, malloc_check_from_ptr() returns one of the following values:  
​         1  The memory block was allocated using by Standard C library Memory Allocator.  
​         0  The memory block was not allocated using by Standard C library Memory Allocator.  
​         -1 The function is not implemented or an error occurred, errno is set to indicate the error.

#### **ERRORS**

​       The following error codes may be set in errno:  
​         **ENOSYS**: The current standard allocator does not support this interface or it is not implemented.  
​         **EINVAL**: Indicates that an invalid argument was passed to the function.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| malloc_check_from_ptr() | Thread safety | MT-safe  |
|                         | Signal safety | Not Safe |

#### HISTORY

​       -- 2025 

#### NOTES

​      If the function returns -1 and errno is set to ENOSYS, it indicates that the functionality is not implemented on the current platform.  
​      If the function returns -1 and errno is set to a value other than ENOSYS, it indicates that an internal error occurred in the memory allocator.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
 #include <stdio.h>
 #include <errno.h>
 #include <malloc.h>
 
 int main(void) {
     void *p = malloc(128);
     if (!p) {
         perror("malloc");
         return 1;
     }

     int result = malloc_check_from_ptr(p);
     if (result == 1) {
         printf("Pointer was allocated using malloc.\n");
     } else if (result == 0) {
         printf("Pointer was not allocated using malloc.\n");
     } else {
        if (errno == ENOSYS) {
             printf("Function not implemented on this platform.\n");
        } else {
            perror("malloc_check_from_ptr failed");
        }
        return 0;
     }
     free(p);
     return 0;
  }
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitee.com/openharmony/third_party_musl/blob/master/docs/).