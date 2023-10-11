#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *p;

    /*
     * malloc() always provides aligned memory.
     * Do not use stack variable like a[9], depending on the compiler you use,
     * a may not be aligned properly.
     */
    p = malloc(sizeof(int) + 1);
    memset(p, 0, sizeof(int) + 1);

    /* making p unaligned */
    p++;

    printf("%d\n", *(int *)p);

    return EXIT_SUCCESS;
}
