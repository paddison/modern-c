#include <stdio.h>

void fgoto(unsigned n) {
    unsigned j = 1;

    printf("%u: p and q are unequal, *p is %u\n",
           j,
           j - 1);
AGAIN:
    printf("%u: p and q are equal, *p is %u\n",
           j,
           j - 1);
    ++j;
    if (j <= n) goto AGAIN;
}
