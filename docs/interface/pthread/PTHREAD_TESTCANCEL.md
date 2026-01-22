## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



### NAME

​       pthread_testcancel - request delivery of any pending cancelation request.

### SYNOPSIS

       void pthread_testcancel(void);

### DESCRIPTION
        
The pthread_testcancel() function shall create a cancellation point in the calling thread. The pthread_testcancel() function shall have no effect if cancelability is disabled.

### ERRORS

This function shall not return an error code of **EINTR**.

### REFERS

Refer to [pthread_testcancel](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_testcancel.html).

### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

typedef void (*PTHREAD_TESTCANCEL_FUNC_TYPE)(void);
typedef int (*PTHREAD_CANCEL_FUNC_TYPE)(pthread_t thread);
typedef int (*PTHREAD_EXTENDED_FUNC_POLICY)(int policy);

PTHREAD_TESTCANCEL_FUNC_TYPE pthread_testcancel_func;
PTHREAD_CANCEL_FUNC_TYPE pthread_cancel_func;

void* worker_thread(void* arg) {
    printf("Worker thread started\n");
    
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        sleep(1);
        
        // set cancel-point manually
        pthread_testcancel_func();
    }
    
    printf("Worker thread finished normally\n");
    return NULL;
}

int main() {
    // get pthread_testcancel symbol.
    pthread_testcancel_func = (PTHREAD_TESTCANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_testcancel");
    if (!pthread_testcancel_func) {
        // find symbol failed.
        return -1;
    }
    // get pthread_cancel symbol.
    pthread_cancel_func = (PTHREAD_CANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_cancel");
    if (!pthread_cancel_func) {
        // find symbol failed.
        return -1;
    }
    // get set_pthread_extended_function_policy symbol.
    PTHREAD_EXTENDED_FUNC_POLICY set_policy
        = (PTHREAD_EXTENDED_FUNC_POLICY)dlsym(RTLD_DEFAULT, "set_pthread_extended_function_policy");
    if (!set_policy) {
        // find symbol failed.
        return -1;
    }
    // enable extended pthread function.
    set_policy(1);

    pthread_t thread;
    
    pthread_create(&thread, NULL, worker_thread, NULL);
    sleep(3); // let main thread sleep 3 seconds.
    
    printf("Main thread calling pthread_cancel\n");
    pthread_cancel_func(thread);
    
    void* result;
    pthread_join(thread, &result);
    
    // check sub-thread -s canceled
    if (result == PTHREAD_CANCELED) {
        printf("Thread was canceled\n");
    }
    
    printf("Main thread exiting\n");
    return 0;
}
```

### NOTE

This interface is differentiated at compile time, only enabled when `musl_extended_function` is true, and requires explicit enabling to fully utilize the functionality using [set_pthread_extended_function_policy](./PTHREAD_EXTENDED_FUNCTION_POLICY.md).

### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).