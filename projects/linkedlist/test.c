#include <criterion/criterion.h>
#include <stddef.h>

#include "list.h"

static void assert_list_order(List_t *list, Node_t **expected, size_t count) {
  Node_t *curr = list->head;

  if (count == 0) {
    cr_assert_null(list->head);
    cr_assert_null(list->tail);
    return;
  }

  cr_assert_eq(list->head, expected[0]);
  cr_assert_eq(list->tail, expected[count - 1]);

  for (size_t i = 0; i < count; i++) {
    cr_assert_not_null(curr, "list ended early at index %zu", i);
    cr_assert_eq(curr, expected[i], "unexpected node at index %zu", i);
    cr_assert_eq(curr->prev, i == 0 ? NULL : expected[i - 1],
                 "unexpected prev link at index %zu", i);
    cr_assert_eq(curr->next, i + 1 == count ? NULL : expected[i + 1],
                 "unexpected next link at index %zu", i);
    curr = curr->next;
  }

  cr_assert_null(curr, "list contained more nodes than expected");
}

Test(node, init_sets_data_and_clears_links) {
  int value = 42;
  Node_t node = {.next = (Node_t *)0x1, .prev = (Node_t *)0x1, .data = NULL};

  node_init(&node, &value);

  cr_assert_eq(node.data, &value);
  cr_assert_null(node.next);
  cr_assert_null(node.prev);
}

Test(list, init_clears_head_and_tail) {
  List_t list = {.head = (Node_t *)0x1, .tail = (Node_t *)0x1};

  list_init(&list);

  cr_assert_null(list.head);
  cr_assert_null(list.tail);
}

Test(list, push_back_on_empty_list_sets_head_tail_and_links) {
  List_t list;
  Node_t first;
  int value = 1;

  list_init(&list);
  node_init(&first, &value);

  list_push_back(&list, &first);

  Node_t *expected[] = {&first};
  assert_list_order(&list, expected, 1);
}

Test(list, push_front_on_empty_list_sets_head_tail_and_links) {
  List_t list;
  Node_t first;
  int value = 1;

  list_init(&list);
  node_init(&first, &value);

  list_push_front(&list, &first);

  Node_t *expected[] = {&first};
  assert_list_order(&list, expected, 1);
}

Test(list, insert_after_updates_adjacent_links_and_tail) {
  List_t list;
  Node_t first;
  Node_t second;
  Node_t third;
  int a = 1;
  int b = 2;
  int c = 3;

  list_init(&list);
  node_init(&first, &a);
  node_init(&second, &b);
  node_init(&third, &c);

  list.head = &first;
  list.tail = &second;
  first.next = &second;
  second.prev = &first;

  list_insert_after(&list, &second, &third);

  Node_t *expected[] = {&first, &second, &third};
  assert_list_order(&list, expected, 3);
}

Test(list, insert_before_updates_adjacent_links_and_head) {
  List_t list;
  Node_t first;
  Node_t second;
  Node_t inserted;
  int a = 1;
  int b = 2;
  int c = 3;

  list_init(&list);
  node_init(&first, &a);
  node_init(&second, &b);
  node_init(&inserted, &c);

  list.head = &first;
  list.tail = &second;
  first.next = &second;
  second.prev = &first;

  list_insert_before(&list, &second, &inserted);

  Node_t *expected[] = {&first, &inserted, &second};
  assert_list_order(&list, expected, 3);
}

Test(list, remove_detaches_middle_node_and_preserves_neighbours) {
  List_t list;
  Node_t first;
  Node_t middle;
  Node_t last;
  int a = 1;
  int b = 2;
  int c = 3;

  list_init(&list);
  node_init(&first, &a);
  node_init(&middle, &b);
  node_init(&last, &c);

  list.head = &first;
  list.tail = &last;
  first.next = &middle;
  middle.prev = &first;
  middle.next = &last;
  last.prev = &middle;

  list_remove(&list, &middle);

  Node_t *expected[] = {&first, &last};
  assert_list_order(&list, expected, 2);
  cr_assert_null(middle.prev);
  cr_assert_null(middle.next);
}

Test(list, pop_front_updates_head_tail_and_detaches_node) {
  List_t list;
  Node_t first;
  Node_t second;
  int a = 1;
  int b = 2;

  list_init(&list);
  node_init(&first, &a);
  node_init(&second, &b);

  list.head = &first;
  list.tail = &second;
  first.next = &second;
  second.prev = &first;

  Node_t *removed = list_pop_front(&list);

  cr_assert_eq(removed, &first);
  cr_assert_null(removed->next);
  Node_t *expected[] = {&second};
  assert_list_order(&list, expected, 1);
}

Test(list, pop_back_updates_head_tail_and_detaches_node) {
  List_t list;
  Node_t first;
  Node_t second;
  int a = 1;
  int b = 2;

  list_init(&list);
  node_init(&first, &a);
  node_init(&second, &b);

  list.head = &first;
  list.tail = &second;
  first.next = &second;
  second.prev = &first;

  Node_t *removed = list_pop_back(&list);

  cr_assert_eq(removed, &second);
  cr_assert_null(removed->prev);
  Node_t *expected[] = {&first};
  assert_list_order(&list, expected, 1);
}

Test(list, get_returns_node_at_index_and_null_out_of_bounds) {
  List_t list;
  Node_t first;
  Node_t second;
  Node_t third;
  int a = 1;
  int b = 2;
  int c = 3;

  list_init(&list);
  node_init(&first, &a);
  node_init(&second, &b);
  node_init(&third, &c);

  list.head = &first;
  list.tail = &third;
  first.next = &second;
  second.prev = &first;
  second.next = &third;
  third.prev = &second;

  cr_assert_eq(list_get(&list, 0), &first);
  cr_assert_eq(list_get(&list, 1), &second);
  cr_assert_eq(list_get(&list, 2), &third);
  cr_assert_null(list_get(&list, 3));
}
