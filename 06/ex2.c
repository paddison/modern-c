#include <time.h>
#include <stdlib.h>

typedef struct timespec timespec;

timespec time_spec_diff(timespec t0, timespec t1) {
    timespec diff = {
        .tv_sec = labs(t0.tv_sec - t1.tv_sec),
        .tv_nsec = labs(t0.tv_nsec - t1.tv_nsec),
    };
    return diff;
}
