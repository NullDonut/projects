#include <stdio.h>
int memo[100] = {0};
int fibonacci(int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  if (memo[n] != 0)
    return memo[n];

  memo[n] = fibonacci(n - 1) + fibonacci(n - 2);
  return memo[n];
}
int main() {
  int terms = 15;
  for (int i = 0; i < terms; i++) {
    printf("%3d | ", fibonacci(i));
    for (int j = 0; j < fibonacci(i); j++) {
      printf("#");
    }

    printf("\n");
  }
  printf("\n");
  return 0;
}
