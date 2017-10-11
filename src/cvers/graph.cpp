#include <graph.h>
#include <algorithm> //Replace, remove

using std::remove;
using std::get;

Graph::Graph (vertex< pair<int,int> vertices, vector<tuple> edges,
  vector< unordered_set<int> > adjacencies){

  this->vertices = vertices
  this->edges = edges;
  this->adjacencies = adjacencies;
  update_adjacencies();
}

void Graph::add_vertex(pair<int,int> vertex){
  vertices.push_back(vertex);
}

void Graph::remove_vertex(pair<int, int> vertex){
  vertices.erase(remove(vertices.begin(), vertices.end(), vertex), vertices.end());
  vector<int> remove_these;

  size_t i;
  for (i = 0; i < edges.size(); i++){
      if (get<0>(edges[i]) == vertex || get<1>(edges[i]) == vertex){
        vertices.erase(vertices.begin() + i);
        i--; //edges will be one element smaller, this index will be different now
      }
  }

  for (i = 0; i < adjacencies.size(); i++){
    if (adjacencies[i].find(vertex) != adjacencies.end()){
      adjacencies.erase(adjacencies.begin() + i);
      i--; //ditto
    }
  }
}

void Graph::add_edge(int source, int dest, int weight){
  if ((vertices.begin(), vertices.end(), source) == vertices.end())
    vertices.push_back(source);

  if ((vertices.begin(), vertices.end(), dest) == vertices.end())
    vertices.push_back(dest);


  tuple<int, int, int> new_edge (source, dest, weight);
  edges.push_back(new_edge);

  unordered_set<int> new_adjacency ( {vert1, vert2} );
  adjacencies.push_back(new_adjacency);
}

bool has_edge(int vert1, int vert2){
  unordered_set<int> edge ( {vert1, vert2} );
  if (find(adjacencies.begin(), adjacencies.end(), edge) == adjacencies.end())
    return 0;

  else
    return 1;
}

void Graph::update_adjacencies(){
  for (size_t i = 0; i < edges.size(); i++){
    unordered_set<int> edge ({get<0>(edges[i]), get<1>(edges[i])});
    edges.push_back(edge);
  }
}
