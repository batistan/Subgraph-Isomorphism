#ifndef ISOMORPHISM_H
#define ISOMORPHISM_H
#include <vector>
#include <tuple>
#include "graph.h"

bool search(Graph *graph, Graph *subgraph, vector < pair<int,bool> > *assignments, vector < vector<bool> > *possible_assignments);
int find_isomorphism(Graph *graph, Graph *subgraph);
void update_possible_assignments(Graph *graph, Graph *subgraph, vector < vector<bool> > *possible_assignments);

#endif
