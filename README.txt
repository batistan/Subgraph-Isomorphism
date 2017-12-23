Subgraph Isomorphism
====================

Takes edge lists of two graphs, uses the 1976 Ullmann algorithm to return a mapping from the vertices of the second graph to the vertices of a subgraph of the first one, if such a mapping exists. If not, reports there is none.

Can be used to check or verify isomorphism between two graphs, if the two are of the same size. For example, modeling various chemical compounds as graphs allows for checking if they share the same structure.

Includes Python, C++, and OpenMP-optimized versions.

Compiling
---------

Enter the directory for the version desired and simply run `make`. For example, to compile the C++ version (without OpenMP optimizations), enter the `cvers` directory and run `make`. An executable named `main` will be produced.

Performance checking is only fully supported for the C++ and OpenMP verions. To produce an executable to check the performance of the algorithm, run `make perf` from the same directory you would run `make` normally. An executable named `perf` will be produced.

Running
-------

The procedure for both `main` and `perf` is the same. Run the executable and pass in the files containing the edge list (optionally with weights) of the search graph and the smaller graph, respectively. For example, to run `main` to check for an instance of a graph defined in `sub.edges` within a larger graph defined in `graph.edges`, run the command `./main graph.edges sub.edges`. Checking performance uses all of the same arguments, but the command would instead be `./perf graph.edges sub.edges`.

Usage information may be printed with the `-h` or `--help` flag. For additional information on program status while running, add the `-d` or `--debug` flag. 

Interactive mode may be specified by the `-i` or `--interactive` flag. In this case, any files provided are ignored. The user is instead prompted to enter the edge lists for both graphs manually from standard input. This also allows for the edge lists to be piped in from the output of another program through I/O redirection, if desired.

Input Format
------------

Edge lists consist of text files, with each edge of the graph on its own line. Each line consists of two or three integers: the two vertices defining the edge, and an optional edge weight. Note that the edge weight has no bearing on the isomorphism between the two graphs, but it does not need to be removed if already in the edge list. The file may not have any other contents. See the `.edges` files in the `src/` directory for examples. 
