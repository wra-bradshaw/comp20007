/* * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * additional hash function implementations by ...
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strhash.h"

#define ARRAY_SIZE 128

// hash everything to a random value (mod size)
unsigned int random_hash(const char *key, unsigned int size) {
  return rand() % size;
}

// hash everything to zero
unsigned int zero_hash(const char *key, unsigned int size) { return 0; }

unsigned int first_char_hash(const char *key, unsigned int size) {
  return *key % size;
}

unsigned int length_hash(const char *key, unsigned int size) {
  return strlen(key) % size;
}

unsigned int universal_hash(const char *key, unsigned int size) {
  static int randomArray[ARRAY_SIZE];
  static int isInitialised = 0;

  if (!isInitialised) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
      randomArray[i] = rand() % 100;
    }
    isInitialised = 1;
  }

  int sum = 0;
  int i = 0;
  while (key[i] != '\0') {
    sum += randomArray[i] * key[i];
    i += 1;
  }

  return sum % size;
}

// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size, char method) {
  switch (method) {
  // really bad hash functions:
  case '0':
    return zero_hash(key, size);
  case 'r':
    return random_hash(key, size);
  case 'f':
    return first_char_hash(key, size);
  case 'u':
    return universal_hash(key, size);
  case 'l':
    return length_hash(key, size);

    // once you have implemented your own hash functions,
    // add cases to this function to access them from main.c

  default:
    fprintf(stderr, "unknown hashing method '%c'\n", method);
    exit(1);
  }
}

// return the string name of a hash function specified by method
char *name(char method) {
  switch (method) {
  case '0':
    return "zero hash";
  case 'r':
    return "random hash";

    // once you have implemented your own hash functions,
    // add cases to this function to access their names from main.c

  default:
    return "unknown";
  }
}
