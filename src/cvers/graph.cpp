#include <algorithm> //Replace, remove, find
#include <iostream>
#include "graph.h"

using std::remove;
using std::find;
using std::get;
using std::cout;

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

Graph::Graph (vector< pair<int, bool>  > vertices, vector<tuple<int, int, int> > edges,
  vector< unordered_set<int> > adjacencies){

  this->vertices = vertices;
  this->edges = edges;
  this->adjacencies = adjacencies;
  update_adjacencies();
}

void Graph::add_vertex(pair<int, bool>  vertex){
  vertices.push_back(vertex);
}

void Graph::remove_vertex(pair<int, bool>  vertex){
  vertices.erase(remove(vertices.begin(), vertices.end(), vertex), vertices.end());
  vector<int> remove_these;

  size_t i;
  for (i = 0; i < edges.size(); i++){
      if (get<0>(edges[i]) == vertex.first || get<1>(edges[i]) == vertex.first){
        vertices.erase(vertices.begin() + i);
        i--; //edges will be one element smaller, this index will be different now
      }
  }

  for (i = 0; i < adjacencies.size(); i++){
    if ((adjacencies[i]).find(vertex.first) != (adjacencies[i]).end()){
      adjacencies.erase(adjacencies.begin() + i);
      i--; //ditto
    }
  }
}

void Graph::add_edge(int source, int dest, int weight){
  if (find_if(vertices.begin(),vertices.end(), CompareFirst(source)) == vertices.end()) {
    pair<int, bool>  vsrc(source, false);
    vertices.push_back(vsrc);
  }

  if (find_if(vertices.begin(),vertices.end(), CompareFirst(dest)) == vertices.end()) {
    pair<int, bool> vdest(dest, false);
    vertices.push_back(vdest);
  }

  tuple<int, int, int> new_edge (source, dest, weight);
  edges.push_back(new_edge);

  unordered_set<int> new_adjacency ( {source, dest} );
  adjacencies.push_back(new_adjacency);
}

bool Graph::has_edge(int vert1, int vert2){
  unordered_set<int> edge ( {vert1, vert2} );
  if (find(adjacencies.begin(), adjacencies.end(), edge) == adjacencies.end())
    return false;

  return true;
}

void Graph::update_adjacencies() {
  for (size_t i = 0; i < edges.size(); i++){
    unordered_set<int> edge ({get<0>(edges[i]), get<1>(edges[i])});
    adjacencies.push_back(edge);
  }
}

// void Graph::print_graph() {
//   cout << "Begin graph" << std::endl;
//   cout << "-------------------------------\n";
//   cout << "Vertices: " << vertices << std::endl;
//   cout << "Edges: " << edges << std::endl;
//   cout << "-------------------------------\n";
//   cout << "End graph" << std::endl;
// }
