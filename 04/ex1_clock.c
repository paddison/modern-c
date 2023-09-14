#include <stdlib.h>
#include <stdio.h>

size_t clock(size_t, size_t);

int main(void) {
    size_t time = 20;
    size_t add = 8;
    printf("%zu:00 + %zu hours is %zu:00\n", time, add, clock(time, add));
}

size_t clock(size_t time, size_t add) {
    return (time + add) % 24;
}