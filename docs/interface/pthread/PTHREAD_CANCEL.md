## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       pthread_cancel - send a cancelation request to a thread

#### **SYNOPSIS**

       int pthread_cancel(pthread_t thread);

#### **DESCRIPTION**
        
This interface is differentiated at compile time, only enabled on specific devices, and requires explicit enabling to fully utilize the functionality. For further information, please refer to the documentation:
        
Refer to [pthread_cancel](https://man7.org/linux/man-pages/man3/pthread_cancel.3.html).
        
#### **ERRORS**

The following error codes may be returned:

**ENOSYS**: No features are currently enabled.

other error codes refer to [pthread_cancel](https://man7.org/linux/man-pages/man3/pthread_cancel.3.html).

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

typedef int (*PTHREAD_CANCEL_FUNC_TYPE)(pthread_t thread);

void* worker_thread(void* arg) {
    int i = 0;
    printf("Worker thread started\n");
    
    while (1) {
        printf("Working... %d\n", ++i);
        sleep(1); // sleep是取消点
    }
    
    printf("Worker thread finished normally\n"); // 不会执行到这里
    return NULL;
}

int main() {
    PTHREAD_CANCEL_FUNC_TYPE pthread_cancel_func = (PTHREAD_CANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_cancel");
    if (!pthread_cancel_func) {
        return -1;
    }
    pthread_t thread;
    
    pthread_create(&thread, NULL, worker_thread, NULL);
    sleep(3); // 让线程运行3秒
    
    printf("Main thread calling pthread_cancel\n");
    pthread_cancel_func(thread); // 发送取消请求
    
    pthread_join(thread, NULL); // 等待线程结束
    printf("Main thread exiting\n");
    
    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).