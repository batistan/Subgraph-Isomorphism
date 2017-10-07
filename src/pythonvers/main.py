""" Main
"""
import sys #args
import getopt
from graph import Graph
from isomorphism import *

#C/C++ habits coming at me again
def main(argv):
    """ Main function
    """
    argc = len(argv)


    interactive, debug = handle_args(argc, argv)

    if interactive:
        filename = sys.stdin
    else:
        filename = argv[1]

    graph = import_data(filename, debug)

def import_data(filename, debug):
    """ Takes graph adjacency list written in file fd
        creates and returns graph object representing
        the graph in the file
        Note that edge weights are all 0 unless otherwise specified.
    """
    if filename == sys.stdin:
        fd = filename
    else:
        try:
            fd = open(filename, 'r')
        except IOError as err:
            sys.stderr.write("Error: Could not open file '%s'. Aborting.\n%s\n" % (filename, err))
            sys.exit(2)

    if debug:
        print(fd)

    graph = Graph()

    if debug:
        print("Reading file")
    for line in fd:
        vals = line.split()
        if len(vals) == 3:
            edge_weight = vals[2]
        elif len(vals) > 2:
            sys.stderr.write("Error reading file '%s'. Data should be in form \
                    'node1 node2 [edge_weight]', but line was %s" % line)
            sys.exit(3)
        else:
            edge_weight = 0

        graph.add_edge(vals[0], vals[1], edge_weight)

    #current_line = fd.readline()
    #while current_line != '':
    #    current_line = fd.readline()
    #    current_line.split()
    #    graph.add_edge(current_line[0], current_line[1], current_line[2])

    #    current_line = fd.readline()

    if filename != sys.stdin:
        fd.close()

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

    if argc == 1:
        usage(argv[0], True)
        sys.exit(1)
    elif argc >= 2:
        try:
            opts, args = getopt.getopt(argv, "hid", ["help", "interactive", "debug"])
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
            elif arg in ("-d", "--debug"):
                debug = True

    return interactive, debug

if __name__ == "__main__":
    main(sys.argv)
