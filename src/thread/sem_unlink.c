#include <semaphore.h>
#include <sys/group_ipc.h>
#include <sys/mman.h>
#include <unsupported_api.h>

int sem_unlink(const char *name)
{
	return shm_unlink(name);
}

int group_sem_unlink(const char *name, gid_t gid)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return group_shm_unlink(name, gid);
}
