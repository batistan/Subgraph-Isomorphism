class Graph:
    def __init__(self, vertices = {}, edges = {}, adjacencies = {}):
        self.vertices = vertices
        self.edges = edges
        self.adjacencies = adjacencies
        __update_adjacencies()

    #Private, don't want anyone calling this by accident.
    def __update_adjacencies():
        for edge in self.edges:
            self.adjacencies.append((edge[0], edge[1]))


    def add_vertex(self, vertex = Vertex()):
        self.verticies.append(vertex)

    def add_edge(self, source, dest, weight):
        if ( (source or dest) not in self.verticies):
            self.verticies.append(source)
            self.verticies.append(dest)

        self.edges.append( {source, dest}, weight) )
        self.adjacencies.append({source, dest})

    def has_edge(self, v1, v2):
        return ( {v1, v2} in self.adjacencies ) #if adjacent, there's an edge
