#include "graph.h"

int main() {

  Graph g;
  map<int,int> vertex_indices = map<int, int>();
  map<int,int> vertex_vals = map<int, int>();

  int i = 0, j;
  for (; i < 1000; i++) {
    for (j = 0; j < 1000; j++) {
      g.add_edge(i, j, 0);
    }
  }

  return 0;
}

