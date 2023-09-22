#include <stdio.h>
#include <stdlib.h>

int is_sorted(int*, size_t);

void swap(int* A, size_t i, size_t j) {
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

void print_array(int* A, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

void quick_sort(int* A, size_t start, size_t pivot) {
    if (pivot <= start) {
        return;
    }

    int l = start;
    int r = pivot;

    while (l < r) {
        if (A[l] > A[pivot]) {
            // swap element left of pivot with the larger one
            --r;
            swap(A, l, r);
        } else {
            ++l;
        }
    }

    swap(A, r, pivot);

    quick_sort(A, start, pivot - 1);
    quick_sort(A, pivot + 1, r);
}

void sort(int* A, size_t size) {
    quick_sort(A, 0, size - 1);
}


int main (void) {
    int A[10] = { 1, 5, 4, 3, 6, 4, 10, 20, 2, 7 };
    sort(A, 10);
    print_array(A, 10);
    if (is_sorted(A, 10)) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

int is_sorted(int* A, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        if (A[i - 1] > A[i]) {
            return 0;
        }
    }

    return 1;
}