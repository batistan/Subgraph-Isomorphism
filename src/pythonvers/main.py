import sys #args
from graph import *
from ismorphism import *

#C/C++ habits coming at me again
def import_data(graph):
    if (len(sys.argv) == 0)
        print("You need a filename as an argument.\n 'python main.py filename.txt'")
        sys.exit(0)

    fh = open(sys.argv[1], 'r')

    current_line = fh.readline()
    while (current_line != ''):
        current_line = fh.readline()
        current_line.split()
        graph.add_edge(current_line[0], current_line[1], current_line[2])

        current_line = fh.readline()

    fh.close()

    return 0


test_graph = Graph()
import_data(test_graph)
