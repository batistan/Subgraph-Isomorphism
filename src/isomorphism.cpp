#include <vector>
#include <pair>
#include <unordered_set>
#include "graph.h"

vector < pair<int,int> > find_isomorphism (Graph &sub, Graph &graph) {

  vector <vector <bool> > possible_assignments = create_possible_assignments(sub, graph);
  vector <pair<int, int> > assignments = vector<pair<int, int> >(sub.vertices.size());

  // first, try to map first vertex of i to first possible assignment, removing that possibility from the rest of the vertices's assignments
  // then... do stuff

  ssize_t i, j, pa_n = possible_assignments.size();

  for (i = 0; i < pa_n; i++) {
    for (j = 0; j < pa_n; j++) {
      if (possible_assignments[i][j]) {
        refine_possible_assignments(sub, graph, possible_assignments);
      }
    }
  }

}

vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph) {
  // possible_assignments[i][j] == true iff a possible assignment exists from i in sub
  // to j in search graph
  vector < vector<bool> > possible_assignments = vector< vector<bool> >(sub.vertices.size(),
                                                 vector<bool>(false,sub.vertices.size()));

  // at first, every vertex in search graph with rank >= i is a possible assignments
  // this will be refined later
  ssize_t i, j;
  ssize_t s_n = sub.vertices.size();
  ssize_t g_n = graph.vertices.size();
  for (i = 0; i < s_n; i++) {
    for (j = 0; j < g_n; j++) {
      if (graph.vertices[j].second >= sub.vertices[i].second) {
        possible_assignments[i][j] = true;
      }
    }
  }

  return possible_assignments;
}

void refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments) {

  ssize_t i, j;
  ssize_t pa_n = possible_assignments.size();

  for (i = 0; i < pa_n; i++) {
    for (j = 0; j < pa_n && j_valid; j++) {
      if (possible_assignments[i][j]) {
        // check if all of i's neighbors have a possible assignment to a neighbor of j
        // iterate through all neighbors of i
        // TODO change this to whatever we're doing for neighbors now
        unordered_set<pair<int, int> > neighbors_i = sub.neighbors(sub.get_value(i));
        unordered_set<pair<int, int> >::iterator n_i = neighbors_i.begin();

        for (; n_i != neighbors_i.end() && j_valid; n_i++) {
          // for each neighbor of i, iterate through all its possible assignments
          ssize_t k;
          bool has_corresponding_neighbor = false;
          for (k = 0; k < pa_n; k++) {
            if (possible_assignments[sub.get_index(n_i)][k]) {
              // for each possible assignment from i's neighbor to graph, check if it is neighbor of j 
              if (graph.has_edge(graph.get_value(j), graph.get_value(k))) {
                // if so, then check the next neighbor
                has_corresponding_neighbor = true;
                break;
              }
            }
          }

          // if this neighbor has no corresponding neighbor, then j is an invalid match.
          // move on to the next possible assignment
          if (!has_corresponding_neighbor) {
            possible_assignments[i][j] = 0;
            break;
          }
        }
      }
    }
  }
}

