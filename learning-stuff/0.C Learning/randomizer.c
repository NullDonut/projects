// This is pseudo generator not good
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL)); // Use current time as seed
  int max = 100;
  int min = 20;

  int randomNum = (rand() % (max - min)) + min;
  printf("%d\n", randomNum);
  printf("Hi \n");
}
