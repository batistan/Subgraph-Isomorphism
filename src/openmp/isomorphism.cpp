#include <vector>
#include <utility>
#include <unordered_set>
#include <algorithm> //for std::find
#include "isomorphism.h"
#include "omp.h"

int THREADS = omp_get_max_threads();

using std::find;

vector <int> find_isomorphism (Graph &sub, Graph &graph) {

  vector < vector<bool> > possible_assignments = create_possible_assignments(sub, graph);
  vector <vector <vector <bool> > > assignments_tree;
  // we need a way to keep track of which columns we've already assigned for each row
  // cols_used[i] == true if column i has already been used
  vector <bool> cols_used = vector<bool>(possible_assignments[0].size(), false);
  // keep track of which column was used at which depth
  // col_depth[i-1] == k iff column k was used at depth i
  vector <size_t> col_depth = vector<size_t>(possible_assignments.size(), 0);

  size_t depth = 1;

  if (search(sub, graph, assignments_tree, possible_assignments, cols_used, col_depth, depth)) {

    vector<vector<bool> > assignments = assignments_tree[depth-2];
    vector<int> isomorphism;
    for (size_t i = 0; i < assignments.size(); i++) {
      //fprintf(stderr, "Row %lu\n", i);
      for (size_t j = 0; j < assignments[i].size(); j++) {
        //fprintf(stderr, "%d\n", assignments[i][j] ? 1 : 0);
        if (assignments[i][j]) {
          isomorphism.push_back(j);
          // go to next row
          // no need to waste iterations
          break;
        }
      }
    }

    return isomorphism;
  }

  return vector<int>(0);
}

bool search (Graph &sub, Graph &graph,
      vector<vector<vector<bool> > > &assignments_tree,
      vector<vector<bool> > &possible_assignments,
      vector<bool> &cols_used,
      vector<size_t> &col_depth,
      size_t &depth) {

  // if refine fails, no possible isomorphism
  if (!refine_possible_assignments(sub, graph, possible_assignments)) {
    return false;
  }

  do {
    size_t numrows = possible_assignments.size(), numcols = possible_assignments[0].size(), col;

    for (col = col_depth[depth-1]; col < numcols; col++) {
      // find first column we havent already used
      // col is index of vertex in search graph we're trying to assign
      // we start searching from col_depth[depth-1]+1 since
      // that's the col after the one we attempted previously and
      // we gotta find the next one
      if (possible_assignments[depth-1][col] && !cols_used[col]) {
        break;
      }
    }

    if (col == numcols) {
      // we couldn't find a valid column.
      // if we're on depth 1, there's no level to go back up to
      // no possible isomorphism
      if (depth == 1) {
        return false;
      }

      // otherwise, go up to the previous depth and try to assign that
      // row to another column
      else {
        depth--;
        possible_assignments = assignments_tree[depth-1];
        return false;
      }
    }

    // we found a valid column
    // set all other possible assignments in this col to 0
    // if we're assigning this vertex to the subgraph vertex
    // we can't have any other subgraph vertex also assigned to it
    size_t i;
    for (i = 0; i < numrows; i++) {
      if (i != depth-1) {
        possible_assignments[i][col] = false;
      }
    }

    // set all other 1s in this row to 0
    // we can only assign each subgraph vertex to a single column
    for (i = 0; i < numcols; i++) {
      if (i != col) {
        possible_assignments[depth-1][i] = false;
      }
    }

    // now go to the next row and repeat
    // we used this column
    cols_used[col] = true;
    // and we used it at this depth
    col_depth[depth-1] = col;
    // save a copy of possible assignments
    assignments_tree.push_back(possible_assignments);
    // increment depth
    depth++;

    if (depth == possible_assignments.size()) {
      // we've assigned every row to a column
      // isomorphism found
      return true;
    }

    // if we get all the way to the bottom by doing this repeatedly,
    // we'll have found an isomorphism
    if (search(sub, graph, assignments_tree, possible_assignments, cols_used, col_depth, depth)) {
      return true;
    }
    else {
      // that didn't work. find a new column.
      cols_used[col] = false;
    }
    // this is fine since we only get here if we need to find a new column
  } while (true);
}

vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph) {
  // possible_assignments[i][j] == true iff a possible assignment exists from i in sub
  // to j in search graph
  vector < vector<bool> > possible_assignments (sub.vertices.size(),
    vector<bool>(graph.vertices.size(), false));

  // at first, every vertex in search graph with rank >= i is a possible assignment
  // this will be refined later
  // see refine_possible_assignments
  size_t i, j;
  size_t s_n = sub.vertices.size();
  size_t g_n = graph.vertices.size();
  for (i = 0; i < s_n; i++) {
    for (j = 0; j < g_n; j++) {
      if (graph.vertices[j].second >= sub.vertices[i].second) {
        possible_assignments[i][j] = true;
      }
    }
  }

  return possible_assignments;
}

bool refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments) {
  size_t i, j;
  size_t pa_n = possible_assignments.size();
  size_t pb_n = possible_assignments[0].size();
  bool changes_made = true;

  while (changes_made) {
    changes_made = false;

    for (i = 0; i < pa_n; i++) {
      // check if this row contains no 1s
      bool no_one = true;
    #pragma omp parallel num_threads(THREADS)
      for (j = 0; j < pb_n; j++) {
        if (possible_assignments[i][j]) {
          no_one = false;
          // check if all of i's neighbors have a possible assignment to a neighbor of j
          // iterate through all neighbors of i
          unordered_set<int> neighbors_i = sub.neighbors(sub.get_value(i));
          unordered_set<int>::iterator n_i = neighbors_i.begin();

          for (; n_i != neighbors_i.end(); n_i++) {
            // for each neighbor of i, iterate through all its possible assignments
            size_t k;
            bool has_corresponding_neighbor = false;
            for (k = 0; k < pb_n; k++) {
              if (possible_assignments[sub.get_index(*(n_i))][k]) {
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
              changes_made = true;
              break;
            }
          }
        }
      }

      // we never found a 1 in this row, so there's no point in continuing
      // there's at least one vertex in subgraph that cannot be mapped to any
      // vertex in the search graph, so we know that
      // possible_assignments cannot specify any isomorphism
      if (no_one) {
        return false;
      }
    }
  }

  // M was successfully refined without creating any rows with all 0s. return true.
  return true;
}
