#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "graph.h"
#include "isomorphism.h"

const char* program_name;

int *handle_args(int argc, char **argv);
void usage (FILE* stream, int exit_code);
Graph *import_data(char *filename, int debug);

int main(int argc, char **argv) {

  program_name = argv[0];
  int *args = handle_args(argc, argv);

  int interactive = args[0];
  int debug = args[1];

  char *graph_filename = NULL;
  char *sub_filename = NULL;

  if (!interactive) {
    graph_filename = args[2];
    sub_filename = args[3];
  }

  else {
    graph_filename = "stdin";
    sub_filename = "stdin";
  }

  if (debug) {
    printf("inter = %d, debug = %d\n", interactive, debug);
    printf("Graph file %s, subgraph file %s\n", graph_filename, sub_filename);
  }

  // TODO: implement import_data

  Graph *graph = import_data(graph_filename, debug);
  Graph *subgraph = import_data(sub_filename, debug);

  return 0;
}

Graph *import_data(const char *filename, const int debug) {

  FILE *fd;
  if (strcmp(filename, "stdin") == 0) {
    fd = stdin;
  }

  else {
    fd = fopen(filename, "r");
    if (fd == -1) {
      fprintf(stderr, "Could not open file %s for reading.\n", filename);
      perror("Error calling fopen()");
      exit(errno);
    }
  }

  char *line = NULL;
  size_t n = 0;
  
  char *tempa, *tempb, *tempweight;

  vector < Vertex::vertex > vertices;
  vector <tuple> edges;
  vector < unordered_set<int> > adjacencies;
  Graph g = Graph(vertices, edges, adjacencies);

  if (debug) {
    printf("Reading file %s...\n", filename);
  }

  // getline returns -1 on failure to read a line (including EOF)
  while (getline(&line, &n, '\n', fd) != -1) {
    tempa = strtok(line, " ");
    // strtok returns NULL if the token (space in this case) was not found in the str
    // otherwise return the string up to the token (exclusive)
    if (tempa == NULL) {
      fprintf(stderr, "Error parsing file %s. Adjacency list must be of form 'node1 node2 [edge_weight]',\
          but line was %s\n", filename, line);
      exit(-1);
    }
    
    tempb = strtok(line, " ");
    if (tempb == NULL) {
      fprintf(stderr, "Error parsing file %s. Adjacency list must be of form 'node1 node2 [edge_weight]',\
          but line was %s\n", filename, line);
      exit(-1);
    }

    tempweight = strtok(line, " ");
    if (tempweight == NULL) {
      tempweight = "0";
    }

    atempa = atoi(tempa);
    atempb = atoi(tempb);
    atempweight = atoi(tempweight);

    printf("Edge is %d %d %d\n", atempa, atempb, atempweight);
    g.add_edge(atempa, atempb, atempweight);
  }
    
  if (fd != stdin) {
    int stat = close(fd);
    if (stat != 0) {
      perror("Error closing file");
      // we never wrote to the file so we probably don't even need to check for errors in the first place
      // but just kill the program if there's an error closing it
      exit(errno);
    }
  }

  free(line);

  return &g;
}

int *handle_args(int argc, char **argv) {
  // for getopt
  const char* const short_options = "hid";
  const struct option long_options[] = {
    { "help",  0, NULL, 'h' },
    { "interactive", 0, NULL, 'i'},
    { "debug", 0, NULL, 'd'},
    { NULL, 0, NULL, 0 }
  }

  int next_option;

  const char* graph_filename = NULL;
  const char* sub_filename = NULL;
  // we only need to save interactive and debug for now
  int *returnargs = (int *) malloc(2*sizeof(int));
  int interactive = 0;
  // TODO change to 0 for final deployment
  int debug = 1;

  do {
    next_option = getopt_long(argc, argv, short_options,
                              long_options, NULL);
    switch(next_option) {
      case 'h':
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
    free(returnargs);
    returnargs = (int *) malloc(4*sizeof(int));
    returnargs[2] argv[opind];
    returnargs[3] = argv[opind+1];

  }

  if (debug) {
    printf("Interactive set to %d. Debug is %d.\n", interactive, debug);
    if (!interactive) {
      printf("Using file %s for graph and %s for subgraph.\n",argv[opind],argv[opind+1]);
    }
  }

  // put what we want to return in their places.
  returnargs[0] = interactive;
  returnargs[1] = debug;
  
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
