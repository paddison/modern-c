#include <stdio.h>
#include <stdlib.h>

void factor(size_t n) {
    printf("%lu: ", n);
    size_t f = 2;

    // while n is divisible by 2
    while (!(n % 2)) {
        printf("%lu ", f);
        n /= 2;
    }
    ++f;
    while (f * f < n) {
        if (!(n % 2)) {
            printf("%lu", f);
        } else {
            f += 2;
        }
    }

    if (n != 1) {
        printf("%lu", n);
    }
    printf("\n");
}

void factors(size_t N, size_t A[N]) {
    for (size_t i = 0; i < N; ++i) {
        factor(A[i]);
    }
}

int main(void) {
    factor(124);
    size_t A[5] = { 10, 13, 15, 4, 10592 };
    factors(5, A);
    EXIT_SUCCESS;
}
