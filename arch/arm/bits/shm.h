#define SHMLBA 4096

struct shmid_ds {
	struct ipc_perm shm_perm;
	size_t shm_segsz;
	unsigned long __shm_atime_lo;
	unsigned long __shm_atime_hi;
	unsigned long __shm_dtime_lo;
	unsigned long __shm_dtime_hi;
	unsigned long __shm_ctime_lo;
	unsigned long __shm_ctime_hi;
	pid_t shm_cpid;
	pid_t shm_lpid;
	unsigned long shm_nattch;
	unsigned long __pad1;
	unsigned long __pad2;
	unsigned long __pad3;
	time_t shm_atime;
	time_t shm_dtime;
	time_t shm_ctime;
};

struct shminfo {
	unsigned long shmmax, shmmin, shmmni, shmseg, shmall, __unused1[4];
};

struct shm_info {
	int __used_ids;
	unsigned long shm_tot, shm_rss, shm_swp;
	unsigned long __swap_attempts, __swap_successes;
};
