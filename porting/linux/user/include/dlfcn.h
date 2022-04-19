#ifndef	_DLFCN_H
#define	_DLFCN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_NOLOAD 4
#define RTLD_NODELETE 4096
#define RTLD_GLOBAL 256
#define RTLD_LOCAL  0

#define RTLD_NEXT    ((void *)-1)
#define RTLD_DEFAULT ((void *)0)

#define RTLD_DI_LINKMAP 2

int    dlclose(void *);
char  *dlerror(void);
void  *dlopen(const char *, int);
void  *dlsym(void *__restrict, const char *__restrict);

/* namespace apis */
#define NS_NAME_MAX 255
typedef struct {
	char name[NS_NAME_MAX+1];
} Dl_namespace;

void dlns_init(Dl_namespace *, const char *);

/* open dso in given namespace which has own lib search paths
 * when namespace is null, it's same to dlopen() 
 * avoid using "default" as namespace, which is the default namespace */
void *dlopen_ns(Dl_namespace *, const char *, int);

/* create the namespace and set lib search paths of namespace,
 * the paths should be splited by ':'. When namespace already exist, 
 * return error */
int dlns_create(Dl_namespace *, const char *);

/* make one namespace inherit another, and so it can use shared libs by the inherited one. 
 * param1: namespace, param2: inherited namespace, param3: shared libs.
 * the shared libs should be splited by ':'. when it is null or empty, all libs can be shared.
 * one namespace can inherit or be inherited by multiple ones.
 * When namespaces do not exist, return error */
int dlns_inherit(Dl_namespace *, Dl_namespace *, const char *);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef struct {
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;
int dladdr(const void *, Dl_info *);
int dlinfo(void *, int, void *);
#endif

#if _REDIR_TIME64
__REDIR(dlsym, __dlsym_time64);
#endif

#ifdef __cplusplus
}
#endif

#endif
