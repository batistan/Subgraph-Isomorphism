#include <vector>
#include <algorithm>
#include <tuple>
#include "graph.h"
#include "isomorphism.h"

using std::get;

bool search(Graph *graph, Graph *subgraph, vector < pair<int,int> > *assignments, vector < pair<int,int> > *possible_assignments) {

  // Uses DFS to find instance of subgraph within larger graph

  update_possible_assignments(graph, subgraph, possible_assignments);

  int i = assignments->size();

  // If all the vertices in the subgraph are assigned, then we are done.
  if (i == subgraph->vertices.size()) {
    return true;
  }

  // Make sure that every edge between assigned vertices in the subgraph is also an
  // edge in the graph.

  vector < tuple<int, int, int> >::iterator edge = subgraph->edges.begin();

  // this loop calls graph.has_edge once for every edge in the subgraph
  for ( ; edge != subgraph->edges.end(); edge++) {
    if (get<0>(*edge) < i && get<1>(*edge) < i) {
      if (!(graph->has_edge(assignments[get<0>(*edge)]).first, assignments[get<1>(*edge)].first)) {
        return false;
      }
    }
  }

  vector< pair<int,int> >::iterator j = possible_assignments->begin();

  for ( ; j != possible_assignments->end(); j++) {
    if (find(possible_assignments->begin(), possible_assignments->end(), *j) == possible_assignments->end()) {
      assignments->push_back(*j);
      // TODO hope this is a deep copy.
      vector< pair<int,int> > new_possible_assignments = *possible_assignments;
      new_possible_assignments.push_back(*j);
      if (search(graph, subgraph, assignments, new_possible_assignments)) {
        return true;
      }

      assignments->pop_back();
    }

      // TODO translate this line because wtf is this doing
      // this shouldn't even work in python
      //possible_assignments[i].remove(j)
      //(*possible_assignments)[i].remove(*j);
      update_possible_assignments(graph, subgraph, possible_assignments);
  }
}

int find_isomorphism(Graph *graph, Graph *subgraph) {

  vector < pair<int,int> > *assignments;
  vector < pair<int,int> > *possible_assignments;

  int matches = 0;

  if (search(graph, subgraph, assignments, possible_assignments)) {
    printf("Match found\n");
    matches++;
    vector< pair<int,int> >::iterator node = assignments->begin();
    for(; node != assignments->end(); node++) {
      pair<int, bool> vertex(node[0], false);
      graph->remove_vertex(vertex);
    }
  }

  return matches;
}

void update_possible_assignments(Graph *graph, Graph *subgraph, vector < pair<int,int> > *possible_assignments) {

  bool any_changes = true;
  bool match;
  while (any_changes) {
    any_changes = false;
    int i = 0;
    for (; i < subgraph->vertices.size(); i++) {
      vector< pair<int,int> >::iterator j = possible_assignments[i].begin();
      for (; j != possible_assignments[i].end(); j++) {
        vector < unordered_set<int> >::iterator adj;
        for (adj = subgraph->adjacencies.begin(); adj != subgraph->adjacencies.end(); adj++) {
          match = false;
          vector < pair<int, bool> >::iterator vert;
          for (vert = graph->vertices.begin(); vert != graph->vertices.end(); vert++) {
            if (find(possible_assignments->begin(), possible_assignments->end(), *adj) == possible_assignments->end()) {
              if (graph->has_edge((*j).first, vert->label)) {
                match = true;
              }
            }

            if (!match) {
              possible_assignments[i].remove(*j);
              any_changes = true;
            }
          }
        }
      }
    }
  }
}
