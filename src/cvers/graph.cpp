#include <algorithm> //Replace, remove, find
#include <iostream>
#include "graph.h"

using std::remove;
using std::find;
using std::get;
using std::cout;
using std::map;
using std::unordered_set;

struct CompareFirst
{
  CompareFirst(int val) : val_(val) {}
  bool operator()(const std::pair<int,int>& elem) const {
    return val_ == elem.first;
  }
  private:
    int val_;
};

// Vertex::Vertex(int label, bool visited) {
//   this->label = label;
//   this->visited = visited;
// }

// edges[i][j] = weight of edge connecting i and j
// 0 if no such edge
// TODO please never use this constructor.
//Graph::Graph (vector< pair<int, int>  > vertices, vector<vector<int> > edges,
//  vector< unordered_set<int> > adjacencies){
//
//  this->vertices = vertices;
//  this->edges = edges;
//  this->adjacencies = adjacencies;
//  update_adjacencies();
//}

Graph::Graph () {
  this->vertices = vector<pair<int, int> >();
  this->edges = vector<vector<int> >();
  this->adjacencies = vector<unordered_set<int> >();
  this->vertex_indices = map<int, int>();
  this->vertex_vals = map<int, int>();
}

void Graph::add_vertex(pair<int, int> vertex){
  vertices.push_back(vertex);
  // add mappings
  vertex_indices[vertex.first] = vertices.size()-1;
  vertex_vals[vertices.size()-1] = vertex.first;
  // add row with edges.size()+1 zeros
  edges.push_back(vector<int>(edges.size()+1,0));
  // add a set for its neighbors
  adjacencies.push_back(unordered_set<int>());
}

void Graph::add_edge(int source, int dest, int weight){
  // weight should be non-zero
  //if (find_if(vertices.begin(),vertices.end(), CompareFirst(source)) == vertices.end()) {
  //  pair<int, bool>  vsrc(source, false);
  //  vertices.push_back(vsrc);
  //}

  //if (find_if(vertices.begin(),vertices.end(), CompareFirst(dest)) == vertices.end()) {
  //  pair<int, bool> vdest(dest, false);
  //  vertices.push_back(vdest);
  //}

  //tuple<int, int, int> new_edge (source, dest, weight);
  //edges.push_back(new_edge);

  if (this->get_index(source) < 0) {
    this->add_vertex(pair<int, int>(source, 0));
  }

  if (get_index(dest) < 0) {
    this->add_vertex(pair<int, int>(source, 0));
  }
  // increase vertex degree
  vertices[this->get_index(source)].second++;
  vertices[this->get_index(dest)].second++;

  // source -> dest
  // we'd need a flag or something for directed graphs
  // somewhere
  edges[this->get_index(source)][this->get_index(dest)] = weight;
  edges[this->get_index(dest)][this->get_index(source)] = weight;
  // TODO: get adjacency set of source and dest (or create them if not in graph)
  // add dest/source to that set
  // once again, consider directed graphs
  this->add_neighbor(source, dest);
  this->add_neighbor(dest, source);
}

bool Graph::has_edge(int vert1, int vert2){
  //unordered_set<int> edge ( {vert1, vert2} );
  //if (find(adjacencies.begin(), adjacencies.end(), edge) == adjacencies.end())
  //  return false;

  //return true;
  // just got this bad boy down to O(1)
  // you're welcome
  return (edges[vert1][vert2] != 0);
}

int Graph::get_index(int value) {
  if (vertex_indices.find(value) != vertex_indices.end()) {
    return vertex_indices[value];
  }

  return -1;
}

int Graph::get_value(int index) {
  if (vertex_vals.find(index) != vertex_vals.end()) {
    return vertex_vals[index];
  }

  return -1;
}


unordered_set<int> Graph::neighbors(int vertex) {
  return this->adjacencies[this->get_index(vertex)];
}

void Graph::add_neighbor (int vert, int neighbor) {
  this->neighbors(vert).insert(this->vertices[this->get_index(neighbor)].first);
}

// basically obselete now
//void Graph::update_adjacencies() {
//  for (size_t i = 0; i < edges.size(); i++){
//    unordered_set<int> edge ({get<0>(edges[i]), get<1>(edges[i])});
//    adjacencies.push_back(edge);
//  }
//}

// void Graph::print_graph() {
//   cout << "Begin graph" << std::endl;
//   cout << "-------------------------------\n";
//   cout << "Vertices: " << vertices << std::endl;
//   cout << "Edges: " << edges << std::endl;
//   cout << "-------------------------------\n";
//   cout << "End graph" << std::endl;
// }
