#ifndef ISOMORPHISM_H
#define ISOMORPHISM_H
#include <vector>
#include <tuple>
#include "graph.h"

bool search(Graph *graph, Graph *subgraph, vector < pair<int,int> > *assignments, vector < pair<int,int> > *possible_assignments);
int find_isomorphism(Graph *graph, Graph *subgraph);
void update_possible_assignments(Graph *graph, Graph *subgraph, vector < pair<int,int> > *possible_assignments);

#endif
