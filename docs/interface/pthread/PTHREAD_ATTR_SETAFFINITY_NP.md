### **PTHREAD_ATTR_SETAFFINITY_NP        OHOS Programmer's Manual**


#### **NAME**

​       pthread_attr_setaffinity_np, pthread_attr_getaffinity_np - set/get CPU affinity attribute in thread attributes object.

#### **SYNOPSIS**


       #define _GNU_SOURCE
       #include <pthread.h>

       int pthread_attr_setaffinity_np(pthread_attr_t *attr,
                                       size_t cpusetsize,
                                       const cpu_set_t *cpuset);

       int pthread_attr_getaffinity_np(pthread_attr_t *attr,
                                       size_t cpusetsize,
                                       cpu_set_t *cpuset);


#### **DESCRIPTION**

​       The  pthread_attr_setaffinity_np() function sets the CPU affinity mask attribute of the thread attributes object referred to by attr to the value specified in cpuset. This attribute determines the CPU affinity mask of a thread created using the thread attributes object attr.
​       The pthread_attr_getaffinity_np() function returns the CPU affinity mask attribute of the thread attributes object referred to by attr in the buffer pointed to by cpuset.

​       The argument cpusetsize is the length (in bytes) of the buffer pointed to by cpuset. Typically, this argument would be specified as sizeof(cpu_set_t).

#### **RETURN VALUE**

​       On success, these functions return 0; on error, they return a nonzero error number.

#### **ERRORS**
The following error codes may be set in errno:

​       **EINVAL**: (pthread_attr_setaffinity_np()) cpuset specified a CPU that was outside the set supported by the kernel or parameter exception.

​       **EINVAL**: (pthread_attr_getaffinity_np())  A CPU in the affinity mask of the thread attributes object referred to by attr lies outside the range specified by cpusetsize (i.e., cpuset/cpusetsize is too small)  or parameter exception.

​       **ENOMEM**: (pthread_attr_setaffinity_np()) Could not allocate memory.

#### ATTRIBUTES

| Interface                                                    | Attribute     | Value    |
| ------------------------------------------------------------ | ------------- | -------- |
| pthread_attr_setaffinity_np(), <br> pthread_attr_getaffinity_np() | Thread safety | MT-Safe  |
|                                                              | Signal safety | Not Safe |


#### HISTORY

​       -- 2026

#### NOTES

​      These feature are designed specifically for when musl_extended_function is true.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#ifdef MUSL_EXTERNAL_FUNCTION
#ifndef __LITEOS_A__
void* thread_func(void* arg) {
    printf("The thread is running...\n");
    return NULL;
}

int main() {
    if (!get_pthread_extended_function_policy()) {
        return -1;
    }
    pthread_t thread;
    pthread_attr_t attr;
    cpu_set_t cpuset;

    pthread_attr_init(&attr);

    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);

    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    cpu_set_t check_cpuset;
    pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &check_cpuset);

    if (CPU_ISSET(2, &check_cpuset)) {
        printf("Successfully bound the thread to CPU 2\n");
    }

    pthread_create(&thread, &attr, thread_func, NULL);
    pthread_join(thread, NULL);
    pthread_attr_destroy(&attr);

    return 0;
}
#endif
#endif
```


#### COLOPHON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)
