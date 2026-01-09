## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       pthread_testcancel - request delivery of any pending cancelation request

#### **SYNOPSIS**

       void pthread_testcancel(void);

#### **DESCRIPTION**
        
This interface is differentiated at compile time, only enabled on specific devices, and requires explicit enabling to fully utilize the functionality. For further information, please refer to the documentation:
        
Refer to [pthread_testcancel](https://man7.org/linux/man-pages/man3/pthread_testcancel.3.html).

#### **REFER TO**

​       [pthread_testcancel](https://man7.org/linux/man-pages/man3/pthread_testcancel.3.html).

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

typedef void (*PTHREAD_TESTCANCEL_FUNC_TYPE)(void);
typedef int (*PTHREAD_CANCEL_FUNC_TYPE)(pthread_t thread);

PTHREAD_TESTCANCEL_FUNC_TYPE pthread_testcancel_func;
PTHREAD_CANCEL_FUNC_TYPE pthread_cancel_func;

void* worker_thread(void* arg) {
    printf("Worker thread started\n");
    
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        sleep(1);
        
        // 手动设置取消点
        pthread_testcancel_func();
    }
    
    printf("Worker thread finished normally\n");
    return NULL;
}

int main() {
    pthread_testcancel_func = (PTHREAD_TESTCANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_testcancel");
    if (!pthread_testcancel_func) {
        return -1;
    }
    pthread_cancel_func = (PTHREAD_CANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_cancel");
    if (!pthread_cancel_func) {
        return -1;
    }

    pthread_t thread;
    
    pthread_create(&thread, NULL, worker_thread, NULL);
    sleep(3); // 让线程运行3秒
    
    printf("Main thread calling pthread_cancel\n");
    pthread_cancel_func(thread);
    
    void* result;
    pthread_join(thread, &result);
    
    // 检查线程是否被取消
    if (result == PTHREAD_CANCELED) {
        printf("Thread was canceled\n");
    }
    
    printf("Main thread exiting\n");
    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).