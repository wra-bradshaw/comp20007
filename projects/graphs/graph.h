/* * * * * * *
 * Module for creating and manipulating direcvted and undirected graphs
 *
 * created for COMP20007 Design of Algorithms 2019 changed 2020
 * by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct graph Graph;

// Creates a new graph with n_vertices and no edges
Graph *new_graph(int n_vertices, bool directed);

// Frees the memory associated with a graph
void free_graph(Graph *graph);

// Creates an edge between two vertices in the graph.
// If the graph is undirected both (u, v) and (v, u) will be added
// u and v should be the index of the vertices
// w is the weight of the edge
void graph_add_edge(Graph *graph, int u, int v, int w);

// Returns whether or not the vertices u and v are adjacent in the
// graph.
bool graph_are_adjacent(Graph *graph, int u, int v);

// Saves the indices of u's neighbours in the neighbours array and returns
// the number of neighbours stored.
// Stores the weights in the weights array.
// Gives an error if the size of the array provided (n) is less than the number
// of vertices.
int graph_get_neighbours(Graph *graph, int u, int *neighbours, int *weights, int n);

// Get the number of vertices in the graph
int graph_num_vertices(Graph *graph);

// Get the number of edges in the graph
int graph_num_edges(Graph *graph);

// Get the out degree of a given node in the graph (i.e., number of out
// edges from this node).
int graph_out_degree(Graph *graph, int u);

// Prints the graph as an adjacency list in the format:
// 0: [a (w_a), b (w_b), ..., z (w_z)]
// ...
// where a, b, ..., z are the neighbours of 0 and the w_a, w_b and w_z are the
// weights of these edges.
void graph_print_adjacency_lists(Graph *graph);

#endif
