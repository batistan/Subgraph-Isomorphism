#ifndef ISOMORPHISM_H
#define ISOMORPHISM_H
#include <vector>
#include <tuple>
#include "graph.h"

vector < int > find_isomorphism (Graph &sub, Graph &graph);

bool search (Graph &sub, Graph &graph, vector<vector<vector<bool> > > &assignments_tree, 
    vector<vector<bool> > &possible_assignments, vector<bool> &cols_used, 
    vector<size_t> &col_depth, size_t &depth);

vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph);

bool refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments);

#endif
