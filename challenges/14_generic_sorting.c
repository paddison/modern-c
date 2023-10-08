#include <stdio.h>
#include <stdlib.h>

typedef int cmp_f(void*, void*);

void copy_into(void* a, void* b, const size_t size) {
    char* target = (char *) a;
    char* source = (char *) b;

    for (size_t i = 0; i < size; ++i) {
        target[i] = source[i];
    }
}

void merge(void* B, void* A, size_t start, size_t middle, size_t end, size_t size, cmp_f* cmp) {
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

void split(void* B, void* A, size_t start, size_t end, size_t size, cmp_f* cmp) {
    if (end - start <= 1) {
        return;
    }

    size_t middle = (end + start) / 2;
    split(A, B, start, middle, size, cmp);
    split(A, B, middle, end, size, cmp);

    merge(B, A, start, middle, end, size, cmp);
}

void m_sort(size_t n_items, void* A, size_t size, cmp_f* cmp) {
    void* B = malloc(n_items * size);

    for (size_t i = 0; i < n_items; ++i) {
        copy_into(B + i * size, A + i * size, size);
    }

    split(A, B, 0, n_items, size, cmp);
}

void swap(void* A, size_t i, size_t j, size_t size) {
    if (i == j) return;
    char* a = (char*) A;
    for (size_t k = 0; k < size; ++k) {
        char tmp = a[i * size + k];
        a[i * size + k] = a[j * size + k];
        a[j * size + k] = tmp;
    }
}

int partition(void* A, int lo, int hi, size_t size, cmp_f* cmp) { 
  void* pivot = A + hi * size; 

  int i = lo - 1;

  for (int j = lo; j < hi; ++j) { 
    if (cmp(A + j * size, pivot) <= 0) { 
      i = i + 1;
      swap(A, i, j, size);
    }
  }

    i = i + 1;
    swap(A, i, hi, size);
    return i;
}

void q_sort(void* A, int lo, int hi, size_t size, cmp_f* cmp) { 
  if (lo >= hi || lo < 0)  
    return;
    
  int p = partition(A, lo, hi, size, cmp);
      
  q_sort(A, lo, p - 1, size, cmp); 
  q_sort(A, p + 1, hi, size, cmp); 
}

int cmp_int(void* a, void* b) {
    int* c = (int*) a;
    int* d = (int*) b;
    return *(c) - *(d);
}

int main(void) {
    int A[10] = { 5, 2, 1, 3, 7, -1, 9, 17, 15, 8 }; // there is one missing
    q_sort(A, 0, 10 - 1, sizeof(int), cmp_int);

    for(size_t i = 0; i < 10; ++i) {
        printf("%d, ", A[i]);
    }
    printf("\n");
}
