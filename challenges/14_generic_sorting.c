#include <stdio.h>
#include <stdlib.h>

void copy_into(void* a, void* b, const size_t size) {
    char* target = (char *) a;
    char* source = (char *) b;

    for (size_t i = 0; i < size; ++i) {
        target[i] = source[i];
    }
}

void merge(void* B, void* A, size_t start, size_t middle, size_t end, size_t size, int cmp(void*, void*)) {
    size_t l = start;
    size_t r = middle;

    for (size_t i = start; i < end; ++i) {
        if (r == end || (l != middle && cmp(A + (r * size), A + (l * size)) > 0)) {
            copy_into(B + i * size, A + l * size, size);
            ++l;
        } else {
            copy_into(B + i * size, A + r * size, size);
            ++r;
        }
    }
}

void split(void* B, void* A, size_t start, size_t end, size_t size, int cmp(void*, void*)) {
    if (end - start <= 1) {
        return;
    }

    size_t middle = (end + start) / 2;
    split(A, B, start, middle, size, cmp);
    split(A, B, middle, end, size, cmp);

    merge(B, A, start, middle, end, size, cmp);
}

void sort(size_t n_items, void* A, size_t size, int cmp(void*, void*)) {
    void* B = malloc(n_items * size);

    for (size_t i = 0; i < n_items; ++i) {
        copy_into(B + i * size, A + i * size, size);
    }

    split(A, B, 0, n_items, size, cmp);
}

int cmp_int(void* a, void* b) {
    int* c = (int*) a;
    int* d = (int*) b;
    return *(c) - *(d);
}

int main(void) {
    int A[10] = { 5, 2, 1, 3, 7, 2, 9, 17, 15, -1 }; // there is one missing
    sort(10, A, sizeof(int), cmp_int);

    for(size_t i = 0; i < 10; ++i) {
        printf("%d, ", A[i]);
    }
    printf("\n");
}
