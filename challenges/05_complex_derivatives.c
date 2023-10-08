#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>

double complex derivative(double complex n, 
                          double complex f(double complex), 
                          double complex eps) {
    return (f(n + eps) - f(n)) / eps;
}

int main(void) {
    double complex eps = 1.E-10 + 1.E-10 * I;
    double complex n = 2 + 0.5 * I;
    double complex d = derivative(n, csinh, eps);
    double complex a = ccosh(n);

    printf("     n: %f + %f\n", creal(n), cimag(n));
    printf("Result: %f + %f\n", creal(d), cimag(d));
    printf("Actual: %f + %f\n", creal(a), cimag(a));


    return EXIT_SUCCESS;
}
