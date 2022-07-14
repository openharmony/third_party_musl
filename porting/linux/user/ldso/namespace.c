#include "namespace.h"

#include "ld_log.h"
#include "strops.h"

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
            LD_LOGW("nsinherits_alloc failed,return NULL!\n");
            free(nsinl);
            nsinl = NULL;
        }
    }
    return nsinl;
}

static void nsinherits_free(ns_inherit_list *nsinl)
{
    if (!nsinl) {
        LD_LOGW("nsinherits_free failed,nsinl is NULL!\n");
        return;
    }
    for (size_t i=0; i<nsinl->num; i++) {
        strlist_free(nsinl->inherits[i]->shared_libs);
        free(nsinl->inherits[i]);
    }
    free(nsinl->inherits);
    free(nsinl);
}

static void nsinherits_realloc(ns_inherit_list *nsinl)
{
    if (!nsinl) {
        LD_LOGW("nsinherits_realloc failed,nsinl is NULL!\n");
        return;
    }
    size_t size = 2*nsinl->size;
    if (size) {
        ns_inherit **inherits;
        inherits = (ns_inherit **)realloc(nsinl->inherits, size * (sizeof *nsinl->inherits));
        if (!inherits) {
            LD_LOGW("nsinherits_realloc failed!\n");
            return;
        }
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
            LD_LOGW("dsolist_alloc failed,return NULL!\n");
            free(dsol);
            dsol = NULL;
        }
    }
    return dsol ;
}

static void dsolist_realloc(dsolist *dsol)
{
    if (!dsol) {
        LD_LOGW("dsolist_realloc failed,dsol is NULL!\n");
        return;
    }
    size_t size = 2*dsol->size;
    if (size) {
        struct dso **ds;
        ds = (struct dso **)realloc(dsol->dsos, size * (sizeof *dsol->dsos));
        if (!ds) {
            LD_LOGW("dsolist_realloc failed!\n");
            return;
        }
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
        LD_LOGW("ns_alloc failed,return NULL!\n");
        free(nst);
        nst = NULL;
    }
    return nst;
}

void ns_free(ns_t *ns)
{
    if (!ns) {
        LD_LOGW("ns_free failed!\n");
        return;
    }
    free(ns->ns_name);
    free(ns->env_paths);
    free(ns->lib_paths);
    free(ns->asan_lib_paths);
    strlist_free(ns->permitted_paths);
    strlist_free(ns->asan_permitted_paths);
    strlist_free(ns->allowed_libs);
    nsinherits_free(ns->ns_inherits);
    free(ns);
}

void ns_add_dso(ns_t *ns, struct dso *dso)
{
    if (!ns||!dso) {
        LD_LOGW("ns_add_dso failed,ns or dso is NULL!\n");
        return;
    }
    if (!ns->ns_dsos) {
        ns->ns_dsos = dsolist_alloc();
    }
    if (!ns->ns_dsos) {
        LD_LOGW("ns_add_dso failed,ns->ns_dsos is NULL!\n");
        return;
    }
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
        LD_LOGW("nslist_init failed!\n");
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
        if (!nss) {
            LD_LOGW("nslist_realloc failed!\n");
            return;
        }
        g_ns_list.size = size;
        g_ns_list.nss = nss;
    }
    return;
}

void nslist_add_ns(ns_t *ns)
{
    if (!ns) {
        LD_LOGW("nslist_add_ns failed,ns is NULL!\n");
        return;
    }
   
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
    if (!ns || !name) {
        LD_LOGW("ns_set_name failed,ns or name is NULL!\n");
        return;
    }
    if (ns->ns_name) free(ns->ns_name);
    ns->ns_name = strdup(name);
    strtrim(ns->ns_name);
    LD_LOGD("ns_set_name ns_name:%s.\n", ns->ns_name);
}

void ns_set_env_paths(ns_t *ns, const char *env_paths)
{
    if (!ns) {
        LD_LOGW("ns_set_env_paths failed,ns is NULL!\n");
        return;
    }
    if (ns->env_paths) free(ns->env_paths);
    if (env_paths) {
        ns->env_paths = strdup(env_paths);
        strtrim(ns->env_paths);
    } else {
        ns->env_paths = NULL;
    }
    LD_LOGD("ns_set_env_paths ns[%s] env_paths:%s.\n", ns->ns_name, ns->env_paths);
}

void ns_set_lib_paths(ns_t *ns, const char *lib_paths)
{
    if (!ns) {
        LD_LOGW("ns_set_lib_paths failed,ns is NULL!\n");
        return;
    }
    if (ns->lib_paths) free(ns->lib_paths);
    if (lib_paths) {
        ns->lib_paths = strdup(lib_paths);
        strtrim(ns->lib_paths);
    } else {
        ns->lib_paths = NULL;
    }
    LD_LOGD("ns_set_lib_paths ns[%s] lib_paths:%s.\n", ns->ns_name, ns->lib_paths);
}

void ns_set_asan_lib_paths(ns_t *ns, const char *asan_lib_paths)
{
    if (!ns) {
        LD_LOGW("ns_set_asan_lib_paths failed,ns is NULL!\n");
        return;
    }
    if (ns->asan_lib_paths) {
        free(ns->asan_lib_paths);
    }
    if (asan_lib_paths) {
        ns->asan_lib_paths = strdup(asan_lib_paths);
        strtrim(ns->asan_lib_paths);
    } else {
        ns->asan_lib_paths = NULL;
    }
    LD_LOGD("ns_set_asan_lib_paths ns[%s] asan_lib_paths:%s.\n", ns->ns_name, ns->asan_lib_paths);
}

void ns_set_permitted_paths(ns_t *ns, const char *permitted_paths)
{
    if (!ns) {
        LD_LOGW("ns_set_permitted_paths failed,ns is NULL!\n");
        return;
    }
    if (ns->permitted_paths) strlist_free(ns->permitted_paths);
    ns->permitted_paths = strsplit(permitted_paths,":");
    LD_LOGD("ns_set_permitted_paths ns[%s] permitted_paths:%s.\n", ns->ns_name, permitted_paths);
}

void ns_set_asan_permitted_paths(ns_t *ns, const char *asan_permitted_paths)
{
    if (!ns) {
        LD_LOGW("ns_set_asan_permitted_paths failed,ns is NULL!\n");
        return;
    }
    if (ns->asan_permitted_paths) {
        strlist_free(ns->asan_permitted_paths);
    }
    ns->asan_permitted_paths = strsplit(asan_permitted_paths, ":");
    LD_LOGD("ns_set_asan_permitted_paths ns[%s] asan_permitted_paths:%s.\n", ns->ns_name, asan_permitted_paths);
}

void ns_set_separated(ns_t *ns, bool separated)
{
    if (!ns) {
        LD_LOGW("ns_set_separated failed,ns is NULL!\n");
        return;
    }
    ns->separated = separated;
    LD_LOGD("ns_set_separated ns[%s] separated:%d.\n", ns->ns_name, ns->separated);
}

void ns_set_allowed_libs(ns_t *ns, const char *allowed_libs)
{
    if (!ns) {
        LD_LOGW("ns_set_allowed_libs failed,ns is NULL!\n");
        return;
    }
    
    if (ns->allowed_libs) strlist_free(ns->allowed_libs);
    ns->allowed_libs = NULL;
    if (allowed_libs) {
        /* if setted and not empty, split to list. */
        char *a_libs = strdup(allowed_libs);
        if (strtrim(a_libs) > 0) ns->allowed_libs = strsplit(a_libs,":");
        free(a_libs);
    }
    LD_LOGD("ns_set_allowed_libs ns[%s] allowed_libs:%s.\n", ns->ns_name, allowed_libs);
}

ns_t *find_ns_by_name(const char *ns_name)
{
    if (!ns_name) {
        LD_LOGW("find_ns_by_name failed,ns_name is NULL!\n");
        return NULL;
    }
    if (strcmp(NS_DEFAULT_NAME, ns_name)==0) {
        LD_LOGD("find_ns_by_name return default namespace!\n");
        return get_default_ns();
    }
    for (size_t i=0; i< g_ns_list.num; i++) {
        if (strcmp(g_ns_list.nss[i]->ns_name, ns_name)==0) {
            return g_ns_list.nss[i];
        }
    }
    LD_LOGD("find_ns_by_name ns_name[%s] failed,return NULL!\n", ns_name);
    return NULL;
}

static ns_inherit *find_ns_inherit(ns_t *ns, ns_t *inherited)
{
    if (!ns||!inherited) {
        LD_LOGW("find_ns_inherit failed,ns or inherited is NULL!\n");
        return NULL;
    }
    if (ns->ns_inherits) {
        for (size_t i=0; i<ns->ns_inherits->num; i++) {
            if (ns->ns_inherits->inherits[i]->inherited_ns == inherited) return ns->ns_inherits->inherits[i];
        }
    }
    LD_LOGD("find_ns_inherit ns[%s] ns_inherited[%s] failed,return NULL!\n", ns->ns_name, inherited->ns_name);
    return NULL;
}

void ns_add_inherit(ns_t *ns, ns_t *ns_inherited, const char *shared_libs)
{   
    bool need_add = false;
    if (!ns||!ns_inherited) {
        LD_LOGW("ns_add_inherit failed,ns or inherited is NULL!\n");
        return;
    }

    ns_inherit *inherit = find_ns_inherit(ns, ns_inherited);
    if (!inherit) {
        inherit = calloc(1, sizeof *inherit);
        if (!inherit) {
            LD_LOGW("ns_add_inherit ns[%s] ns_inherited[%s] calloc failed!\n", ns->ns_name, ns_inherited->ns_name);
            return;
        }
        inherit->inherited_ns = ns_inherited;
        need_add = true;
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] need_add is true.\n", ns->ns_name, ns_inherited->ns_name);
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

    if (!need_add) {
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] not need_add!\n", ns->ns_name, ns_inherited->ns_name);
        return;
    }

    if (!ns->ns_inherits) {
        ns->ns_inherits = nsinherits_alloc();
    }

    if (!ns->ns_inherits) {
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] nsinherits_alloc failed!\n",
                ns->ns_name, ns_inherited->ns_name);
        free(inherit);
        return;
    }

    if (ns->ns_inherits->num == ns->ns_inherits->size) {
        /* if list is full, realloc size to double*/
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] list is full, realloc size to double!\n",
                ns->ns_name, ns_inherited->ns_name);
        nsinherits_realloc(ns->ns_inherits);
    }

    if (ns->ns_inherits->num < ns->ns_inherits->size) {
        /* realloc succ */
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] realloc success!\n", ns->ns_name, ns_inherited->ns_name);
        ns->ns_inherits->inherits[ns->ns_inherits->num] = inherit;
        ns->ns_inherits->num++;
    } else {
        /* realloc failed */
        LD_LOGD("ns_add_inherit ns[%s] ns_inherited[%s] realloc failed!\n", ns->ns_name, ns_inherited->ns_name);
        if (inherit->shared_libs) strlist_free(inherit->shared_libs);
        free(inherit);
    }
    return;
}

/* check library's pathname if accessible in this namespace */
bool is_accessible(ns_t *ns, const char *lib_pathname, bool is_asan, bool check_inherited)
{
    if (check_inherited && !ns->separated) {
        LD_LOGD("is_accessible ns [%s] is not separated, return true.\n", ns->ns_name);
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
            if (i >= ns->allowed_libs->num) {
                LD_LOGD("is_accessible ns [%s] lib_pathname [%s] is not in allowed_libs, return false.\n",
                        ns->ns_name, lib_pathname);
                return false;
            }
        }
    }
    strlist  *paths;
    if (ns->env_paths && (paths = strsplit(ns->env_paths, ":"))) {
        for (size_t i=0; i<paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname+len +1, '/')) {
                LD_LOGD("is_accessible ns [%s] lib_pathname [%s] in env_paths, return true.\n",
                        ns->ns_name, lib_pathname);
                strlist_free(paths);
                return true;
            }
        }
        strlist_free(paths);
    }

    if (is_asan) {
        if (check_asan_path(ns, lib_pathname)) {
            LD_LOGD("is_accessible ns [%s] lib_pathname [%s] check_asan_path success, return true.\n",
                    ns->ns_name, lib_pathname);
            return true;
        }
    }

    if (ns->lib_paths && (paths = strsplit(ns->lib_paths, ":"))) {
        for (size_t i=0; i<paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname+len +1, '/')) {
                strlist_free(paths);
                LD_LOGD("is_accessible ns [%s] lib_pathname [%s] in lib_paths, return true.\n",
                        ns->ns_name, lib_pathname);
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
                LD_LOGD("is_accessible ns [%s] lib_pathname [%s] in permitted_paths, return true.\n",
                        ns->ns_name, lib_pathname);
                return true;
            }
        }
    }
    return false;
}

bool check_asan_path(ns_t *ns, const char *lib_pathname)
{
    strlist  *paths;
    if (ns->asan_lib_paths && (paths = strsplit(ns->asan_lib_paths, ":"))) {
        for (size_t i = 0; i < paths->num; i++) {
            size_t len = strlen(paths->strs[i]);
            if (strncmp(lib_pathname, paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/' &&
                !strchr(lib_pathname+len +1, '/')) {
                strlist_free(paths);
                LD_LOGD("check_asan_path ns [%s] lib_pathname [%s] in asan_lib_paths, return true.\n",
                        ns->ns_name, lib_pathname);
                return true;
            } 
        }
        strlist_free(paths);
    }
    if (ns->asan_permitted_paths) {
        for (size_t i = 0; i < ns->asan_permitted_paths->num; i++) {
            size_t len = strlen(ns->asan_permitted_paths->strs[i]);
            if (strncmp(lib_pathname, ns->asan_permitted_paths->strs[i], len) == 0 &&
                lib_pathname[len] == '/') {
                LD_LOGD("check_asan_path ns [%s] lib_pathname [%s] in asan_permitted_paths, return true.\n",
                        ns->ns_name,lib_pathname);
                return true;
            }
        }
    }
    LD_LOGD("check_asan_path ns [%s] lib_pathname [%s] failed, return false.\n", ns->ns_name, lib_pathname);
    return false;
}

bool is_sharable(ns_inherit *inherit, const char *lib_name)
{
    if (inherit && lib_name && inherit->shared_libs) {
        for (size_t i=0; i<inherit->shared_libs->num; i++) {
            if (strcmp(inherit->shared_libs->strs[i], lib_name)==0) {
                LD_LOGD("is_sharable inherit [%s] lib_name [%s] found, return true.\n", inherit->inherited_ns->ns_name, lib_name);
                return true;
            }
        }
        LD_LOGD("is_sharable inherit [%s] lib_name [%s] not found, return false.\n", inherit->inherited_ns->ns_name, lib_name);
        return false;
    }
     LD_LOGD("is_sharable shared_libs not config, return true.\n");
    return true;
}
