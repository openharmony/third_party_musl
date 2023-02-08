#include <sys/sysinfo.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _GNU_SOURCE
#include <dirent.h>

int get_nprocs_conf()
{
	DIR *d = opendir("/sys/devices/system/cpu");
	struct dirent *de;
	unsigned int cnt = 0;

	if (!d)
		return 1;
	while ((de = readdir(d))) {
		if ((de->d_type == DT_DIR) &&
			(strlen(de->d_name) > 3) &&
			(de->d_name[0] == 'c') &&
			(de->d_name[1] == 'p') &&
			(de->d_name[2] == 'u') &&
			(isdigit(de->d_name[3])))
			cnt++;
	}
	closedir(d);
	return cnt;
}

int get_nprocs()
{
	int cnt = 1;
	FILE* fp = fopen("/sys/devices/system/cpu/online", "re");
	if (fp != NULL) {
		char buf[128];
		memset(buf, 0, sizeof(buf));
		if (!fgets(buf, sizeof(buf), fp)) {
			return 1;
		}
		cnt = 0;
		char *tmp, *p;
		tmp = strtok_r(buf, ",", &p);
		while (tmp != NULL) {
			char *tok = strstr(tmp, "-");
			if (tok) {
				tok++;
				cnt += atoi(tok) - atoi(tmp) + 1;
			} else {
				cnt += 1;
			}
			tmp = strtok_r(NULL, ",", &p);
		}
		fclose(fp);
	}
	return cnt;
}

long get_phys_pages()
{
	return sysconf(_SC_PHYS_PAGES);	
}

long get_avphys_pages()
{
	return sysconf(_SC_AVPHYS_PAGES);	
}
