#ifndef ISOMORPHISM_H
#define ISOMORPHISM_H
#include <vector>
#include <tuple>
#include "graph.h"

vector < pair<int,int> > *find_isomorphism (Graph &sub, Graph &graph);
vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph);
bool refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments);

#endif
