#include <stddef.h>
#pragma once

typedef struct Node_t Node_t;

typedef struct Node_t {
  void *data;
  Node_t *next;
  Node_t *prev;
} Node_t;

void node_init(Node_t *node, void *data);

typedef struct List_t {
  Node_t *head;
  Node_t *tail;
} List_t;

void list_init(List_t *list);

void list_insert_after(List_t *list, Node_t *target, Node_t *new_node);

void list_insert_before(List_t *list, Node_t *target, Node_t *new_node);

void list_push_front(List_t *list, Node_t *new_node);

void list_push_back(List_t *list, Node_t *new_node);

void list_remove(List_t *list, Node_t *node);

Node_t *list_pop_front(List_t *list);

Node_t *list_peek_front(List_t *list);

Node_t *list_pop_back(List_t *list);

Node_t *list_peek_back(List_t *list);

Node_t *list_get(List_t *list, size_t idx);

#define LIST_FOR_EACH(curr, list)                                              \
  for (curr = (list)->head; curr != NULL; curr = curr->next)
