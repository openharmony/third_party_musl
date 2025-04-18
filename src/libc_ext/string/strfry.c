#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

char *strfry(char *s) {
    static unsigned int seed;
    static int initialized;
    size_t len = strlen(s);

    if (len <= 1)
        return s;

    // Initialize seed once using PID and current time
    if (!initialized) {
        seed = time(NULL) ^ getpid();
        initialized = 1;
    }

    // Fisher-Yates shuffle using rand_r for thread-unsafe but isolated PRNG
    for (size_t i = 0; i < len - 1; ++i) {
        size_t j = i + (rand_r(&seed) % (len - i));
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

    return s;
}