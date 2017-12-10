#include "graph.h"
#include <map>

using std::pair;
using std::map;

class dummy {
  public:
    dummy() {}

    int gi(int value) {
      if (this->vi.find(value) != this->vi.end()) {
        // map contains value
        // return index
        return this->vi[value];
      }
      // map does not contain value
      // return -1
      return -1;
    }

    void add_edge(int source, int dest, int weight) {

      this->insert(pair<int, int>(source, 0));

      this->insert(pair<int, int>(dest, 0));

      int sourceind = this->gi(source);
      int destind = this->gi(dest);

      this->v[sourceind].second++;
      this->v[destind].second++;

      if (sourceind < destind) {
        this->edges[sourceind][destind] = weight;
      }
      else {
        this->edges[destind][sourceind] = weight;
      }
    }

    void insert(pair<int,int> a) {
      if (this->gi(a.first) < 0) {
        this->v.push_back(a);
        this->edges.push_back(vector<int>(edges.size()+1,-1));
        //this->inserti(a.first, v.size()-1);
        this->vi[a.first] = v.size()-1;
        //this->insertv(v.size()-1, a.first);
        this->vv[v.size()-1] = a.first;
      }
    }

    void inserti(int a, int b) {
      this->vi.insert(pair<int,int>(a, b));
    }

    void insertv(int a, int b) {
      this->vv.insert(pair<int,int>(a, b));
    }

  private:
    vector<pair<int,int> > v;
    vector<vector<int> > edges;
    map<int,int> vi;
    map<int,int> vv;
};


int main() {

  Graph g;
  dummy d;
  map<int,int> vertex_indices = map<int, int>();
  map<int,int> vertex_vals = map<int, int>();

  int i = 0, j;
  for (; i < 1000; i++) {
    for (j = 0; j < 1000; j++) {
      //vertex_indices[i] = 1000-j;
      //vertex_vals[1000-i] = j;
      d.add_edge(i, 1000-j, 0);
      //int p = ((i+j)*(i+j+1))/2+j;
      //d.insert(pair<int,int>(p,0));
      //fprintf(stderr, "trying to add edge %d to %d\n", i, 1000-j);
      //g.add_edge(i, 1000-j, 0);
      //fprintf(stderr, "success!\n");
    }
    //g.add_vertex(pair<int,int>(i,0));
    //fprintf(stderr, "Vertex %d mapped to index %d\n", i, g.get_index(i));
    //fprintf(stderr, "Index %d mapped to vertex %d\n", g.get_value(g.get_index(i)), g.get_index(i));
  }

  //fprintf(stderr,"Trying to find an index that doesn't exist....\n");
  //fprintf(stderr,"Index of non-existent vertex 1234 is %d\n", g.get_index(1234));

  return 0;
}

