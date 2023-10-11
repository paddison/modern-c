#include <stdlib.h>
#include <stdio.h>

typedef union pointerInspect pointerInspect;
union pointerInspect {
    double* p;
    unsigned char bytes[sizeof(double*)];
};

int main(void) {
    double* n = malloc(sizeof(double));
    *n = 12.5;
    pointerInspect twofold = { .p = n };

    double A[3] = { 2.5, 3.12, 0.1 };
    pointerInspect I[3] = { 0 };

    for (size_t i = 0; i < 3; ++i) {
        I[i] = (pointerInspect) { .p = &A[i] };

        printf("valus is %p\n", I[i].p);
        for (size_t j = 0; j < sizeof I[i].bytes; ++j) {
            printf("byte[%zu]: 0x%.02hhX\n", j, I[i].bytes[j]);
        }
    }


    printf("valus is %p\n", twofold.p);
    for (size_t i = 0; i < sizeof twofold.bytes; ++i) {
        printf("byte[%zu]: 0x%.02hhX\n", i, twofold.bytes[i]);
    }


}
