#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void print_array(size_t N, double A[N]) {
    for (size_t i = 0; i < N; ++i) {
        printf("%f ", A[i]);
    }
    printf("\n");
}

void print_matrix(size_t M, size_t N, double A[M][N]) {
    for (size_t r = 0; r < M; ++r) {
        print_array(N, A[r]);
    }
}

double dot(size_t N, double u[static N], size_t M, double v[static M]) {
    if (N != M) {
        return NAN;
    }

    double sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += u[i] * v[i];
    }

    return sum;
}

void matrix_vector_product(size_t N, size_t M, double m[static M][N], double v[static N], double out[static M]) {

    for (size_t i = 0; i < M; ++i) {
        out[i] = dot(N, m[i], N, v);
    }
}

double mag(size_t N, double u[static N]) {
    double squared_sum = 0;
    for (size_t i = 0; i < N; ++i) {
        squared_sum += u[i] * u[i];
    }
    return sqrt(squared_sum);
}

int is_echelon(double m[3][4]) {
    for (size_t r = 1; r < 3; ++r) {
        for (size_t c = 0; c < r; ++c) {
            if (m[r][c] != 0) {
                return 0;
            }
        }
        
    }
    return 1;
}

void add_rows(double scalar, size_t N, double u[N], double v[N]) {
    for (size_t c = 0; c < N; ++c) {
        u[c] += scalar * v[c];
    }
}
 
int gauss_elimination(size_t M, size_t N, double m[M][N]) {
    if (N - M != 1) {
        return 0;
    }

    // build echelon form
    for (size_t i = 1; i < M; ++i) {
        // find multiplicant
        for (size_t r = i; r < M; ++r) {
            double x = m[r][i - 1] / -m[i - 1][i - 1];
            // add rows
            add_rows(x, N, m[r], m[i - 1]);
        }
    }
    // check that it is in upper triangular form
    if (!is_echelon(m)) {
        return 0;
    }

    // build entity matrix
    for (size_t i = 1; i < M; ++i) {
        for (size_t r = 0; r < i; ++r) {
            double x = m[r][i] / -m[i][i];
            add_rows(x, N, m[r], m[i]);
        }
    }

    // normalize 
    for (size_t r = 0; r < N; ++r) {
        double x = 1 / m[r][r];
        m[r][r] *= x;
        m[r][N - 1] *= x;
     }

    return 1;
}

int main(void) {
    const size_t M = 2;
    const size_t N = 3;
    double m[M][N] = {
        { 1, 2, 3 },
        { 3, 4, 5 },
    };

    double v[N] = { 5, 2, 1 };
    double result[M] = { 0 };
    matrix_vector_product(3, 2, m, v, result);
    for (size_t i = 0; i < M; ++i) {
        printf("%lu: %f\n", i, result[i]); 
    }

    double A[3][4] = {
        { 2, 1, -1, 8 },
        { -3, -1, 2, -11 },
        { -2, 1, 2, -3 },
    };

    gauss_elimination(3, 4, A);
    print_matrix(3, 4, A);
    return EXIT_SUCCESS;
}
