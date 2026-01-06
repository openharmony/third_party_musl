## __REGISTER_ATFORK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       __register_atfork - register fork handlers.

#### **SYNOPSIS**   
       
       #include <pthread.h>

       int __register_atfork(void (*prepare) (void), void (*parent) (void), void (*child) (void), void *dso_handle);

#### **DESCRIPTION**

​       The __register_atfork() function is similar to pthread_atfork, registers fork handlers that are to be executed when fork is called by any thread in a process. The handlers are executed in the context of the thread that calls fork.

​       Four kinds of handler can be registered:  

​         •  prepare specifies a handler that is executed in the parent process before fork processing starts.

​         •  parent specifies a handler that is executed in the parent process after fork processing completes.

​         •  child specifies a handler that is executed in the child process after fork processing completes.

​         •  dso_handle is an internal variable with no practical significance and should be passed directly as a null value.

​        If the corresponding stage in fork processing does not require a handler, any of the first three parameters can be NULL.

#### **RETURN VALUE**

​       On success, __register_atfork() returns zero.  On error, it returns an error number.  __register_atfork() may be called multiple times by a process to register additional handlers.  The handlers for each phase are called in a specified order: the prepare handlers are called in reverse order of registration; the parent and child handlers are called in the order of registration.

#### **ERRORS**

​       The __register_atfork() function shall fail if:  
​       **ENOMEM**: Insufficient memory exists to record the fork handler functions

#### ATTRIBUTES

| Interface           | Attribute     | Value    |
| ------------------- | ------------- | -------- |
| __register_atfork() | Thread safety | MT-safe  |
|                     | Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This function is ‌exclusively designed for PC environments‌.​

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define TEST_THREAD_SLEEP 3

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;
static int gprepare_flag = 0;
static int gparent_flag = 0;
static int gchild_flag = 0;

void reset_data(void)
{
    gprepare_flag = 0;
    gparent_flag = 0;
    gchild_flag = 0;
}

static void prepare(void)
{
    int ret = pthread_mutex_lock(&glock);
    if (ret == 0) {
        printf("prepare callback: lock success\n");
        gprepare_flag++;
    } else {
        printf("prepare callback: lock fail, ret = %d\n", ret);
    }
}

static void parent(void)
{
    int ret = pthread_mutex_unlock(&glock);
    if (ret == 0) {
        printf("parent callback: unlock success\n");
        gparent_flag++;
    } else {
        printf("parent callback: unlock fail, ret = %d\n", ret);
    }
}

static void child(void)
{
    int ret = pthread_mutex_unlock(&glock);
    if (ret == 0) {
        printf("child callback: unlock success\n");
        gchild_flag++;
    } else {
        printf("child callback: unlock fail, ret = %d\n", ret);
    }
}

static void *thread_fun(void *arg)
{
    pthread_mutex_lock(&glock);
    sleep(TEST_THREAD_SLEEP);
    pthread_mutex_unlock(&glock);
    sleep(1);
    return NULL;
}

int main(void) {
    reset_data();
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    printf("pthread_create: ret=%d\n", ret);
    if (ret != 0) {
        printf("pthread_create fail");
        return -1;
    }

    ret = __register_atfork(prepare, parent, child, NULL);
    printf("__register_atfork: ret=%d\n", ret);
    if (ret != 0) {
        printf("__register_atfork fail");
        return -1;
    }
    sleep(1);

    pid_t pid = fork();
    if (pid == 0) {
        pthread_mutex_lock(&glock);
        sleep(1);
        pthread_mutex_unlock(&glock);
        if (gchild_flag == 1) {
            printf("gchild_flag verify success\n");
        } else {
            printf("gchild_flag verify fail: %d", gchild_flag);
        }
        _exit(0);
    } else if (pid > 0) {
        pthread_join(tid, NULL);

        if (gprepare_flag == 1) {
            printf("gprepare_flag verify success\n");
        } else {
            printf("gprepare_flag verify fail: %d", gprepare_flag);
        }

        if (gparent_flag == 1) {
            printf("gparent_flag verify success\n");
        } else {
            printf("gparent_flag verify fail: %d", gparent_flag);
        }
    } else {
        printf("fork failed");
        return -1;
    }
    return 0;
}
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)