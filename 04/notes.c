#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int* count_elements(int* A, int* B, size_t size, int threshold) {
    for (size_t i = 0; i < size; ++i) {
        B[A[i] >= threshold] += 1;
    }
    return B;
}

int main(void) {
    // size_t represents natural numbers until SIZE_MAX
    SIZE_MAX;

    // operators
    size_t a = 45;
    size_t b = 7;
    size_t c = (+a + -b) * 2; // although b is negated by the unary operator `-`
                              // it is still well defined, even though it is unsigned!

    // booleans

    // remember that booleans in c are just 0 and 1.
    // so they can be actually used to index arrays:
    int A[12] = { 
        -1, 2, 5, -3,
        2, 3, 7, -1000,
        12, 13, 5, -2 
    };
    int B[2] = { 0, 0 };
    int threshold = 2;

    count_elements(A, B, 12, threshold);
    printf("%i elements were smaller than %i, %i larger or equal.\n", B[0], threshold, B[1]);

    return EXIT_SUCCESS;
}