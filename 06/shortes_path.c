#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

size_t add_edge(size_t N, size_t t, size_t h, size_t w, size_t G[N][N]) {
    // always override edges when adding them, returning old weight
    if (t >= N || h >= N) {
        printf("head or tail not in graph");
    }

    size_t last = G[t][h];
    G[t][h] = w;
    return last;
}

size_t dijkstra(size_t N, size_t start, size_t goal, size_t G[N][N], size_t P[N]) {
    // tentative distances
    size_t D[N]; 
    size_t Q[N];
    size_t front = 0;
    size_t back = 1;

    for (size_t i = 0; i < N; ++i) {
        D[i] = SIZE_MAX;
    }

    // add start to queue
    Q[0] = start;
    D[start] = 0;

    while (front < back) {
        size_t v = Q[front]; 
        if (v == goal) {
            return D[v];
        }
        for (size_t n = 0; n < N; ++n) {
            // if n is reachable by v  
            size_t e = G[v][n];
            if (e == SIZE_MAX) {
                continue;
            }
            // if the path is shorter than the one previously discovered
            if (D[n] > D[v] + e) {
                // update distance
                D[n] = D[v] + e;
                // update path
                P[n] = v;
                // add to queue
                Q[back++] = n;
            }
        }
        ++front;
    }    

    return SIZE_MAX;
}

void print_path(size_t N, size_t goal, size_t P[N]) {
    size_t v = goal;
    if (P[v] == v) {
        printf("%lu\n", v);
        return;
    }
    printf("%lu->", v);
    print_path(N, P[v], P);
}

int main(void) {
    size_t const N = 6;
    size_t G[N][N] = {{ 0 }};
    size_t P[N] = { 0 };

    for (size_t i = 0; i < N; ++i) {
        P[i] = i;
        for (size_t j = 0; j < N; ++j) {
            G[i][j] = SIZE_MAX;
        }
    }

    add_edge(N, 0, 1, 2, G);
    add_edge(N, 0, 2, 3, G);
    add_edge(N, 0, 4, 1, G);
    add_edge(N, 2, 3, 1, G);
    add_edge(N, 3, 5, 2, G);
    add_edge(N, 4, 2, 2, G);
    add_edge(N, 4, 5, 1, G);

    

    size_t cost = dijkstra(N, 0, 5, G, P);
    printf("cost: %lu\n", cost);
    print_path(N, 5, P);
    return EXIT_SUCCESS;
}
