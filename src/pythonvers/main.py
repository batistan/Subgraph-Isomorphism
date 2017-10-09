""" Main
"""
import sys #args
import getopt
from graph import Graph
from isomorphism import find_isomorphism
import cProfile

def main(argv):
    """ Main function
    """
    argc = len(argv)

    interactive, debug = handle_args(argc, argv)

    if interactive:
        print("Enter the graph as a space-separated adjacency list of the form\
                node1 node2 [edge_weight]:")
        filename = sys.stdin
    else:
        filename = argv[1]

    graph = import_data(filename, debug)

    if debug:
        graph.print_graph()

    if interactive:
        print("Enter the subgraph using the same format:")
        sub = sys.stdin
    else:
        sub = argv[2]

    subgraph = import_data(sub, debug)

    if debug:
        subgraph.print_graph()

    cProfile.runctx('find_isomorphism(graph, subgraph)', globals(), locals())

def import_data(filename, debug):
    """ Takes graph adjacency list written in file fd
        creates and returns graph object representing
        the graph in the file
        Note that edge weights are all 0 unless otherwise specified.
    """
    if filename == sys.stdin:
        fdesc = filename
    else:
        try:
            fdesc = open(filename, 'r')
        except IOError as err:
            sys.stderr.write("Error: Could not open file '%s'. Aborting.\n%s\n" % (filename, err))
            sys.exit(2)

    if debug:
        print(fdesc)

    graph = Graph()

    if debug:
        print("Reading file")
    for line in fdesc:
        vals = line.split()
        if len(vals) == 3:
            edge_weight = int(vals[2])
        elif len(vals) > 2:
            sys.stderr.write("Error reading file '%s'. Data should be in form \
                    'node1 node2 [edge_weight]', but line was %s\n" % line)
            sys.exit(3)
        else:
            edge_weight = 0

        graph.add_edge(int(vals[0]), int(vals[1]), edge_weight)

    if filename != sys.stdin:
        fdesc.close()

    return graph

def usage(name, err):
    """ Prints usage info
    """
    usg = """usage: %s [FILE] [OPTION]
    Options and arguments:
    FILE:               Space-separated file containing graph adjacency list. If edge weights are not specified, all weights will be assumed 0.
    -h, --help:         Print this help message and exit.
    -i, --interactive:  Get space-separated graph adjacency list from stdin.
    -d, --debug:        Print debugging information.\n"""
    if err:
        sys.stderr.write(usg % name)
        return

    sys.stdout.write(usg % name)

def handle_args(argc, argv):
    """ get command line arguments, set variables accordingly
    """
    # TODO change to False for final deployment
    debug = True
    interactive = False

    # arguments should be either 2 files or the two files and some args
    # unless -i is specified in which case just the args
    if argc < 2:
        usage(argv[0], True)
        sys.exit(1)
    else:
        try:
            # using _ as a variable name is sort of the conventional way of saying
            # "we don't need or use this variable but i'm assigning it because
            # the method or api or whatever i'm using requires it"
            _, args = getopt.getopt(argv, "hid", ["help", "interactive", "debug"])
        except getopt.GetoptError:
            # print usage info and quit
            usage(argv[0], True)
            sys.exit(1)
        # handle args
        for arg in args:
            if arg in ("-h", "--help"):
                usage(argv[0], False)
                sys.exit()
            elif arg in ("-i", "--interactive"):
                interactive = True
                args.remove(arg)
            elif arg in ("-d", "--debug"):
                debug = True
                args.remove(arg)

    return interactive, debug

if __name__ == "__main__":
    main(sys.argv)
