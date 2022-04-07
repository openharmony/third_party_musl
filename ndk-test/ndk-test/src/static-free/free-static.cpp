#include <stdlib.h>

int main() {
    int *buf;
    buf = (int*)malloc(100*sizeof(int));
    free(buf);
    return 0;
}