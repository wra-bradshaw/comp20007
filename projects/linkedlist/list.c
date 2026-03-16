#include "list.h"
#include "assert.h"

void node_init(Node_t *node, void *data) {
  node->next = NULL;
  node->prev = NULL;
  node->data = data;
}

void list_init(List_t *list) {
  list->head = NULL;
  list->tail = NULL;
}

void list_insert_after(List_t *list, Node_t *target, Node_t *new_node) {
  assert(target != NULL);

  new_node->next = target->next;
  new_node->prev = target;
  target->next = new_node;

  if (new_node->next != NULL) {
    new_node->next->prev = new_node;
  }

  if (target == list->tail) {
    list->tail = new_node;
  }
}

void list_insert_before(List_t *list, Node_t *target, Node_t *new_node) {
  assert(target != NULL);

  new_node->next = target;
  new_node->prev = target->prev;
  target->prev = new_node;

  if (new_node->prev != NULL) {
    new_node->prev->next = new_node;
  }

  if (target == list->head) {
    list->head = new_node;
  }
}

void list_push_front(List_t *list, Node_t *new_node) {
  new_node->prev = NULL;
  new_node->next = list->head;
  if (list->head != NULL) {
    list->head->prev = new_node;
  } else {
    list->tail = new_node;
  }

  list->head = new_node;
}

void list_push_back(List_t *list, Node_t *new_node) {
  new_node->next = NULL;
  new_node->prev = list->tail;
  if (list->tail != NULL) {
    list->tail->next = new_node;
  } else {
    list->head = new_node;
  }

  list->tail = new_node;
}

void list_remove(List_t *list, Node_t *node) {
  Node_t *next = node->next;
  Node_t *prev = node->prev;

  if (next != NULL) {
    next->prev = prev;
  }

  if (prev != NULL) {
    prev->next = next;
  }

  if (list->tail == node) {
    list->tail = prev;
  }

  if (list->head == node) {
    list->head = next;
  }

  node->next = NULL;
  node->prev = NULL;
}

Node_t *list_pop_front(List_t *list) {
  Node_t *head = list->head;

  if (head == NULL) {
    return NULL;
  }

  list->head = head->next;

  if (list->head != NULL) {
    list->head->prev = NULL;
  } else {
    list->tail = NULL;
  }

  head->next = NULL;
  head->prev = NULL;

  return head;
}

Node_t *list_peek_front(List_t *list) { return list->head; }

Node_t *list_pop_back(List_t *list) {
  Node_t *tail = list->tail;

  if (tail == NULL) {
    return NULL;
  }

  list->tail = tail->prev;

  if (list->tail != NULL) {
    list->tail->next = NULL;
  } else {
    list->head = NULL;
  }

  tail->prev = NULL;
  tail->next = NULL;

  return tail;
}

Node_t *list_peek_back(List_t *list) { return list->tail; }

Node_t *list_get(List_t *list, size_t idx) {
  Node_t *curr;
  size_t i = 0;
  LIST_FOR_EACH(curr, list) {
    if (i == idx) {
      return curr;
    }
    i++;
  }
  return NULL;
}
