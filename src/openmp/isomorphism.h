#ifndef ISOMORPHISM_H
#define ISOMORPHISM_H
#include <vector>
#include <tuple>
#include "graph.h"

typedef struct search_tree {
  // assignments_tree[i] specifies the possible_assignments at depth i
  vector <vector <vector <bool> > > assignments_tree;
  // matrix M
  vector <vector <bool> > possible_assignments;
  // columns_used[i] = true iff column i in M has been used at current stage of computation
  vector <bool> columns_used;
  // column_depth[d] = k if column k was used at depth d
  vector <int> column_depth;
  // column currently being used
  ssize_t k;
  // rows in M
  ssize_t pa_n;
  // columns in M
  ssize_t pb_n;
  // current depth in the tree
  int depth;
} search_tree;

vector < pair<int,int> > *find_isomorphism (Graph &sub, Graph &graph);
vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph);
bool refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments);

#endif
