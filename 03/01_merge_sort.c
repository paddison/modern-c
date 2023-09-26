#include <stdio.h>
#include <stdlib.h>

void merge(int* B, int* A, size_t start, size_t middle, size_t end) {
    size_t l = start;
    size_t r = middle;

    for (size_t i = start; i < end; ++i) {
        if (r == end || (l != middle && A[l] < A[r])) {
            B[i] = A[l];
            ++l;
        } else {
            B[i] = A[r];
            ++r;
        }
    }
}

void split(int* B, int* A, size_t start, size_t end) {
    if (end - start <= 1) {
        return;
    }

    size_t middle = (end + start) / 2;
    split(A, B, start, middle);
    split(A, B, middle, end);

    merge(B, A, start, middle, end);
}

void sort(size_t size, int A[size]) {
    int B[size];
    for (size_t i = 0; i < size; ++i) {
        B[i] = A[i];
    }
    split(A, B, 0, size);
}

void print_array(size_t size, int A[size]) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int is_sorted(size_t size, int A[size]) {
    for (size_t i = 1; i < size; ++i) {
        if (A[i - 1] > A[i]) {
            return 0;
        }
    }

    return 1;
}


int main(void) {
    int A[10] = { 5, 2, 1, 3, 7, 2, 9, 17, 15 }; // there is one missing
    sort(10, A);
    print_array(10, A);
    if (is_sorted(10, A)) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

