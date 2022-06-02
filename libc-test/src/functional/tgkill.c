#include <stdio.h>
#include <errno.h>
#include "test.h"
#include <stdlib.h>
#include <signal.h>
int main(int argc, char *argv[]){

    int tgid, tid;

    tgid = atoi(argv[1]);
    tid = atoi(argv[2]);

    if (tgkill(tgid, tid, SIGPROF) == -1 && errno != ESRCH) {
        perror("tgkill failed");
    }

    return 0;
}