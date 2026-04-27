/* * * * * * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * 
 * additional hash function implementations by ...
 *
 * available hash functions:
 * method | description
 * --------------------
 *   'o'  | hash everything to zero
 *   'r'  | hash to a random value
 *   ...  | ...
 * (add any new hash functions here)
 */


// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size, char method);

// return the string name of a hash function specified by method
char *name(char method);
