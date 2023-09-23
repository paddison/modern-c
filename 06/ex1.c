#include <string.h>
#include <stdio.h>

int strcmp_bounds(char const s0[static 1], char const s1[static 1], size_t len_0, size_t len_1) {
    size_t min =  (len_0 > len_1) ? len_1 : len_0;

    return memcmp(s0, s1, min);
}
