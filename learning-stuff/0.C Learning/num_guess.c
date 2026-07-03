#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
  srand(time(NULL));
  int max = 100;
  int min = 1;
  int randomNum = (rand() % (max - min)) + min;
  int tries = 0;
  while (1) {
    int n;
    tries++;
    printf("Guess the number between 1-100: ");
    scanf("%d", &n);
    if (randomNum - n > 20) {
      printf("Too Low!\n");
      continue;
    } else if (n - randomNum > 20) {
      printf("Too High!\n");
      continue;
    } else if (randomNum - n > 0) {
      printf("Low!\n");
      continue;
    } else if (n - randomNum > 0) {
      printf("High!\n");
    } else {
      printf("CORRECT!!\n");
      printf("The correct answer is %d\n", randomNum);
      printf("It took you %d tries. \n", tries);
      exit(0);
    }
  }
}
