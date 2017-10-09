"""
Graph module
only has a class
"""

class Graph:
    """
    Graph class
    """
    def __init__(self, vertices=None, edges=None, adjacencies=None):
        if vertices is None:
            self.vertices = []
            self.n_vertices = 0
        else:
            self.vertices = vertices
            self.n_vertices = len(vertices)
        if edges is None:
            self.edges = []
        else:
            self.edges = edges
        if adjacencies is None:
            self.adjacencies = []
        else:
            self.adjacencies = adjacencies

        self.__update_adjacencies()

    #Private, don't want anyone calling this by accident.
    def __update_adjacencies(self):
        for edge in self.edges:
            self.adjacencies.append((edge[0], edge[1]))

    # TODO: figure out what fran was initially trying to do with the default argument here
    def add_vertex(self, vertex):#=vertex()):
        """ Add vertex to graph
        """
        self.vertices.append(vertex)
        self.n_vertices += 1

    def remove_vertex(self, vertex):
        """ Remove vertex from graph
            along with edges which contain it
        """
        self.vertices.remove(vertex)
        # list comprehension
        # TLDR remove all elements in self.edges which contain vertex
        # we need this since edges is a list of tuples, not of single elements
        self.edges.remove([edge for edge in self.edges if vertex in edge])
        # see above
        self.adjacencies.remove([adj for adj in self.adjacencies if vertex in adj])
        self.n_vertices -= 1

    def add_edge(self, source, dest, weight):
        """ Add edge connecting source and dest
        along with weight
        """
        if (source or dest) not in self.vertices:
            self.vertices.append(source)
            self.vertices.append(dest)

        self.edges.append([source, dest, weight])
        self.adjacencies.append([source, dest])

    def has_edge(self, vert1, vert2):
        """ Checks if edge conecting vert1 and vert2 is in the graph
        """
        return ([vert1, vert2] in self.adjacencies) #if adjacent, there's an edge

    # for debugging
    def print_graph(self):
        """ Prints information about the graph
        """
        print("Graph (vertices, edges, adjacencies):")
        print(self.vertices)
        print(self.edges)
        print(self.adjacencies)
