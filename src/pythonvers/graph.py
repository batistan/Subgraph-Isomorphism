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
        if edges is None:
            self.edges = []
        if adjacencies is None:
            self.adjacencies = []

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

    def add_edge(self, source, dest, weight):
        """ Add edge connecting source and dest
        along with weight
        """
        if (source or dest) not in self.vertices:
            self.vertices.append(source)
            self.vertices.append(dest)

        self.edges.append({source, dest, weight})
        self.adjacencies.append({source, dest})

    def has_edge(self, vert1, vert2):
        """ Checks if edge conecting vert1 and vert2 is in the graph
        """
        return ({vert1, vert2} in self.adjacencies) #if adjacent, there's an edge
