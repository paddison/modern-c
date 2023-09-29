#include <stdlib.h>
#include <stdio.h>

size_t Find(size_t i, size_t size, size_t parent[static size]) {
    if (parent[i] != i) {
        return Find(parent[i], size, parent);
    }
    return i;
}

void FindReplace(size_t i, size_t value, size_t size, size_t parent[static size]) {
    for (size_t j = 0; j < size; ++j) {
        if (parent[j] == i) {
            parent[j] = value;
        }
    }
}

size_t FindCompress(size_t i, size_t size, size_t parent[static size]) {
    // change all parent entries to the root that has been found
    if (parent[i] != i) {
        parent[i] = FindCompress(parent[i], size, parent);
    }
    return parent[i];
}

void Union(size_t l, size_t r, size_t size, size_t parent[static size]) {
    FindReplace(r, FindCompress(l, size, parent), size, parent);
}

void print_array(size_t size, size_t A[static size]) {
    for (size_t i = 0; i < size; ++i) {
        printf("%zu ", A[i]);
    }
    printf("\n");
}

int main(void) {
    size_t SIZE = 10;
    size_t parent[SIZE];
    for (size_t i = 0; i < SIZE; ++i) {
        parent[i] = i;
    }

    Union(1, 2, SIZE, parent);
    print_array(SIZE, parent);
    Union(3, 4, SIZE, parent);
    print_array(SIZE, parent);
    Union(5, 6, SIZE, parent);
    print_array(SIZE, parent);
    Union(4, 5, SIZE, parent);
    print_array(SIZE, parent);
}
