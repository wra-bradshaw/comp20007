/* * * * * * *
 * Program for counting and plotting collisions for various hash functions
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strhash.h"

#define MAX_WORD_LEN 128
#define SCREEN_WIDTH 80
#define GRAPH_SYMBOL 'o'


int main(int argc, char **argv) {

	// get table size and hashing method
	if (argc < 3) {
		fprintf(stderr, "usage: %s <size> <hash>\n", argv[0]);
		fprintf(stderr, " size: positive integer size of hash table\n");
		fprintf(stderr, " hash: a character specifying the function to use\n");
		fprintf(stderr, "  '0' - hash everything to zero\n");
		fprintf(stderr, "  'r' - hash everything to random (bad idea!)\n");
		// add descriptions of your own hash functions here
		// ...

		return 1;
	}
	int size = atoi(argv[1]);
	if (size < 1) {
		fprintf(stderr, "table size must be a positive integer\n");
		return 1;
	}
	char method = argv[2][0];

	// initialise hash table
	printf("initialising table of size %d...\n", size);

	int *table = malloc(size * (sizeof *table));
	if (table == NULL) {
		fprintf(stderr, "can't allocate space for table of size %d\n", size);
		return 1;
	}
	int i;
	for (i = 0; i < size; i++) {
		table[i] = 0;
	}

	// read unique words from stdin, hash them into our table (counting
	// only the number of words in each cell, not actually storing them)
	printf("reading and hashing string keys from stdin...\n");

	int max_count = 0, max_address = 0, total = 0, used = 0;
	char word[MAX_WORD_LEN];
	while (fgets(word, MAX_WORD_LEN, stdin)) {
		word[strlen(word)-1] = '\0'; // strip newline

		// calculate the hash value
		unsigned int address = hash(word, size, method);
		
		// increment the count of the number of times this address was generated
		table[address] += 1;
		
		// record some stats
		total++;
		if (table[address] > max_count) {
			max_count = table[address];
			max_address = address;
		}
		if (table[address] == 1) {
			used++; // record first time this address was used
		}
	}

	// print some stats about the distribution:
	printf("\nfinished hashing %d keys\n", total);
	printf("hash function used was '%s'\n", name(method));
	printf("used %d of %d addresses (%d unused)\n", used, size, size-used);
	printf("max: %d keys hashed to address %d\n", max_count, max_address);
	printf("distribution:\n");

	// print the resulting distribution as a (horizontal) bar graph

	int label_width = snprintf(NULL, 0, "%d", size-1);  // max strlen of address
	int count_width = snprintf(NULL, 0, "%d", max_count);// max strlen of counts
	int graph_width = SCREEN_WIDTH - (label_width+2) - (count_width+2) - 1;
	float scale_factor = graph_width * 1.0 / max_count;
	scale_factor = scale_factor < 1 ? scale_factor : 1;
	
	int j;
	for (j = 0; j < size; j++) {
		printf(" %*d|", label_width, j);
		int len = (int) (scale_factor * table[j]);
		int k;
		for (k = 0; k < len; k++) {
			printf("%c", GRAPH_SYMBOL);
		}
		printf(" (%d)", table[j]);
		printf("\n");
	}

	free(table);
	return 0;
}