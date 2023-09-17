#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

int main(void) {
    // just printing out limits:
    printf("short: %d\n", SHRT_MAX);
    printf("unsigned: %u\n", UINT_MAX);
    printf("unsigned long: %lu\n", ULONG_MAX);
    printf("unsigned long long: %llu\n", ULLONG_MAX);
    printf("size_t %lu\n", SIZE_T_MAX);

    // numbers with fixed sizes are provided by <stdint.h>.
    uint64_t b64 = 0; // type definitions exits
    // if we want to use them as consts, there are also macros for this
    # define MY_U64 UINT64_C(23)

    // since we don't know the exact type of these, when using prints, 
    // there are also macros defined in <inttypes.h>
    printf("printing a unsigned 64 bit integer: %" PRIu64 "\n", MY_U64);
}