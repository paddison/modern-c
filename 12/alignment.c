#include <stdio.h>
#include <inttypes.h>
#include <complex.h>
#include <stdalign.h>

typedef complex double cdbl;

int main(void) {
    /* An overlay of complex values and bytes. */
    union {
        cdbl val[2];
        unsigned char buf[sizeof(cdbl[2])];
    } toocomplex = {
        .val = { 0.5 + 0.5*I, 0.75 + 0.75*I, },
    };

    printf("size/alignment: %zu/%zu\n",
            sizeof(cdbl), alignof(cdbl));

    /* Run over all offests, and crash on misalignment. */
    for (size_t offset = sizeof(cdbl); offset; offset /= 2) {
        printf("offset\t%zu\t", offset);
        fflush(stdout);
        cdbl* bp = (cdbl*)(&toocomplex.buf[offset]); // align
        printf("%g\t+%gI\t", creal(*bp), cimag(*bp));
        fflush(stdout);
        *bp  *= *bp;
        printf("%g\t+%gI\t", creal(*bp), cimag(*bp));
        fputc('\n', stdout);
    }
}
