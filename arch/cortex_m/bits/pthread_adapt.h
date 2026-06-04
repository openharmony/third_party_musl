#ifndef _PTHREAD_ADAPT_H
#define _PTHREAD_ADAPT_H

#include "time.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PTHREAD_MUTEX_PRIORITY            (31 - 1)

#define PTHREAD_MUTEX_ERRORCHECK_NP 2
#define PTHREAD_MUTEX_RECURSIVE_NP  1

#ifndef POSIX_MUTEX_DEFAULT_INHERIT
#define POSIX_MUTEX_DEFAULT_INHERIT
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_INHERIT, PTHREAD_MUTEX_PRIORITY, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    0 }
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_INHERIT, PTHREAD_MUTEX_PRIORITY, 0, 0 }, \
    0 }
#endif
#define PTHREAD_COND_INITIALIZER          { { 0 }, 0 }

#include <stdint.h>

typedef struct pthread_mutexattr {
    uint8_t protocol;
    uint8_t prioceiling;
    uint8_t type;
    uint8_t reserved;
} pthread_mutexattr_t;

typedef struct pthread_mutex {
    pthread_mutexattr_t stAttr;
    unsigned int sId;
} pthread_mutex_t;

typedef struct {
    clockid_t clock;
} pthread_condattr_t;

typedef struct pthread_cond {
    pthread_condattr_t stCondAttr;
    unsigned int sId;
} pthread_cond_t;

typedef struct __pthread_attr_s {
    unsigned int detachstate;
    unsigned int schedpolicy;
    int sched_priority;
    unsigned int inheritsched;
    unsigned int scope;
    unsigned int stackaddr_set;
    void* stackaddr;
    unsigned int stacksize_set;
    size_t stacksize;
} pthread_attr_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PTHREAD_ADAPT_H */
