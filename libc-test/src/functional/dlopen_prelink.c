#include "dlopen_util.h"
#include "dlprelink.h"

#include <sys/wait.h>

#define FOO "/data/local/tmp/libfoo.so"
#define FN "call"

static void dlopen_prelinked_repeated(void)
{
	void *h = dlopen(FOO, RTLD_LOCAL);
	if (!h) {
		t_error("dlopen %s failed: %s\n", FOO, dlerror());
		return;
	}
	void *f0 = dlsym(h, FN);
	if (!f0) {
		t_error("sym \"%s\" not found\n", FN);
		return;
	}
	if (dlclose(h) < 0) {
		t_error("dlclose %s failed\n", FOO);
		return;
	}
	h = NULL;

	pid_t pid = fork();
	if (pid < 0) {
		t_error("fork failed: %m\n");
		return;
	} else if (pid == 0) {
		h = dlopen(FOO, RTLD_LOCAL);
		if (!h) {
			t_error("dlopen %s failed: %s\n", FOO, dlerror());
			_exit(1);
		}
		void *f = dlsym(h, FN);
		if (f != f0) {
			t_error("function address mismatch\n");
			_exit(1);
		}
		_exit(0);
	}

	int ws = -1;
	if (waitpid(pid, &ws, 0) < 0) {
		t_error("waitpid failed: %m\n");
	} else if (!WIFEXITED(ws) || WEXITSTATUS(ws) != 0) {
		t_error("invalid child exit status %d\n", ws);
	}
}

int main(int argc, char *argv[])
{
	set_prelink_chk_enable();
	dlprelink();
	dlopen_prelinked_repeated();
	
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
	dlclose_recursive();
	dlclose_recursive_by_multipthread();
	dlopen_global_test();
	dlclose_exit_test();
	dlclose_with_tls_test();

	return t_status;
}
