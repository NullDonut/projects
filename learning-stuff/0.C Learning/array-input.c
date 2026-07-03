#include <stdio.h>
int main(int argc, char *argv[]) {
  int scores[5] = {0};
  for (int i = 0; i < (sizeof scores / sizeof scores[0]); i++) {
    printf("Element at %d: ", i);
    scanf(" %d", &scores[i]);
  }

  for (int i = 0; i < (sizeof scores / sizeof scores[0]); i++) {
    printf("%d, ", scores[i]);
  }

  return 0;
}
