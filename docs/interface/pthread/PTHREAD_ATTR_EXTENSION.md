### **PTHREAD_ATTR_EXTENSION        OHOS Programmer's Manual**


#### **NAME**

​       pthread_attr_extension_init - initialize thread extension properties.

​       pthread_attr_extension_destroy - destroy thread extension properties.


#### **SYNOPSIS**

       #define _GNU_SOURCE
       #include <pthread.h>

       int pthread_attr_extension_init(pthread_attr_t *attr);

       int pthread_attr_extension_destroy(pthread_attr_t *attr);


#### **DESCRIPTION**

​       The  pthread_attr_extension_init() function initializes thread extension attributes object.

​       The pthread_attr_extension_destroy() function destroys the memory requested by the extension.

#### **RETURN VALUE**

​       On success, these functions return 0; on error, they return a nonzero error number.

#### **ERRORS**
The following error codes may be set in errno:

​       **EINVAL**:  parameter exception, **musl_pc_module** is zero or the  `__LITEOS_A__` macro is defined.

​       **ENOMEM**: (pthread_attr_extension_init()) Could not allocate memory.

#### ATTRIBUTES

| Interface                                                    | Attribute     | Value    |
| ------------------------------------------------------------ | ------------- | -------- |
| pthread_attr_extension_init()                                | Thread safety | MT-Unsafe|
|                                                              | Signal safety | Not Safe |
| pthread_attr_extension_destroy()                             | Thread safety | MT-Unsafe|
|                                                              | Signal safety | Not Safe |


#### HISTORY

​       -- 2026

#### NOTES

​       These functions are ‌exclusively designed for PC environments‌.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>

#ifdef MUSL_EXTERNAL_FUNCTION
int main() {
#ifndef __LITEOS_A__
    set_pthread_extended_function_policy(1);
    if (!get_pthread_extended_function_policy()) {
        printf("musl_pc_module is zero.\n");
        return -1;
    }

    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret != 0) {
        printf("pthread_attr_init failed: %d\n", ret);
        return -1;
    }

    ret = pthread_attr_extension_init(&attr);
    if (ret != 0) {
        printf("pthread_attr_extension_init failed: %d\n", ret);
        pthread_attr_destroy(&attr);
        return -1;
    }

    ret = pthread_attr_extension_destroy(&attr);
    if (ret != 0) {
        printf("pthread_attr_extension_destroy failed: %d\n", ret);
    }

    ret = pthread_attr_destroy(&attr);
    if (ret != 0) {
        printf("pthread_attr_destroy failed: %d\n", ret);
        return -1;
    }
    return 0;
#else
    printf("__LITEOS_A__ is defined.\n");
    return -1;
#endif
}
#endif
```


#### COLOPHON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)
