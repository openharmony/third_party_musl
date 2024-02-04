#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "__dirent.h"

extern uint64_t __get_dir_tag(DIR* dir);

int closedir(DIR *dir)
{
#ifdef __LITEOS__
	int ret = close(dir->fd);
#else
	int ret = fdsan_close_with_tag(dir->fd, __get_dir_tag(dir));
#endif
	free(dir);
	return ret;
}
