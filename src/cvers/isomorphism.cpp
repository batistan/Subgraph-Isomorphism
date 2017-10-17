#include <vector>
#include <algorithm>
#include <tuple>
#include "graph.h"

bool search(Graph *graph, Graph *subgraph, vector < pair<int,int> > *assignments, vector < pair<int,int> > *possible_assignments) {

  // Uses DFS to find instance of subgraph within larger graph

  update_possible_assignments(graph, subgraph, possible_assignments)

  i = assignments->size();

  // If all the vertices in the subgraph are assigned, then we are done.
  if (i == subgraph->vertices.size()) {
    return true;
  }

  // Make sure that every edge between assigned vertices in the subgraph is also an
  // edge in the graph.

  vector < tuple<int, int, int> >::iterator edge = subgraph->edges.begin();

  // this loop calls graph.has_edge once for every edge in the subgraph
  for ( ; edge != subgraph->edges.end(); edge++) {
    if ((*edge)[0] < i && (*edge)[1] < i) {
      if (!graph->has_edge(assignments[edge[0]], assignments[edge[1]])) {
        return false;
      }
    }
  }

  vector< pair<int,int> >::iterator j = possible_assignments->begin();

  for ( ; j != possible_assignments.end(); j++) {
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

// TODO update_possible_assignments and find_isomorphism
