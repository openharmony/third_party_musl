#ifndef _NAMESPACE_H
#define _NAMESPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "strops.h"

#define NS_DEFAULT_NAME "default"
struct dso;
/* define dso list */
typedef struct _dso_list_ {
    size_t num;
    size_t size;
    struct dso **dsos;    
} dsolist;
/* define namespace struct */
struct _ns_inherit_list_;
typedef struct _namespace_t_ {
    char *ns_name;            /* namespace name */
    char *env_paths;          /* value of LD_LIBRARY_PATH. splited by ':'. */
    char *lib_paths;          /* library search paths splited by ':'. */
    strlist *permitted_paths;    /* when separated, permitted search paths splited by ':', including sub dirs. */

    bool separated;           /* if separated */
    strlist *allowed_libs;       /* when separated, allowed library names splited by ':'. */
    dsolist *ns_dsos;         /* dso list in this namespace */
    struct _ns_inherit_list_ *ns_inherits;   /* inherit list in this namespace */
} ns_t;
/* define namespace list */
typedef struct _namespaces_list_ {
    size_t num;
    size_t size;
    ns_t **nss;
} nslist;
/* define namespace inherit */
typedef struct _namespace_inherit_ {
    ns_t *inherited_ns;       /* inherited namespace */
    strlist *shared_libs;        /* when inherited, shared library names splited by ':'. */
} ns_inherit;
/* define namespace inherit list */
typedef struct _ns_inherit_list_ {
    size_t num;
    size_t size;
    ns_inherit **inherits;
} ns_inherit_list;

/* init g_ns_list */
nslist *nslist_init();

/* namespace funcs */
ns_t *ns_alloc();
void ns_free(ns_t *ns);
void ns_set_name(ns_t *ns, const char *name);
void ns_set_env_paths(ns_t *ns, const char *env_paths);
void ns_set_lib_paths(ns_t *ns, const char *lib_paths);
void ns_set_permitted_paths(ns_t *ns, const char *permitted_paths);
void ns_set_separated(ns_t *ns, bool separated);
void ns_set_allowed_libs(ns_t *ns, const char *allowed_libs);

void ns_add_dso(ns_t *ns, struct dso *dso);
void nslist_add_ns(ns_t *ns);
void ns_add_inherit(ns_t *ns,ns_t *inherited, const char *shared_libs);

/* get default namespace */
ns_t *get_default_ns();

/* check if library pathname is accessible in the namespace */
bool is_accessible(ns_t *ns, const char *lib_pathname);

/* check if library is sharable in the inherited namespace */
bool is_sharable(ns_inherit *inherit, const char *lib_name);

/* find namespace by name */
ns_t *find_ns_by_name(const char *ns_name);

#ifdef __cplusplus
}
#endif

#endif