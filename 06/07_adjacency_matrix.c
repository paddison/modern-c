#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void print_array(size_t N, bool A[N]) {
    for (size_t i = 0; i < N; ++i) {
        printf("%u ", A[i]);
    }
    printf("\n");
}

void print_matrix(size_t M, size_t N, bool A[M][N]) {
    for (size_t r = 0; r < M; ++r) {
        print_array(N, A[r]);
    }
}

bool add_edge(size_t tail, size_t head, size_t N, bool G[N][N]) {
  if (G[tail][head]) {
    return false;
  }
  G[tail][head] = true;
  return true;
}



size_t bfs(size_t N, size_t start, size_t goal, bool G[6][6]) {
  bool seen[N];
  size_t queue[N];
  size_t cur = 0;
  size_t size = 1;
  for (size_t i = 0; i < N; ++i) {
    seen[i] = false;
    queue[i] = 0;
  }

  // add start to queue and seen
  seen[start] = true;
  queue[cur] = start;

  while (cur < size) {
    size_t v = queue[cur];
    if (v == goal) {
      return goal;
    }
    // traverse the graph in direction of edges
    for (size_t n = 0; n < N; ++n) {
      if (!seen[n] && G[v][n]) {
        seen[n] = true;
        queue[size++] = n;
      }
    }
    ++cur;
  }

  return SIZE_MAX;
}

void dfs_spanning_tree(size_t N, size_t cur, bool G[N][N], bool T[N][N], bool seen[N]) {
  if (seen[cur]) {
    return;
  }

  // mark as visited
  seen[cur] = true;

  for (size_t n = 0; n < N; ++n) {
    // outgoing edges
    if (!seen[n] && G[cur][n]) {
      // add neighbor to tree
      T[cur][n] = true;
      dfs_spanning_tree(N, n, G, T, seen);
    }
    // incoming edges
    if (!seen[n] && G[n][cur]) {
      T[n][cur] = true;
      dfs_spanning_tree(N, n, G, T, seen);
    }
  }
}

void dfs_components(size_t N, size_t v, bool G[N][N], size_t C[N], size_t n_components) {
  // check if v already belongs to a component
  if (C[v]) {
    return;
  }

  // add v to current component
  C[v] = n_components;

  for (size_t n = 0; n < N; ++n) {
    // outgoing edges
    if (!C[n] && G[v][n]) {
      dfs_components(N, n, G, C, n_components);
    }
    // incoming edges
    if (!C[n] && G[n][v]) {
      dfs_components(N, n, G, C, n_components);
    }
  }
}

void connected_components(size_t N, bool G[N][N], size_t C[N]) {
  size_t n_components = 1;
  for (size_t v = 0; v < N; ++v) {
    if (!C[v]) {
      dfs_components(N, v, G, C, n_components);
      ++n_components;
    }
  }
}

void spanning_tree(size_t N, size_t start, bool G[N][N], bool T[N][N]) {

  bool seen[N];

  for(size_t i = 0; i < N; ++i) {
    seen[i] = false;
  }
  dfs_spanning_tree(N, start, G, T, seen);
}

int main(void) {
  // build the graph
  bool G[6][6] = {{ 0 }};

  add_edge(0, 1, 6, G);
  add_edge(0, 2, 6, G);
  add_edge(2, 3, 6, G);
  add_edge(3, 2, 6, G);
  add_edge(4, 5, 6, G);
  add_edge(5, 4, 6, G);

  printf("graph:\n");
  print_matrix(6, 6, G);

  // initialize the spanning tree
  bool T[6][6] = {{ 0 }};

  size_t found = bfs(6, 5, 4, G);
  printf("found: %lu\n", found);

  size_t C[6] = { 0 };

  connected_components(6, G, C);

  printf("connected components:\n");
  for (size_t i = 0; i < 6; ++i) {
    printf("%lu, ", C[i]);
  }
  printf("\n");

  add_edge(0, 4, 6, G);
  add_edge(2, 4, 6, G);
  spanning_tree(6, 0, G, T);

  printf("\ntree: \n");
  print_matrix(6, 6, T);

  return EXIT_SUCCESS;
}
