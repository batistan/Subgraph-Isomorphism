#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <cstring>
#include <vector>
#include <string>

#include "isomorphism.h"

const char* program_name;

using std::string;
using std::vector;

void set_up_graphs(int argc, char **argv, Graph &sub, Graph &graph);
vector<string> handle_args(int argc, char **argv);
void usage (FILE* stream, int exit_code);
Graph import_data(const char *filename);

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
    int cpu, int group_fd, unsigned long flags) {

  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
      group_fd, flags);
  return ret;
}

int main(int argc, char **argv) {

  struct perf_event_attr pe;
  long long count;
  int fd;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }

  Graph subgraph, graph;

  set_up_graphs(argc, argv, subgraph, graph);

  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

  vector<int> isomorphism = find_isomorphism(subgraph, graph);

  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  read(fd, &count, sizeof(long long));

  if (isomorphism.size() == 0) {
    printf("No isomorphism found.\n");
  }

  else {
    printf("Isomorphism found! Assignments omitted, since we're just checking"
        " performance.\n");
  }

  printf("Searching for isomorphism took %lld CPU cycles\n", count);

  close(fd);
}

void set_up_graphs(int argc, char **argv, Graph &sub, Graph &graph) {

  program_name = argv[0];

  vector<string> args = handle_args(argc, argv);

  int interactive = atoi(args[0].c_str());

  char filename_default[] = "stdin";

  const char *graph_filename = (!interactive) ? args[2].c_str()
                                              : filename_default;
  const char *sub_filename = (!interactive) ? args[3].c_str()
                                              : filename_default;

  graph = import_data(graph_filename);

  sub = import_data(sub_filename);

  return;
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

  vector<string> returnargs;
  returnargs.resize(4);
  int interactive = 0;
  int debug = 0;

  if (argc < 2) {
    usage(stderr, 1);
  }

  int next_option;
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
  returnargs[0] = std::to_string(interactive);
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

Graph import_data(const char *filename) {

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

  // don't free line since it's null now

  return g;
}
