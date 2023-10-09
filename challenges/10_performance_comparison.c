#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void merge_sort(size_t size, int A[size]) {
    int B[size];
    for (size_t i = 0; i < size; ++i) {
        B[i] = A[i];
    }
    split(A, B, 0, size);
}

void swap(int* A, size_t i, size_t j) {
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

// this is just copied from wikipedia, because there was some error in 
// my implementation and i was too lazy to look for it...

// Divides array into two partitions
int partition(int* A, int lo, int hi) { 
  int pivot = A[hi]; // Choose the last element as the pivot

  // Temporary pivot index
  int i = lo - 1;

  for (int j = lo; j < hi; ++j) { 
    // If the current element is less than or equal to the pivot
    if (A[j] <= pivot) { 
      // Move the temporary pivot index forward
      i = i + 1;
      // Swap the current element with the element at the temporary pivot index
      swap(A, i, j);
    }
  }

  // Move the pivot element to the correct pivot position (between the smaller and larger elements)
    i = i + 1;
    swap(A, i, hi);
    return i; // the pivot index
}

// Sorts a (portion of an) array, divides it into partitions, then sorts those
void quicksort(int* A, int lo, int hi) { 
  // Ensure indices are in correct order
  if (lo >= hi || lo < 0)  
    return;
    
  // Partition array and get the pivot index
  int p = partition(A, lo, hi);
      
  // Sort the two partitions
  quicksort(A, lo, p - 1); // Left side of pivot
  quicksort(A, p + 1, hi); // Right side of pivot
}

int cmp_int(const void* a, const void* b) {
    return *((int* )a) - *((int*) b);
}

int main(int argc, char* argv[argc+1]) {
    // create arrays with the specified amount of data:
    for (size_t i = 1; i < argc; ++i) {
        unsigned long n = strtoul(argv[i], 0, 0);
        int Q[n]; 
        int M[n];
        int I[n];
        struct timespec q_start;
        struct timespec q_end;
        struct timespec m_start;
        struct timespec m_end;
        struct timespec internal_start;
        struct timespec internal_end;


        // set the seed
        srand(time(0));

        // initialize A with random numbers
        for (size_t j = 0; j < n; ++j) {
            Q[j] = rand() % n;
        }

        // copy elements from Q to M
        for (size_t j = 0; j < n; ++j) {
            M[j] = Q[j];
        }

        // copy elements from Q to I
        for (size_t j = 0; j < n; ++j) {
            I[j] = Q[j];
        }


        // sort them and measure the time elapsed
        timespec_get(&q_start, TIME_UTC);
        quicksort(Q, 0, n - 1);
        timespec_get(&q_end, TIME_UTC);
        long q_diff = q_end.tv_nsec - q_start.tv_nsec;

        timespec_get(&m_start, TIME_UTC);
        merge_sort(n, M);
        timespec_get(&m_end, TIME_UTC);
        long m_diff = m_end.tv_nsec - m_start.tv_nsec;

        timespec_get(&internal_start, TIME_UTC);
        qsort(I, n, sizeof(int), cmp_int);
        timespec_get(&internal_end, TIME_UTC);
        long i_diff = internal_end.tv_nsec - internal_start.tv_nsec;

        // print the results
        printf("n: %lu\n", n);
        printf("Quick Sort: %lu us\n", q_diff / 1000);
        printf("Merge Sort: %lu us\n", m_diff / 1000);
        printf("    C Sort: %lu us\n", i_diff / 1000);
    }

    return EXIT_SUCCESS;
}
