#include <dlfcn.h>
#include "test.h"

#define SO_FOR_NO_DELETE "lib_for_no_delete.so"
#define SO_FOR_DLOPEN "lib_for_dlopen.so"
#define SO_LOAD_BY_LOCAL "libdlopen_for_load_by_local_dso.so"
#define SO_LOAD_BY_GLOBAL "libdlopen_for_load_by_global_dso.so"
typedef void(*TEST_PTR)(void);

void do_dlopen(const char *name, int mode)
{
	void* handle = dlopen(name, mode);

	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", name, mode, dlerror());

	if(dlclose(handle))
		t_error("dlclose %s failed : %s \n", name, dlerror());
}

void dlopen_lazy()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_LAZY);
}

void dlopen_now()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_NOW);
}

void dlopen_global()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_GLOBAL);
}

void dlopen_local()
{
	do_dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
}

void dlopen_so_used_by_dlsym()
{
	void* handle1 = dlopen(SO_LOAD_BY_LOCAL, RTLD_LOCAL);
	if(!handle1)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_LOAD_BY_LOCAL, RTLD_LOCAL, dlerror());

	// dlsym can't see the so which is loaded by RTLD_LOCAL.
	TEST_PTR for_local_ptr = dlsym(RTLD_DEFAULT, "for_local");
	if (for_local_ptr != NULL) {
		t_error("dlsym RTLD_LOCAL so(%s) should failed but get succeed.\n", "for_local");
	}

	if(dlclose(handle1))
		t_error("dlclose %s failed : %s \n", SO_LOAD_BY_LOCAL, dlerror());


	void* handle2 = dlopen(SO_LOAD_BY_GLOBAL, RTLD_GLOBAL);
	if(!handle2)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_LOAD_BY_GLOBAL, RTLD_LOCAL, dlerror());

	// dlsym can see the so which is loaded by RTLD_DEFAULT even without dependencies.
	TEST_PTR for_global_ptr = dlsym(RTLD_DEFAULT, "for_global");
	if (!for_global_ptr) {
		t_error("dlsym RTLD_GLOBAL so(%s) should succeed but get failed: %s \n", "for_global", dlerror());
	}

	if(dlclose(handle2))
		t_error("dlclose %s failed : %s \n", SO_LOAD_BY_GLOBAL, dlerror());
}

void dlopen_nodelete_and_noload()
{
	void* handle1 = dlopen(SO_FOR_NO_DELETE, RTLD_NODELETE);

	if(!handle1)
		t_error("dlopen(name=%s, mode=RTLD_NODELETE) failed: %s\n", SO_FOR_NO_DELETE, dlerror());

	if(dlclose(handle1))
		t_error("dlclose %s failed : %s \n", SO_FOR_NO_DELETE, dlerror());


	void* handle2 = dlopen(SO_FOR_NO_DELETE, RTLD_NOLOAD);
	if(!handle2)
		t_error("dlopen(name=%s, mode=RTLD_NOLOAD) failed: %s\n", SO_FOR_NO_DELETE, dlerror());

	if (handle1 != handle2) {
		t_error("dlopen %s by RTLD_NODELETE but get different handle when dlopen by RTLD_NOLOAD again.\n", SO_FOR_NO_DELETE);
	}
}

void dlopen_dlclose()
{
	void* handle = dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	handle = dlopen(SO_FOR_DLOPEN, RTLD_LOCAL);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	if(dlclose(handle))
		t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// lib should still exist in memory.
	handle = dlopen(SO_FOR_DLOPEN, RTLD_NOLOAD);
	if(!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());

	if(dlclose(handle))
			t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// It need to do one more dlclose because call dlopen by RTLD_NOLOAD add reference counting.
	if(dlclose(handle))
			t_error("dlclose %s failed : %s \n", SO_FOR_DLOPEN, dlerror());

	// dlopen and dlclose call counts match so the lib should not exist in memory.
	handle = dlopen(SO_FOR_DLOPEN, RTLD_NOLOAD);
	if(handle) {
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", SO_FOR_DLOPEN, RTLD_LOCAL, dlerror());
		dlclose(handle);
	}
}

#define DLOPEN_WEAK "libdlopen_weak.so"
typedef int (*FuncPtr_TestNumber)(int input);

void dlopen_dlclose_weak()
{
	void* handle = dlopen(DLOPEN_WEAK, RTLD_LAZY | RTLD_GLOBAL);
	if (!handle)
		t_error("dlopen(name=%s, mode=%d) failed: %s\n", DLOPEN_WEAK, RTLD_LAZY | RTLD_GLOBAL, dlerror());
	FuncPtr_TestNumber fn = (FuncPtr_TestNumber)dlsym(handle, "TestNumber");
	if (fn) {
		int ret = fn(12);
		if (ret != 0)
			t_error("weak symbol relocation error: so_name: %s, symbol: TestNumber\n", DLOPEN_WEAK);
	}
	dlclose(handle);
}

int main(int argc, char *argv[])
{
	void *h, *g;
	int *i, *i2;
	char *s;
	void (*f)(void);
	char buf[512];

	if (!t_pathrel(buf, sizeof buf, argv[0], "libdlopen_dso.so")) {
		t_error("failed to obtain relative path to dlopen_dso.so\n");
		return 1;
	}
	h = dlopen(buf, RTLD_LAZY|RTLD_LOCAL);
	if (!h)
		t_error("dlopen %s failed: %s\n", buf, dlerror());
	i = dlsym(h, "i");
	if (!i)
		t_error("dlsym i failed: %s\n", dlerror());
	if (*i != 1)
		t_error("initialization failed: want i=1 got i=%d\n", *i);
	f = (void (*)(void))dlsym(h, "f");
	if (!f)
		t_error("dlsym f failed: %s\n", dlerror());
	f();
	if (*i != 2)
		t_error("f call failed: want i=2 got i=%d\n", *i);

	g = dlopen(0, RTLD_LAZY|RTLD_LOCAL);
	if (!g)
		t_error("dlopen 0 failed: %s\n", dlerror());
	i2 = dlsym(g, "i");
	s = dlerror();
	if (i2 || s == 0)
		t_error("dlsym i should have failed\n");
	if (dlsym(g, "main") != (void*)main)
		t_error("dlsym main failed: %s\n", dlerror());

	/* close+open reinitializes the dso with glibc but not with musl */
	h = dlopen(buf, RTLD_LAZY|RTLD_GLOBAL);
	i2 = dlsym(g, "i");
	if (!i2)
		t_error("dlsym i failed: %s\n", dlerror());
	if (i2 != i)
		t_error("reopened dso should have the same symbols, want %p, got %p\n", i, i2);
	if (*i2 != 2)
		t_error("reopened dso should have the same symbols, want i2==2, got i2==%d\n", *i2);
	if (dlclose(g))
		t_error("dlclose failed: %s\n", dlerror());
	if (dlclose(h))
		t_error("dlclose failed: %s\n", dlerror());

	dlopen_lazy();
	dlopen_now();
	dlopen_global();
	dlopen_local();
	dlopen_so_used_by_dlsym();
	dlopen_nodelete_and_noload();
	dlopen_dlclose();
	dlopen_dlclose_weak();

	return t_status;
}
