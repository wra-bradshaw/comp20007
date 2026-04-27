/* * * * * * *
 * Module for creating and manipulating direcvted and undirected graphs
 *
 * created for COMP20007 Design of Algorithms 2019 changed 2020
 * by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"

struct graph {
  List **vertices; // Array of adjacency Lists
  List **weights; // Array of weight Lists
  int n_vertices; // Number of vertices in the array
  int n_edges; // Number of vertices in the array
  bool directed; // Whether or not the graph is directed
};

// Creates a new graph with n_vertices and no edges.
Graph *new_graph(int n_vertices, bool directed) {
  Graph *graph = malloc(sizeof(*graph));
  assert(graph);

  // Store the number of vertices
  graph->n_vertices = n_vertices;
  graph->n_edges = 0;
  graph->directed = directed;

  // Create space for the (List *) pointers which will be used as
  // adjacency lists
  graph->vertices = malloc(sizeof(List *) * n_vertices);
  assert(graph->vertices);
  graph->weights = malloc(sizeof(List *) * n_vertices);
  assert(graph->weights);

  for (int i = 0; i < n_vertices; i++) {
    graph->vertices[i] = new_list();
    graph->weights[i] = new_list();
  }

  return graph;
}

// Frees the memory associated with a graph
void free_graph(Graph *graph) {
  int i;

  // Free each of the Lists
  for (i = 0; i < graph->n_vertices; i++) {
    free_list(graph->vertices[i]);
    graph->vertices[i] = NULL;
    free_list(graph->weights[i]);
    graph->weights[i] = NULL;
  }

  // Free the array of (List *) pointers
  free(graph->vertices);
  free(graph->weights);

  // Free the actual graph struct
  free(graph);
}

// Creates an edge between two vertices in the graph.
// If the graph is undirected both (u, v) and (v, u) will be added
// u and v should be the index of the vertices
// w is the weight of the edge
void graph_add_edge(Graph *graph, int u, int v, int w) {
  assert(graph != NULL);
  // Assert that u and v are valid indices
  assert(u >= 0 && u < graph->n_vertices);
  assert(v >= 0 && v < graph->n_vertices);

  if (graph_are_adjacent(graph, u, v)) {
    // Can't add an edge that already exists
    fprintf(stderr, "Error: edge already exists between %d and %d\n", u, v);
    exit(EXIT_FAILURE);
  }

  // Since we're implementing an undirected graph, this edge is bidirectional
  // As such we must add u to v's adjacency list and visa versa
  list_add_end(graph->vertices[u], v);
  if (!graph->directed) {
    list_add_end(graph->vertices[v], u);
  }

  list_add_end(graph->weights[u], w);
  if (!graph->directed) {
    list_add_end(graph->weights[v], w);
  }

  graph->n_edges++;
}

// Returns whether or not the vertices u and v are adjacent in the
// graph.
bool graph_are_adjacent(Graph *graph, int u, int v) {
  // Note: I added the list_contains() method to list.h and list.c for this
  List *u_neighbours = graph->vertices[u];
  List *v_neighbours = graph->vertices[v];

  if (graph->directed) {
    return list_contains(u_neighbours, v);
  } else {
    // Since the edges are bidirectional in this case
    // it's enough to check that u is in
    // v's adjacency list, or the other way around.
    // We'll do whichever operation is cheapest (i.e., the linear scan
    // will take the least amount of time)
    if (list_size(u_neighbours) < list_size(v_neighbours)) {
      return list_contains(u_neighbours, v);
    } else {
      return list_contains(v_neighbours, u);
    }
  }
}

// Saves the indices of u's neighbours in the neighbours array and returns
// the number of neighbours stored.
// Stores the weights in the weights array, unless weights == NULL.
// Gives an error if the size of the array provided (n) is less than the number
// of vertices.
int graph_get_neighbours(Graph *graph, int u, int *neighbours, int *weights, int n) {
  assert(graph != NULL);
  assert(neighbours != NULL);

  List *adjacentcy_list = graph->vertices[u];
  List *weights_list = graph->weights[u];

  // There's only room for n vertex indices in the array neighbours, so if
  // u has more neighbours than this then print an error
  if (list_size(adjacentcy_list) > n) {
    fprintf(stderr,
      "Error: graph_get_neighbours() not provided with a big enough array\n");
    exit(EXIT_FAILURE);
  }

  // Note: We added the ListIterator type for this purpose
  ListIterator *neighbour_iterator = new_list_iterator(adjacentcy_list);
  int i = 0;
  while (list_iterator_has_next(neighbour_iterator)) {
    neighbours[i] = list_iterator_next(neighbour_iterator);
    i++;
  }
  free_list_iterator(neighbour_iterator);

  // Now iterate through the weights
  if (weights != NULL) {
    ListIterator *weights_iterator = new_list_iterator(weights_list);
    int j = 0;
    while (list_iterator_has_next(weights_iterator)) {
      weights[j] = list_iterator_next(weights_iterator);
      j++;
    }
    free_list_iterator(weights_iterator);

    // After the loop, i will store the number of neighbours
    // So should j, so we'll assert that they're equal
    assert(i == j);
  }

  return i;
}

// Get the number of vertices in the graph
int graph_num_vertices(Graph *graph) {
  return graph->n_vertices;
}

// Get the number of edges in the graph
int graph_num_edges(Graph *graph) {
  return graph->n_edges;
}

// Get the out degree of a given node in the graph (i.e., number of out
// edges from this node).
int graph_out_degree(Graph *graph, int u) {
  return list_size(graph->vertices[u]);
}

// Prints the graph as an adjacency list in the format:
// 0: [a (w_a), b (w_b), ..., z (w_z)]
// ...
// where a, b, ..., z are the neighbours of 0 and the w_a, w_b and w_z are the
// weights of these edges.
void graph_print_adjacency_lists(Graph *graph) {
  int u;
  ListIterator *neighbour_iterator, *weights_iterator;
  for (u = 0; u < graph->n_vertices; u++) {
    printf("%2d: [", u);
    neighbour_iterator = new_list_iterator(graph->vertices[u]);
    weights_iterator = new_list_iterator(graph->weights[u]);

    for (int i = 0; i < graph_out_degree(graph, u); i++) {
      if (i > 0) printf(", ");
      printf("%d (%d)",
        list_iterator_next(neighbour_iterator),
        list_iterator_next(weights_iterator));
    }
    printf("]\n");

    free_list_iterator(neighbour_iterator);
    free_list_iterator(weights_iterator);
  }
}
