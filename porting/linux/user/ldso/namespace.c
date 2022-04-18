#include "strops.h"
#include "namespace.h"

static ns_t g_ns_default;
static nslist g_ns_list;

#define NSLIST_DEFAULT_SIZE 16
#define DSOLIST_DEFAULT_SIZE 16
#define INHERIT_DEFAULT_SIZE 16

static ns_inherit_list *nsinherits_alloc()
{
    ns_inherit_list *nsinl;
    nsinl = (ns_inherit_list *)calloc(1, sizeof *nsinl) ;

    if (nsinl) {
        nsinl->size = INHERIT_DEFAULT_SIZE;
        nsinl->inherits = (ns_inherit **)calloc(INHERIT_DEFAULT_SIZE, sizeof *nsinl->inherits);
        if (!nsinl->inherits) {
            free(nsinl);
            nsinl = NULL;
        }
    }
    return nsinl;
}

static void nsinherits_free(ns_inherit_list *nsinl)
{
    if (!nsinl) return;
    for (size_t i=0; i<nsinl->num; i++) {
        strlist_free(nsinl->inherits[i]->shared_libs);
        free(nsinl->inherits[i]);
    }
    free(nsinl->inherits);
    free(nsinl);
}

static void nsinherits_realloc(ns_inherit_list *nsinl)
{
    if (!nsinl) return;
    size_t size = 2*nsinl->size;
    if (size) {
        ns_inherit **inherits;
        inherits = (ns_inherit **)realloc(nsinl->inherits, size * (sizeof *nsinl->inherits));
        if (!inherits) return;
        nsinl->size = size;
        nsinl->inherits = inherits;
    }
    return;
}

static dsolist *dsolist_alloc()
{
    dsolist *dsol;
    dsol = (dsolist *)calloc(1, sizeof *dsol) ;

    if (dsol) {
        dsol->size = DSOLIST_DEFAULT_SIZE ;
        dsol->dsos = (struct dso**)calloc(DSOLIST_DEFAULT_SIZE, sizeof *dsol->dsos);
        if (!dsol->dsos) {
            free(dsol);
            dsol = NULL;
        }
    }
    return dsol ;
}

static void dsolist_realloc(dsolist *dsol)
{
    if (!dsol) return;
    size_t size = 2*dsol->size;
    if (size) {
        struct dso **ds;
        ds = (struct dso **)realloc(dsol->dsos, size * (sizeof *dsol->dsos));
        if (!ds) return;
        dsol->size = size;
        dsol->dsos = ds;
    }
    return;
}

ns_t *ns_alloc()
{
    ns_t *nst = (ns_t *)calloc(1, sizeof *nst);
    nst->ns_dsos = dsolist_alloc();
    if (!nst->ns_dsos) {
        free(nst);
        nst = NULL;
    }
    return nst;
}

void ns_free(ns_t *ns)
{
    if (!ns) return;
    free(ns->ns_name);
    free(ns->env_paths);
    free(ns->lib_paths);
    strlist_free(ns->permitted_paths);
    strlist_free(ns->allowed_libs);
    nsinherits_free(ns->ns_inherits);
    free(ns);
}

void ns_add_dso(ns_t *ns, struct dso *dso)
{
    if (!ns||!dso) return;
    if (!ns->ns_dsos) {
        ns->ns_dsos = dsolist_alloc();
    }
    if (!ns->ns_dsos) return;
    if (ns->ns_dsos->num == ns->ns_dsos->size) {
        /* if list is full, realloc size to double*/
        dsolist_realloc(ns->ns_dsos);
    }
    if (ns->ns_dsos->num < ns->ns_dsos->size) {
        /* realloc succ */
        ns->ns_dsos->dsos[ns->ns_dsos->num] = dso;
        ns->ns_dsos->num++;
    }
    return;
}

nslist *nslist_init()
{
    g_ns_list.size = NSLIST_DEFAULT_SIZE;
    g_ns_list.num = 0;
    g_ns_list.nss = (ns_t **)calloc(NSLIST_DEFAULT_SIZE, sizeof *g_ns_list.nss);
    if (!g_ns_list.nss) {
        return NULL;
    }
    return &g_ns_list;
}

static void nslist_realloc()
{
    size_t size = 2*g_ns_list.size;
    if (size) {
        ns_t **nss;
        nss = (ns_t **)realloc(g_ns_list.nss, size * (sizeof *g_ns_list.nss));
        if (!nss) return;
        g_ns_list.size = size;
        g_ns_list.nss = nss;
    }
    return;
}

void nslist_add_ns(ns_t *ns)
{
    if (!ns) return;
   
    if (g_ns_list.num == g_ns_list.size) {
        /* if list is full, realloc size to double*/
        nslist_realloc();
    }
    if (g_ns_list.num < g_ns_list.size) {
        /* realloc succ */
        g_ns_list.nss[g_ns_list.num] = ns;
        g_ns_list.num++;
    }
    return;
}

ns_t *get_default_ns()
{
    return &g_ns_default;
}

/* set namespace attributes*/
void ns_set_name(ns_t *ns, const char *name)
{
    if (!ns || !name) return;
    if (ns->ns_name) free(ns->ns_name);
    ns->ns_name = strdup(name);
    strtrim(ns->ns_name);
}

void ns_set_env_paths(ns_t *ns, const char *env_paths)
{
    if (!ns) return;
    if (ns->env_paths) free(ns->env_paths);
    if (env_paths) {
        ns->env_paths = strdup(env_paths);
        strtrim(ns->env_paths);
    } else {
        ns->env_paths = NULL;
    }
}

void ns_set_lib_paths(ns_t *ns, const char *lib_paths)
{
    if (!ns) return;
    if (ns->lib_paths) free(ns->lib_paths);
    if (lib_paths) {
        ns->lib_paths = strdup(lib_paths);
        strtrim(ns->lib_paths);
    } else {
        ns->lib_paths = NULL;
    }
}

void ns_set_permitted_paths(ns_t *ns,const char *permitted_paths)
{
    if (!ns) return;
    if (ns->permitted_paths) strlist_free(ns->permitted_paths);
    ns->permitted_paths = strsplit(permitted_paths,":");
}

void ns_set_separated(ns_t *ns,bool separated)
{
    if (!ns) return;
    ns->separated = separated;
}

void ns_set_allowed_libs(ns_t *ns, const char *allowed_libs)
{
    if (!ns) return;
    
    if (ns->allowed_libs) strlist_free(ns->allowed_libs);
    ns->allowed_libs = NULL;
    if (allowed_libs) {
        /* if setted and not empty, split to list. */
        char *a_libs = strdup(allowed_libs);
        if (strtrim(a_libs) > 0) ns->allowed_libs = strsplit(a_libs,":");
        free(a_libs);
    }
}

ns_t *find_ns_by_name(const char *ns_name)
{
    if (!ns_name) return NULL;
    if (strcmp(NS_DEFAULT_NAME, ns_name)==0) return get_default_ns();
    for (size_t i=0; i< g_ns_list.num; i++) {
        if (strcmp(g_ns_list.nss[i]->ns_name, ns_name)==0) {
            return g_ns_list.nss[i];
        }
    }
    return NULL;
}

static ns_inherit *find_ns_inherit(ns_t *ns, ns_t *inherited)
{
    if (!ns||!inherited) return NULL;
    if (ns->ns_inherits) {
        for (size_t i=0; i<ns->ns_inherits->num; i++) {
            if (ns->ns_inherits->inherits[i]->inherited_ns == inherited) return ns->ns_inherits->inherits[i];
        }
    }
    return NULL;
}

void ns_add_inherit(ns_t *ns, ns_t *ns_inherited, const char *shared_libs)
{   
    bool need_add = false;
    if (!ns||!ns_inherited) return;

    ns_inherit *inherit = find_ns_inherit(ns, ns_inherited);
    if (!inherit) {
        inherit = calloc(1, sizeof *inherit);
        if (!inherit) return;
        inherit->inherited_ns = ns_inherited;
        need_add = true;
    }
    
    if (inherit->shared_libs) {
        strlist_free(inherit->shared_libs);
        inherit->shared_libs = NULL;
    }

    /* if setted and not empty, split to list. */
    if (shared_libs) {
        char *s_libs = strdup(shared_libs);
        if (strtrim(s_libs) > 0) inherit->shared_libs = strsplit(shared_libs,":");
        free(s_libs);
    }

    if (!need_add) return;

    if (!ns->ns_inherits) {
        ns->ns_inherits = nsinherits_alloc();
    }

    if (!ns->ns_inherits) {
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        free(inherit);
        return;
    }

    if (ns->ns_inherits->num == ns->ns_inherits->size) {
        /* if list is full, realloc size to double*/
        nsinherits_realloc(ns->ns_inherits);
    }

    if (ns->ns_inherits->num < ns->ns_inherits->size) {
        /* realloc succ */
        ns->ns_inherits->inherits[ns->ns_inherits->num] = inherit;
        ns->ns_inherits->num++;
    } else {
        /* realloc failed */
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        free(inherit);
    }
    return;
}

/* check library's pathname if accessible in this namespace */
bool is_accessible(ns_t *ns,const char *lib_pathname)
{
    if (!ns->separated) {
        return true;
    }
    if (ns->allowed_libs) {
        char *shortname = strrchr(lib_pathname, '/');
        if (shortname) {
            shortname += 1;
            size_t i = 0;
            for (; i<ns->allowed_libs->num; i++) {
                if (strcmp(shortname, ns->allowed_libs->strs[i]) == 0) {
                    break;
                }
            }
            if (i >= ns->allowed_libs->num) return false;
        }
    }
    strlist  *paths;
    if (ns->env_paths && (paths = strsplit(ns->env_paths, ":"))) {
        for (size_t i=0; i<paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname+len +1, '/')) {
                strlist_free(paths);
                return true;
            }
        }
        strlist_free(paths);
    }

    if (ns->lib_paths && (paths = strsplit(ns->lib_paths, ":"))) {
        for (size_t i=0; i<paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname+len +1, '/')) {
                strlist_free(paths);
                return true;
            } 
        }
        strlist_free(paths);    
    }

    if (ns->permitted_paths) {  
        for (size_t i=0; i<ns->permitted_paths->num; i++) {
            size_t len = strlen(ns->permitted_paths->strs[i]);
            if (strncmp(lib_pathname, ns->permitted_paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/') {
                return true;
            }
        }
    }

    return false;
}

bool is_sharable(ns_inherit *inherit, const char *lib_name)
{
    if (inherit && lib_name && inherit->shared_libs) {
        for (size_t i=0; i<inherit->shared_libs->num; i++) {
            if (strcmp(inherit->shared_libs->strs[i], lib_name)==0) {
                return true;
            }
        }
        return false;
    }
    return true;
}
