#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <vector>
#include <tuple>
#include <map>

using std::vector;
using std::tuple;
using std::unordered_set;
using std::pair;
using std::map;

// class Vertex{
// public:
//   Vertex (int label, bool visited);
//   int label;
//   bool visited;
// };

class Graph{
private:
  // DO NOT TOUCH THESE DIRECTLY
  // IF YOU NEED SOMETHING FROM THEM USE A HANDY FUNCTION

  // in all functions, use vertex values as parameters
  // except get_value, obviously
  // adjacencies contains neighbors list of all vertices
  // don't ask about the name
  vector < unordered_set<int> > adjacencies;
  // maps vertex value to its index in vertices
  // vertex_indices[value] => index
  map <int, int> vertex_indices;
  // maps vertex index to value
  // vertex_vals[index] => value
  map <int, int> vertex_vals;
  // adds a neighbor to vertex
  void add_neighbor (int vert, int neighbor);
  void update_adjacencies();

public:

  // vertex is pair of ints
  // vert.first is its value, vert.second is its degree
  vector < pair<int, int> > vertices;
  vector < vector<int> > edges;
  // DON'T USE THIS CONSTRUCTOR ANYMORE 
  // DEPRECATED
//  Graph (vector< pair<int, int> > vertices, vector<tuple<int, int, int> > edges, vector<unordered_set<int> > adjacencies);
  // USE THIS ONE
  Graph();

  // gets called mostly from within add_edge
  // but let's leave it here since a vertex might not have any neighbors
  void add_vertex(pair<int, int> vertex); 

  void add_edge(int source, int dest, int weight);

  bool has_edge(int vert1, int vert2);

  int get_index(int value);

  int get_value(int index);

  // get set of neighboring vertices of vertex with value vertex
  unordered_set<int> neighbors(int vertex);

  void print_graph(); //For debugging

};
#endif
