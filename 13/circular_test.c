#include "circular.h"
#include <stdio.h>
#include <assert.h>

static void print_circular(circular* c) {
    for (size_t i = 0; i < c->max_len; ++i) {
            printf("%f, ", c->tab[i]);
    }
    printf("\n");
}

int main(void) {
    circular* c = circular_new(6);
    circular_append(c, 1);
    circular_append(c, 1);
    circular_append(c, 2);
    circular_append(c, 3);
    circular_pop(c);

    circular_resize(c, 3);

    assert(c->start == 1);
    assert(c->max_len == 3);
    assert(*circular_element(c, 0) == 1);
    assert(*circular_element(c, 1) == 2);
    assert(*circular_element(c, 2) == 3);
    print_circular(c);

    circular* d = circular_new(6);
    circular_append(d, 1);
    circular_append(d, 1);
    circular_append(d, 1);
    circular_append(d, 1);
    circular_append(d, 1);
    circular_append(d, 2);
    circular_pop(d);
    circular_append(d, 3);

    circular_resize(d, 3);

    assert(c->start == 1);
    assert(c->max_len == 3);
    assert(*circular_element(c, 0) == 1);
    assert(*circular_element(c, 1) == 2);
    assert(*circular_element(c, 2) == 3);
    print_circular(c);
}

