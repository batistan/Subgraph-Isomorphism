""" Subgraph Isomorphism
CPU Implementation
"""

import copy as cp # for deepcopy

def search(graph, subgraph, assignments, possible_assignments):
    """ Uses DFS to find instance of subgraph within larger graph
    """

    update_possible_assignments(graph, subgraph, possible_assignments)

    i = len(assignments)

    # Make sure that every edge between assigned vertices in the subgraph is also an
    # edge in the graph.

    # this loop calls graph.has_edge once for every edge in the subgraph
    for edge in subgraph.edges:
        if edge[0] < i and edge[1] < i:
            if not graph.has_edge(assignments[edge[0]], assignments[edge[1]]):
                return False

    # If all the vertices in the subgraph are assigned, then we are done.
    if i == subgraph.n_vertices():
        return True

    for j in possible_assignments[i]:
        if j not in assignments:
            assignments.append(j)

            # Create a new set of possible assignments, where graph node j is the only
            # possibility for the assignment of subgraph node i.
            new_possible_assignments = cp.deepcopy(possible_assignments)
            new_possible_assignments[i] = [j]

            if search(graph, subgraph, assignments, new_possible_assignments):
                return True

            assignments.pop()

        possible_assignments[i].remove(j)
        update_possible_assignments(graph, subgraph, possible_assignments)

def find_isomorphism(graph, subgraph):

    assignments = []
    possible_assignments = [[True]*graph.n_vertices() for i in range(subgraph.n_vertices())]
    # TODO remove subgraph instance from graph and run this again with the modified graph
    matches = 0
    if search(graph, subgraph, assignments, possible_assignments):
        #print(assignments)
        print("Match found")
        #return True
       # something like this
        matches += 1
        for node in assignments:
            graph.remove_vertex(node)
       #     optionally put the removed nodes into a new graph and tack that onto a list
       #     idk why max wants that but that's how i'd do it
        matches += find_isomorphism(graph, subgraph)
       #
    print(matches, " matches found")
    return matches

def update_possible_assignments(graph, subgraph, possible_assignments):
    """ docstring to suppress the warning
    figure it out later i guess
    """
    # TODO figure it out later i guess
    any_changes = True
    while any_changes:
        any_changes = False
        for i in range(0, subgraph.n_vertices()):
            for j in possible_assignments[i]:
                for adj in subgraph.adjacencies(i):
                    match = False
                    for vert in range(0, graph.n_vertices()):
                        # graph.has_edge gets called once for every vertex in the graph
                        # for every item in the subgraph's adjacencies
                        # for every possible assignments
                        # that is a huge number of calls to has_edge
                        # which is in itself an O(n) operation
                        # definitely room for improvement.
                        if adj in possible_assignments[adj] and graph.has_edge(j, vert):
                            match = True
                    if not match:
                        possible_assignments[i].remove(j)
                        any_changes = True
