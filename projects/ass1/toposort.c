/*
 * Starter file for Project 1 Part 2 (Link
 * https://edstem.org/au/courses/31090/lessons/104103/slides/714974)
 *
 * Created for COMP20007 Design of Algorithms 2026
 * Template written by Thomas Minuzzo <thomas.minuzzo@unimelb.edu.au>
 * Implementation by YOUR NAME HERE.
 * Compile with gcc -Wall -g -o toposort toposort.c -lm
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_LENGTH 9
#define DEBUG 0
#define PRINTED -1

typedef struct {
  char code[CODE_LENGTH + 1]; //+1 for null byte
  char **pre_req;             // Malloc each to CODE_LENGTH + !
  int num_prereqs;
} subject_t;

/* Reads a list of subjects and their prerequisites into a dynamically allocated
 * adjacency list*/
subject_t *read_major(int *num_subjects);
/* Prints the associated memory of a major, for debugging*/
void print_major(subject_t *major, int num_subjects);
/* Implements Kahn's algorithm to topologically sort a major's subjects */
void kahns_toposort(subject_t *major, int num_subjects);
/* Frees a dynamically allocated adjacency list*/
void free_major(subject_t *major, int num_subjects);
/* Gets the index in major array for a code */
int get_code_index(char *code, subject_t *major, int num_subjects);

int main(int argc, char **argv) {

  int num_subjects;
  subject_t *major = read_major(&num_subjects);

  if (DEBUG)
    print_major(major, num_subjects);

  kahns_toposort(major, num_subjects);

  free_major(major, num_subjects);

  return 0;
}

subject_t *read_major(int *num_subjects) {

  scanf("Number of subjects: %d\n", num_subjects);

  subject_t *major = (subject_t *)malloc(sizeof(subject_t) * (*num_subjects));

  for (int i = 0; i < (*num_subjects); i++) {
    for (int c = 0; c < CODE_LENGTH; c++) {
      scanf("%c", &major[i].code[c]);
    }
    major[i].code[CODE_LENGTH] = '\0';

    if (DEBUG) {
      printf("%s\n", major[i].code);
    }

    scanf("(%d)", &major[i].num_prereqs);

    major[i].pre_req = (char **)malloc(sizeof(char *) * major[i].num_prereqs);
    for (int j = 0; j < major[i].num_prereqs; j++) {
      major[i].pre_req[j] = (char *)malloc(sizeof(char) * (CODE_LENGTH + 1));
      for (int c = 0; c < CODE_LENGTH; c++) {
        scanf("%c", &major[i].pre_req[j][c]);
      }
      major[i].pre_req[j][CODE_LENGTH] = '\0';
      if (DEBUG) {
        printf("%s\n", major[i].pre_req[j]);
      }

      scanf(",");
    }
    scanf("\n");
  }
  return major;
}

void print_major(subject_t *major, int num_subjects) {
  for (int i = 0; i < num_subjects; i++) {
    printf("%s - ", major[i].code);
    for (int j = 0; j < major[i].num_prereqs; j++) {
      printf("%s", major[i].pre_req[j]);
      if (j != major[i].num_prereqs -
                   1) { // Not on last pre-requisite, needs a comma
        printf(", ");
      }
    }
    printf("\n");
  }
}

int get_code_index(char *code, subject_t *major, int num_subjects) {
  for (int i = 0; i < num_subjects; i++) {
    if (strcmp(code, major[i].code) == 0) {
      return i;
    }
  }
  return -1;
}

int find_zero(int *arr, int num_subjects) {
  for (int i = 0; i < num_subjects; i++) {
    if (arr[i] == 0) {
      return i;
    }
  }
  return -1;
}

void kahns_toposort(subject_t *major, int num_subjects) {
  int *sorted = (int *)malloc(sizeof(int) * (num_subjects));
  int sorted_n = 0;
  for (int i = 0; i < num_subjects; i++) {
    sorted[i] = -1;
  }

  int *remaining_prereqs = (int *)malloc(sizeof(int) * num_subjects);
  int **dependents = (int **)malloc(sizeof(int *) * (num_subjects));
  int *num_dependents = (int *)malloc(sizeof(int) * num_subjects);

  for (int i = 0; i < num_subjects; i++) {
    remaining_prereqs[i] = major[i].num_prereqs;

    dependents[i] = (int *)malloc(sizeof(int) * (num_subjects));
    num_dependents[i] = 0;
  }

  for (int i = 0; i < num_subjects; i++) {
    subject_t *subject = &major[i];
    for (int j = 0; j < subject->num_prereqs; j++) {
      int idx = get_code_index(subject->pre_req[j], major, num_subjects);
      dependents[idx][num_dependents[idx]] = i;
      num_dependents[idx]++;
    }
  }

  int subject_idx = find_zero(remaining_prereqs, num_subjects);
  while (subject_idx != -1) {
    remaining_prereqs[subject_idx] = -1;
    sorted[sorted_n] = subject_idx;
    sorted_n++;

    for (int i = 0; i < num_dependents[subject_idx]; i++) {
      int idx = dependents[subject_idx][i];
      remaining_prereqs[idx]--;
    }

    subject_idx = find_zero(remaining_prereqs, num_subjects);
  }

  int has_err = 0;
  for (int i = 0; i < num_subjects; i++) {
    if (sorted[i] == -1) {
      has_err = 1;
    }
  }

  if (has_err) {
    fprintf(stdout, "Pre-requisites impossible - check for cycles or missing "
                    "pre-requisites\n");
  } else {
    fprintf(stdout, "Sorted Order:\n");
    for (int i = 0; i < num_subjects; i++) {
      fprintf(stdout, "%s\n", major[sorted[i]].code);
    }
  }

  for (int i = 0; i < num_subjects; i++) {
    free(dependents[i]);
  }
  free(dependents);
  free(num_dependents);
  free(sorted);
  free(remaining_prereqs);
  return;
}

void free_major(subject_t *major, int num_subjects) {
  for (int i = 0; i < num_subjects; i++) {
    // Note: this set, j, will be empty after Kahn's algorithm
    for (int j = 0; j < major[i].num_prereqs; j++) {
      free(major[i].pre_req[j]);
    }
    free(major[i].pre_req);
  }
  free(major);
}

/* =====================================================================
   This skeleton/program is compiled by the comp20007 teaching team,
   the university of Melbourne.
   Last Updated: 16/02/2026
   ================================================================== */
