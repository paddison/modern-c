#include <stdio.h>

int main(void) {
    // since for unsigned, all arithmetic are well defined in the ring (meaning overflow)
    // this is a valid statement:
    for (unsigned i = 1; i; ++i) {
        if (!(i % 1000000000)) {
            printf("i: %u\n", i);
        }
    }
    printf("finished loop 1");
    // in contrast, overflow for signed values is undefined:
    // when the compiler sees this, it might assume that since i is starts at one
    // and is only incremented, it will never be zero
    // in contrast to the unsigned version, where i will wrap around because of the modulo definition
    // in theory, the compiler might thus optimize this loop to
    // while (true) { do_something(); } 
    for (signed i = 1; i; ++i) {
        if (!(i % 1000000000) || i < 0) {
            printf("i: %d\n", i);
        }
    }
}