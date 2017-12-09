// use stderr to print debug info since stderr is unbuffered
// i.e. anything sent to stderr will be printed immediately
// instead of put into a buffer and printed when the buffer
// gets flushed
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream> //debugging
#include <vector>
#include <tuple>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "graph.h"
#include "isomorphism.h"
#include <unordered_set>
#include <cstring>
using std::string;

const char* program_name;

vector<string> handle_args(int argc, char **argv);
void usage (FILE* stream, int exit_code);
Graph import_data(const char *filename, const int debug);

int main(int argc, char **argv) {

  program_name = argv[0];

  vector<string> args = handle_args(argc, argv);

  int interactive = atoi(args[0].c_str());
  int debug = atoi(args[1].c_str());

  char filename_default[] = "stdin";

  const char *graph_filename = (!interactive) ? args[2].c_str()
                                              : filename_default;
  const char *sub_filename = (!interactive) ? args[3].c_str()
                                              : filename_default;

  if (debug) {
    fprintf(stderr, "inter = %d, debug = %d\n", interactive, debug);
    fprintf(stderr, "Graph file %s, subgraph file %s\n", graph_filename, sub_filename);
  }


  Graph graph = import_data(graph_filename, debug);

  if (debug) {
    fprintf(stderr, "Successfully imported graph from file %s.\n",
        graph_filename);
  }

  Graph subgraph = import_data(sub_filename, debug);

  if (debug) {
    fprintf(stderr, "Successfully imported subgraph from file %s.\n",
        sub_filename);
  }

  if (debug) {
    fprintf(stderr, "Running find_isomorphism....\n");
  }

  vector<int> isomorphism = find_isomorphism(subgraph, graph);

  if (debug){
    fprintf(stderr, "find_isomorphism terminated.\n");
  }

  if (isomorphism.size() == 0) {
    printf("No isomorphism found.\n");
  }

  else{
    printf("Isomorphism found! Assignments are:\n");
    size_t num_assignments = isomorphism.size();
    for (size_t i = 0; i < num_assignments; i++) {
      printf("Vertex %lu maps to vertex %d\n", i, isomorphism[i]);
    }
  }

  return 0;
}

Graph import_data(const char *filename, const int debug) {

  FILE *fd;
  if (strcmp(filename, "stdin") == 0) {
    fd = stdin;
  }

  else {
    fd = fopen(filename, "r");
    if (fd == NULL) {
      fprintf(stderr, "Could not open file %s for reading.\n", filename);
      perror("Error calling fopen()");
      exit(errno);
    }
  }

  char *line = NULL;
  size_t n = 0;

  Graph g;

  if (debug) {
    fprintf(stderr, "Reading file %s...\n", filename);
  }

  char *tempa, *tempb, *tempweight;
  char tempweight_default[] = "0";
  ssize_t bytes_read;
  // getline returns -1 on failure to read a line (including EOF)
  bytes_read = getline(&line, &n, fd);
  while (bytes_read != -1) {
    tempa = strtok(line, " ");
    // strtok returns NULL if the token (space in this case) was not found in the str
    // otherwise return the string up to the token (exclusive)
    if (tempa == NULL) {
      fprintf(stderr, "Error parsing file %s. Adjacency list must be of form 'node1 node2 [edge_weight]',\
          but line was %s\n", filename, line);
      exit(-1);
    }

    // if arg is null after a call to strtok, it'll keep reading after the position of the last token
    tempb = strtok(NULL, " ");
    if (tempb == NULL) {
      fprintf(stderr, "Error parsing file %s. Adjacency list must be of form 'node1 node2 [edge_weight]',\
          but line was %s\n", filename, line);
      exit(-1);
    }

    tempweight = strtok(NULL, " ");
    if (tempweight == NULL) {
      tempweight = tempweight_default;
    }

    int atempa = atoi(tempa);
    int atempb = atoi(tempb);
    int atempweight = atoi(tempweight);

    //printf("Edge is %d %d %d\n", atempa, atempb, atempweight);
    // this will also add the vertices if they don't exist
    // add_vertex is called from within add_edge if no vertex with the given value exists
    g.add_edge(atempa, atempb, atempweight);
    bytes_read = getline(&line, &n, fd);
  }

  if (fd != stdin) {
    int stat = fclose(fd);
    if (stat != 0) {
      perror("Error closing file");
      // we never wrote to the file so we probably don't even need to check for errors in the first place
      // but just kill the program if there's an error closing it
      exit(errno);
    }
  }

  free(line);

  return g;
}

vector<string> handle_args(int argc, char **argv) {
  // for getopt
  const char* const short_options = "hid";
  const struct option long_options[] = {
    { "help",  0, NULL, 'h' },
    { "interactive", 0, NULL, 'i'},
    { "debug", 0, NULL, 'd'},
    { NULL, 0, NULL, 0 }
  };

  // we only need to save interactive and debug for now
  vector<string> returnargs;
  returnargs.resize(4);
  int interactive = 0;
  // TODO change to 0 for final deployment
  int debug = 1;

  if (argc < 2) {
    //free(returnargs);
    usage(stderr, 1);
  }

  int next_option;
  do {
    next_option = getopt_long(argc, argv, short_options,
                              long_options, NULL);
    switch(next_option) {
      case 'h':
        //free(returnargs);
        usage(stdout, 0);
        // calling usage quits the program anyway but whatever
        // we'll put a break here anyway
        break;

      case 'i':
        interactive = 1;
        break;

      case 'd':
        debug = 1;
        break;

      case '?': // invalid option
        //free(returnargs);
        usage(stderr, 1);
        break;

      case -1: // no more options
        break;

      default: // something went very wrong if we got here
        abort();
    }
  }
  while (next_option != -1);

  // now that we've gone through all the options, OPIND points to first
  // nonoption argument. which is hopefully the first filename. unless
  // interactive was specified, in which case yell at the user.

  if (!interactive) {
    // interactive was 0. free the memory it took up and reallocate enough for the two args
    // and the two filenames
    //free(returnargs);
    //returnargs = (string *) malloc(4*sizeof(string));
    returnargs[2] = string(argv[optind]);
    returnargs[3] = string(argv[optind+1]);

  }

  if (debug) {
    fprintf(stderr,"Interactive set to %d. Debug is %d.\n", interactive, debug);
    if (!interactive) {
      fprintf(stderr,"Using file %s for graph and %s for subgraph.\n",argv[optind],argv[optind+1]);
    }
  }

  // put what we want to return in their places.
  //sprintf(returnargs[0],"%d",interactive);
  returnargs[0] = std::to_string(interactive);
  //sprintf(returnargs[1],"%d",debug);
  returnargs[1] = std::to_string(debug);

  return returnargs;
}

void usage (FILE* stream, int exit_code) {
  fprintf(stream, "usage: %s [FILE] [OPTION]\n", program_name);
  fprintf(stream,
    "Options and arguments:\n"
    "FILE:               Space-separated file containing graph adjacency list. If edge weights are not specified, all weights will be assumed 0.\n"
    "-h, --help:         Print this help message and exit.\n"
    "-i, --interactive:  Get space-separated graph adjacency list from stdin.\n"
    "-d, --debug:        Print debugging information.\n");

  exit(exit_code);
}
