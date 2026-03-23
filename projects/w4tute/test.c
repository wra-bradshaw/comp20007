#include <criterion/criterion.h>
#include <stdio.h>

#define main w4tute_main
#include "main.c"
#undef main

Test(subset_sum, finds_matching_subset) {
  int set[] = {3, 34, 4, 12, 5, 2};
  int out[6] = {0};
  subset_sum(set, out, 0, 0, 6, 9, 0);

  fprintf(stdout, "%d %d %d %d %d %d", out[0], out[1], out[2], out[3], out[4],
          out[5]);

  cr_assert(subset_sum(set, out, 0, 0, 6, 9, 0));
}
