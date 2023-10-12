#include <stdio.h>
#include <stdlib.h>


int main(void) {
    unsigned i = 1;     // A new object 
    if (i) {
        extern unsigned i;
        printf("%u\n", i);
    } else {
        printf("%u\n", i);
    }

    return EXIT_SUCCESS;
}
