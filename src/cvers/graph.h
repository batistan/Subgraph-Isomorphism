#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <vector>
#include <tuple>
using std::vector;
using std::tuple;
using std::unordered_set;
using std::pair;

class Vertex{
public:
  Vertex (int label, bool visited);
  int label;
  bool visited;
};

class Graph{
public:
  vector < Vertex > vertices;
  vector < tuple<int, int, int> > edges;
  vector < unordered_set<int> > adjacencies;

  Graph (vector< Vertex > vertices, vector<tuple<int, int, int> > edges, vector<unordered_set<int> > adjacencies);
  void add_vertex(Vertex vertex); //Vertices are currently represented by pairs.
  void remove_vertex(Vertex vertex);

  void add_edge(int source, int dest, int weight);

  bool has_edge(int vert1, int vert2);

  void print_graph(); //Debugging

private:
  void update_adjacencies();
};
#endif
