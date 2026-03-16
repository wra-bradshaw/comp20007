#include <stdio.h>

int main(void) {
  int i, num, j;
  printf("Enter the number: ");
  if (scanf("%d", &num) != 1)
    return 1;

  for (i = 1; i < num; i++)
    j = j * i;

  printf("The factorial of %d is %d\n", num, j);
}
