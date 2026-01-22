## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



### NAME

​       pthread_cancel - send a cancelation request to a thread.

### SYNOPSIS

       int pthread_cancel(pthread_t thread);

### DESCRIPTION

parameter `thread`: thread structure will be canceled.

The pthread_cancel() function shall request that thread be canceled. The target thread's cancelability state and type determines when the cancellation takes effect. When the cancellation is acted on, the cancellation cleanup handlers for thread shall be called. When the last cancellation cleanup handler returns, the thread-specific data destructor functions shall be called for thread. When the last destructor function returns, thread shall be terminated.

The cancellation processing in the target thread shall run asynchronously with respect to the calling thread returning from pthread_cancel().
        
### ERRORS
The following errno may be returned:
1. **ENOSYS**: No features are currently enabled.
2. **ESRCH**: No thread could be found corresponding to that specified by the given thread ID.

The pthread_cancel() function shall not return an error code of **EINTR**.

### REFERS

Refer to [pthread_cancel](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cancel.html).

### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

typedef int (*PTHREAD_CANCEL_FUNC_TYPE)(pthread_t thread);
typedef int (*PTHREAD_EXTENDED_FUNC_POLICY)(int policy);

void* worker_thread(void* arg) {
    int i = 0;
    printf("Worker thread started\n");
    
    while (1) {
        printf("Working... %d\n", ++i);
        sleep(1); // sleep is cancel-point.
    }
    
    printf("Worker thread finished normally\n"); // Will not execute here.
    return NULL;
}

int main() {
    // get pthread_cancel symbol.
    PTHREAD_CANCEL_FUNC_TYPE pthread_cancel_func = (PTHREAD_CANCEL_FUNC_TYPE)dlsym(RTLD_DEFAULT, "pthread_cancel");
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
    pthread_cancel_func(thread); // send cancel request.
    pthread_join(thread, NULL); // waiting for sub-thread end.
    printf("Main thread exiting\n");
    
    return 0;
}
```

#### NOTE

This interface is differentiated at compile time, only enabled when `musl_extended_function` is true, and requires explicit enabling to fully utilize the functionality using [set_pthread_extended_function_policy](./PTHREAD_EXTENDED_FUNCTION_POLICY.md).

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).