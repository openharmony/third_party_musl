#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/tgkill.h>
#include "test.h"

int main(int argc, char *argv[]){

    int tgid, tid;

    tgid = atoi(argv[1]);
    tid = atoi(argv[2]);

    if (tgkill(tgid, tid, SIGPROF) == -1 && errno != ESRCH) {
        perror("tgkill failed");
    }

    return 0;
}