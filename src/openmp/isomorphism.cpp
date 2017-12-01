#include <vector>
#include <utility>
#include <unordered_set>
#include <algorithm> //for std::find
#include "isomorphism.h"
#include "omp.h"

int THREADS = omp_get_max_threads();

using std::find;

vector < pair<int,int> > *find_isomorphism (Graph &sub, Graph &graph) {

  // assignments_tree[i] specifies the possible_assignments at depth i
  vector <vector <vector <bool> > > assignments_tree;
  // generate M0
  vector < vector<bool> > possible_assignments = create_possible_assignments(sub, graph);
  // return the address of this vector, or null if no isomorphism was found

  //This doesn't work, assignments is out of scope. Throw it on the heap.
  //vector <pair<int, int> > assignments = vector<pair<int, int> >(0);

  vector <pair<int, int> > *assignments = new vector <pair<int, int> >;


  // columns_used[i] = true iff column i in M has been used at current stage of computation
  vector <bool> columns_used (possible_assignments[0].size(), false);

  // column_depth[d] = k if column k was used at depth d
  vector<int> column_depth (possible_assignments.size(), 0);

  ssize_t i, j, k, pa_n = possible_assignments.size(), pb_n = possible_assignments[0].size();

  int depth = 0;
  columns_used[depth] = 0;

  // refine the possible assignments using the initial set of possible assignments
  // refine M0
  if (!refine_possible_assignments(sub, graph, possible_assignments)) {
    // if refine_possible_assignments returns false, then there is no possible isomorphism
    // so we can just stop here.
    return NULL;
  }

  // check if there's a j such that possible_assignments[d][j] = 1 and f[j] = 0
  // 2
  // if there is no j such that possible_assignments[d][j] == true and f[j] == false goto 7
  two:
  if (find(possible_assignments[depth].begin(),
        possible_assignments[depth].end(), true) != possible_assignments[depth].end()
      && find(column_depth.begin(), column_depth.end(), false) != column_depth.end()) {
    // there exists such a j
    assignments_tree.push_back(possible_assignments);
    if (depth == 0) {
      k = column_depth[depth];
    }

    else {
      k = 0;
    }
  }

  else {
    // we found no such j
    goto seven;
  }

  // 3
  // increment k until we find a possible assignment for this row
  three:
  for (; !possible_assignments[depth][k] || columns_used[k]; k++) ;

  // found one.
  for (j = 0; j < pb_n; j++) {
    // set all of the columns in this row to 0 except that first assignment we just found
    if (j != k) {
      possible_assignments[depth][j] = false;
    }
  }

  if(!refine_possible_assignments(sub, graph, possible_assignments)) {
    // 5
    five:
    int r;
    r = k+1;
    for (; r < pb_n; r++) {
      if (possible_assignments[depth][j] && !columns_used[j]) {
        possible_assignments = assignments_tree[depth];
        goto three;
      }
    }
    goto seven;

    // 6
    six:
    column_depth[depth] = k;
    columns_used[depth] = true;
    depth++;
    goto two;

    // 7
    seven:
    if (depth == 1) {
      return NULL;
    }

    else {
      columns_used[depth] = false;
      depth--;
      possible_assignments = assignments_tree[depth];
      k = column_depth[depth];
      // TODO goto 5
      goto five;
    }

  }

  // 4
  four:
  if (depth < pa_n) {
    goto six;
  }

  else {
    // isomorphism found. add it
    for (int i = 0; i < pa_n; i++) {
      for (int j = 0; j < pb_n; j++) {
        if (possible_assignments[i][j]) {
          (*assignments).push_back(pair<int, int>(sub.get_value(i), graph.get_value(j)));
        }
      }
    }
  }

  return assignments;
}

vector < vector<bool> > create_possible_assignments(Graph &sub, Graph &graph) {
  // possible_assignments[i][j] == true iff a possible assignm  ent exists from i in sub
  // to j in search graph
  printf("create_possible_assignments entered\n");
  vector < vector<bool> > possible_assignments (sub.vertices.size(),
  vector<bool>(graph.vertices.size(), false));

  // at first, every vertex in search graph with rank >= i is a possible assignments
  // this will be refined later
  ssize_t i, j;
  ssize_t s_n = sub.vertices.size();
  ssize_t g_n = graph.vertices.size();
  int id;
  #pragma omp parallel num_threads(THREADS)
  {
		  #pragma omp parallel for collapse(2)
		  for (i = 0; i < s_n; i++) {
			for (j = 0; j < g_n; j++) {
			  if (graph.vertices[j].second >= sub.vertices[i].second) {
				possible_assignments[i][j] = true;
			  }
			}
		  }
		  
	  
  }
  return possible_assignments;
}

bool refine_possible_assignments(Graph &sub, Graph &graph, vector < vector<bool> > &possible_assignments) {
  ssize_t i, j;
  ssize_t pa_n = possible_assignments.size();
  ssize_t pb_n = possible_assignments[0].size();
  bool changes_made = true;
  int id;
  #pragma omp parallel num_threads(THREADS)
  {
  
  while (changes_made) {
	
    changes_made = false;
	#pragma omp parallel for
    for (i = 0; i < pa_n; i++) {
      // check if this row contains no 1s
      bool no_one = true;
	  #pragma omp parallel for
      for (j = 0; j < pb_n; j++) {
        if (possible_assignments[i][j]) {
          no_one = false;
          // check if all of i's neighbors have a possible assignment to a neighbor of j
          // iterate through all neighbors of i
          unordered_set<int> neighbors_i = sub.neighbors(sub.get_value(i));
          unordered_set<int>::iterator n_i = neighbors_i.begin();

          for (; n_i != neighbors_i.end(); n_i++) {
            // for each neighbor of i, iterate through all its possible assignments
            ssize_t k;
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
        printf("Expecting no iso\n");
        return false;
      }
    }
  }
  }
  // M was successfully refined without creating any rows with all 0s. return true.
  printf("Expecting iso and seg fault\n");
  return true;
}
