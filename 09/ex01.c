#include <stdio.h>
#include <stdlib.h>

void shift(double* p0, double* p1, double* p2) {
    double tmp = *p2;
    *p2 = *p1;
    *p1 = *p0;
    *p0 = tmp;
}

int main(void) {
    double a = 0;
    double b = 1;
    double c = 2;

    printf("a: %f, b: %f, c: %f\n", a, b, c);
    shift(&a, &b, &c);
    printf("a: %f, b: %f, c: %f\n", a, b, c);
    return EXIT_SUCCESS;
}
