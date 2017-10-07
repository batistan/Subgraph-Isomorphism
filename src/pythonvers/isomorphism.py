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
    for edge in subgraph.edges:
        if edge.first < i and edge.second < i:
            if not graph.has_edge(assignments[edge.first], assignments[edge.second]):
                return False

    # If all the vertices in the subgraph are assigned, then we are done.
    if i == subgraph.n_vertices:
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

def find_isomporhism(graph, subgraph):
    """ does the actual matching
    returns ??? FRAAAAN
    """

    assignments = []
    possible_assignments = [[True]*graph.n_vertices for i in range(subgraph.n_vertices)]
    if search(graph, subgraph, assignments, possible_assignments):
        return assignments
    return None

def update_possible_assignments(graph, subgraph, possible_assignments):
    """ docstring to suppress the warning
    figure it out later i guess
    """
    any_changes = True
    while any_changes:
        any_changes = False
        for i in range(0, len(subgraph.n_vertices)):
            for j in possible_assignments[i]:
                for x in subgraph.adjacencies(i):
                    match = False
                    for y in range(0, len(graph.n_vertices)):
                        if x in possible_assignments[x] and graph.has_edge(j, y):
                            match = True
                    if not match:
                        possible_assignments[i].remove(j)
                        any_changes = True
