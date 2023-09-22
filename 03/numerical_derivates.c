#include <stdio.h>
#include <math.h>

double f(double F(double), double x, double eps) {
    return (F(x + eps) - F(x)) / eps;  
}

int main(void) {
    printf("f(sin(x)): %f, cos(x): %f\n", f(sin, 2, 1E-10), cos(2));
}