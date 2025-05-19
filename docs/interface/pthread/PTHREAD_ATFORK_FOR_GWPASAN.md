### **PTHREAD_ATFORK_FOR_GWPASAN        OHOS Programmer's Manual    **



#### **NAME**

​       pthread_atfork_for_gwpasan - pthread_atfork specified for GWP sanitizer,
<200b>       register fork handler for GWP sanitizer to make sure prepare handler of 
<200b>       GWP sanitizer will be executed last after all the other registed prepare handler. 
<200b>       parent and child handler for GWP sanitizer will be executed first before 
<200b>       before all the other registed parent and child handler.

#### **SYNOPSIS**

​       #include <pthread.h>

       int pthread_atfork_for_gwpasan(typeof(void (void)) *prepare,
                                      typeof(void (void)) *parent,
                                      typeof(void (void)) *child);

#### **DESCRIPTION**

​       The pthread_atfork_for_gwpasan() function registers fork handlers that are
​       to be executed when fork() is called by any thread in a process.
​       The handlers are executed in the context of thread that calls fork().

<200b>       Three kinds of handler can be registered:

<200b>       1. prepare specifies a handler that is executed in the parent
<200b>          process before fork() processing starts.

<200b>       2. parent specifies a handler that is executed in the parent process
<200b>          after fork() processing completes.

<200b>       3. child specifies a handler that is executed in the child process
<200b>          after fork() processing completes.

<200b>       Any of the three arguments may be NULL is no handler is needed in<200b>       the corresponding phase of fork() processing.

<200b>       pthread_atfork() may be called multiple times
<200b>       by a process to register additional handlers. The handlers for each
<200b>       phase are called in a specified order: the prepare handlers are
called
<200b>       in reverse order of registration; the parent and child handlers
are
<200b>       called in the order of registration. GWP sanitizer will call
<200b>       pthread_atfork_gwpasan to make sure prepare handler of GWP sanitizer
<200b>       will be executed after other prepare handlers, parent and child
<200b>       handlers of GWP sanitizer will be executed before others.

#### **RETURN VALUE**

​       On success, pthread_atfork_for_gwpasan() returns zero. On error, it returns:
​       an error number.

#### **ERRORS**

​        ENOMEM Could not allocate memory to record the fork handler list
entry.

#### ATTRIBUTES

| Interface                    | Attribute     | Value      |
| ---------------------------- | ------------- | ---------- |
| pthread_atfork_for_gwpasan() | Thread safety | MT-unsafe  |
|                              | Signal safety | Not Safe   |

#### HISTORY

​       -- 2025 

#### NOTES

​      The pthread_atfork_for_gwpasan() is specified for GWP sanitizer.
<200b>      The behavior will not be guranteed under other situations.
<200b>      pthread_atfork() should be used to register fork handler. 

#### EXAMPLES

```c
 #include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>
 #include <unistd.h>
 
 void prepare() {
     printf("Prepare handler called\n");
 }

 void parent() {
     printf("Parent handler called\n");
 }

 void child() {
     printf("Child handler called\n");
 } 

 int main(void) {
     if (pthread_atfork_for_gwpasan(prepare, parent, child) != 0) {
         perror("pthread_atfork_for_gwp_asan");
         exit(EXIT_FAILURE);
     }

     

     pid_t pid = fork();
     if (pid == -1) {
         perror("fork");
         exit(EXIT_FAILURE);
     } else if (pid == 0) {
         printf("Child process\n");
         exit(EXIT_SUCCESS);
     } else {
         printf("Parent process\n");
         wait(NULL);
     }

     return 0;
  }
```


#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitee.com/openharmony/third_party_musl/blob/master/docs/)
