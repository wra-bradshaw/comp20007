/* * * * * * *
 * Simple linked-list implementation of a priority queue.
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * modifications made by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 */

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "priorityqueue.h"

typedef struct node Node;

// a list node points to the next node in the list, and to some data
struct node {
	Node *next;
	int key;
  int priority;
};

// a priority_queue points to its first and last nodes, and stores its size
// i.e., (num. nodes)
struct priority_queue {
	Node *head;
	Node *tail;
	int size;
};

// helper function to create a new node and return its address
Node *new_pq_node();

// helper function to clear memory of a node (does not free the node's data)
void free_pq_node(Node *node);


/* * * *
 * FUNCTION DEFINITIONS
 */

// create a new queue and return a pointer to it
PriorityQueue *new_priority_queue() {
	PriorityQueue *queue = malloc(sizeof *queue);
	assert(queue);

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	return queue;
}

// destroy a queue and free its memory
void free_priority_queue(PriorityQueue *queue) {
	assert(queue != NULL);
	// free each node
	Node *node = queue->head;
	Node *next;
	while (node) {
		next = node->next;
		free_pq_node(node);
		node = next;
	}
	// free the queue struct itself
	free(queue);
}

// helper function to create a new node and return its address
Node *new_pq_node() {
	Node *node = malloc(sizeof *node);
	assert(node);

	return node;
}

// helper function to clear memory of a node
void free_pq_node(Node *node) {
	free(node);
}

// insert an element into the queue
void priority_queue_insert(PriorityQueue *queue, int key, int priority) {
	assert(queue != NULL);

	// create and initialise a new queue node
	Node *node = new_pq_node();
	node->key = key;
  node->priority = priority;
	node->next = queue->head; // next will be the old first node (may be null)

	// place it at the start of the queue
	queue->head = node;

	// if queue was empty, this new node is also the last node now
	if(queue->size == 0) {
		queue->tail = node;
	}

	// and we need to keep size updated!
	queue->size++;
}

// remove the element with the lowest priority and return the key
int priority_queue_remove_min(PriorityQueue *queue) {
  assert(queue != NULL);
	assert(queue->size > 0);

  Node *min_prev = NULL;
  Node *min_node = queue->head;
  int min_priority = min_node->priority;

  Node *prev = queue->head;
  Node *node = queue->head->next;
  int priority;
  while (node != NULL) {
    priority = node->priority;

    // if we find a lower priority, repalce the current minimums
    if (priority < min_priority) {
      min_prev = prev;
      min_node = node;
      min_priority = priority;
    }

    prev = node;
    node = node->next;
  }

  // Save the key
  int key = min_node->key;

  // If we're the head or the tail then we better update that we're removing
  // this node

  if (min_node == queue->head) {
    queue->head = queue->head->next;
  } else {
    assert(min_prev != NULL);
    assert(min_prev->next == min_node);
    min_prev->next = min_node->next;
  }

  if (min_node == queue->tail) {
    queue->tail = min_prev;
  }

  queue->size--;

	// and we're finished with the node holding this data
	free_pq_node(min_node);

	// done!
	return key;
}

// update an elements priority in the queue by key
// returns whether or not this was succesful (i.e., the key was already
// in the queue)
bool priority_queue_update(PriorityQueue *queue, int key, int new_priority) {
  assert(queue != NULL);

  Node *node = queue->head;
  while (node != NULL) {
    if (node->key == key) {
      node->priority = new_priority;
      return true;
    }

    node = node->next;
  }

  return false;
}

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(PriorityQueue *queue) {
	assert(queue != NULL);
	return (queue->size==0);
}
