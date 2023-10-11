#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef union unsignedInspect unsignedInspect;
union unsignedInspect {
    unsigned val;
    unsigned char bytes[sizeof(unsigned)];
};

int main(void) {
    unsignedInspect twofold = { .val = 0xAABBCCDD };

    printf("value is 0x%.08X\n", twofold.val);
    for (size_t i = 0; i < sizeof twofold.bytes; ++i) {
        printf("byte[%zu]: 0x%.02hhX\n", i, twofold.bytes[i]);
    }
    return EXIT_SUCCESS;
}
