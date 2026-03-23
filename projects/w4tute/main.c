#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char *argv[]) { return 0; }

int subset_sum(int *set, int *out, size_t i, size_t j, size_t len,
               int desired_sum, int current_sum) {
  if (current_sum == desired_sum) {
    return 1;
  }
  if (current_sum > desired_sum || i >= len) {
    return 0;
  }

  out[j] = set[i];
  return subset_sum(set, out, i + 1, j + 1, len, desired_sum,
                    current_sum + set[i]) ||
         subset_sum(set, out, i + 1, j, len, desired_sum, current_sum);
}

int partition_problem(int *set, size_t set_size, int *s1, size_t s1_size,
                      int s1_sum, size_t s1_i, int *s2, size_t s2_size,
                      int s2_sum, int s2_i, size_t i) {
  if (i >= set_size) {
    return s1_sum == s2_sum;
  }

  s1[s1_i] = set[i];
  s2[s2_i] = set[i];

  return partition_problem(set, set_size, s1, s1_size + 1, s1_sum + set[i],
                           s1_i + 1, s2, s2_size, s2_sum, s2_i, i + 1) ||
         partition_problem(set, set_size, s1, s1_size, s1_sum, s1_i, s2,
                           s2_size + 1, s2_sum + set[i], s2_i + 1, i + 1);
}
