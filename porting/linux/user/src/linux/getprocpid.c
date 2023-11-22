#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PID_MAX_LEN 11
#define PID_STR_LEN 5
#define STATUS_LINE_SIZE 255

static int __find_and_convert_pid(char *buf, int len)
{
	int count = 0;
	char pid_buf[PID_MAX_LEN] = {0};
	char *str = buf;

	if (len <= PID_STR_LEN) {
		return -1;
	}
	while (*str != '\0') {
		if ((*str >= '0') && (*str <= '9') && (count < PID_MAX_LEN - 1)) {
			pid_buf[count] = *str;
			count++;
			str++;
			continue;
		}
		if (count > 0) {
			break;
		}
		str++;
	}
	pid_buf[count] = '\0';
	return atoi(pid_buf);
}

static int __parse_pid_form_proc(void)
{
	const char *path = "/proc/self/status";
	char buf[STATUS_LINE_SIZE] = {0};

	FILE *fp = fopen(path, "r");
	if (!fp) {
		return -errno;
	}
	while (!feof(fp)) {
		if (!fgets(buf, STATUS_LINE_SIZE, fp)) {
			fclose(fp);
			return -errno;
		}
		if (strncmp(buf, "Tgid:", PID_STR_LEN) == 0) {
			break;
		}
	}
	(void)fclose(fp);
	return __find_and_convert_pid(buf, strlen(buf));
}

pid_t getprocpid(void)
{
	static pid_t proc_pid = -1;
	int pid;

	if (proc_pid < 0) {
		pid = __parse_pid_form_proc();
		if (pid < 1) {
			return -1;
		}
		proc_pid = (pid_t)pid;
	}
	return proc_pid;
}
