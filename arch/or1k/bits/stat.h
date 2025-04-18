/* copied from kernel definition, but with padding replaced
 * by the corresponding correctly-sized userspace types. */

struct stat {
	dev_t st_dev;
	ino_t st_ino;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_rdev;
	long long __st_rdev_padding;
	off_t st_size;
	blksize_t st_blksize;
	int __st_blksize_padding;
	blkcnt_t st_blocks;
	struct {
		long tv_sec;
		long tv_nsec;
	} __st_atim32, __st_mtim32, __st_ctim32;
	unsigned __unused1[2];
 	struct timespec st_atim;
 	struct timespec st_mtim;
 	struct timespec st_ctim;
};
