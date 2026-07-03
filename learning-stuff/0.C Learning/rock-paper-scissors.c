// Arrays

#include <stdio.h>

int main() {
  int numbers[] = {10, 20, 30, 40, 0};
  int size = (sizeof numbers / sizeof numbers[0]);
  printf("SIze: %d\n", size);
  for (int i = 0; i < size; i++) {
    printf("%d ", numbers[i]);
  }
}
