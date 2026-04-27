/* * * * * * *
 * Simple linked-list implementation of a priority queue.
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * modifications made by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct priority_queue PriorityQueue;

// create a new queue and return a pointer to it
PriorityQueue *new_priority_queue();

// destroy a queue and free its memory
void free_priority_queue(PriorityQueue *queue);

// insert an element into the queue
void priority_queue_insert(PriorityQueue *queue, int key, int priority);

// remove the element with the lowest priority and return the key
int priority_queue_remove_min(PriorityQueue *queue);

// update an elements priority in the queue by key
// returns whether or not this was succesful (i.e., the key was already
// in the queue)
bool priority_queue_update(PriorityQueue *queue, int key, int new_priority);

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(PriorityQueue *queue);

#endif
