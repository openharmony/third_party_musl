## PTHREAD_YIELD &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### NAME

​       pthread_yield - yield the current thread's claim to the processor.

#### SYNOPSIS

       #include <pthread.h>

       int pthread_yield(void);

#### DESCRIPTION

The pthread_yield() function causes the calling thread to relinquish the CPU, allowing other ready threads to run. This function is equivalent to sched_yield().

If the implementation is built without musl_extended_function support, pthread_yield() returns ENOSYS.

#### RETURN VALUE

On success, pthread_yield() returns 0. When musl_extended_function is disabled, pthread_yield() returns ENOSYS.

#### ERRORS

No errors are defined.

#### ATTRIBUTES

| Interface       | Attribute     | Value    |
| --------------- | ------------- | -------- |
| pthread_yield() | Thread safety | MT-safe  |
|                 | Signal safety | Not Safe |

#### REFERS

Refer to [sched_yield](https://pubs.opengroup.org/onlinepubs/9699919799/functions/sched_yield.html).

#### EXAMPLES

```c
#include <pthread.h>
#include <stdio.h>

#define LOOP_NUM (5)

static void *worker(void *arg)
{
    int id = (int)(long)arg;
    for (int i = 0; i < LOOP_NUM; i++) {
        printf("thread %d running\n", id);
        pthread_yield();
    }
    return NULL;
}

int main(void)
{
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1, NULL, worker, (void *)(long)1);
    pthread_create(&tid2, NULL, worker, (void *)(long)2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
```

#### NOTE

This interface is always declared. When `musl_extended_function` is disabled, pthread_yield() returns ENOSYS.

#### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
