#include <stdlib.h>
#include <stdio.h>

int main(void) {
    size_t length = 8000000000;
    double* largeVec = malloc(length * sizeof *largeVec);

    for (size_t i = 0; i < length; ++i) {
        largeVec[i] = 0.0;
    }

    free(largeVec);
    return EXIT_SUCCESS;
}
