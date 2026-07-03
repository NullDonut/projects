
#include <ctype.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
  char question[][100] = {"What is largest planet in the solarsystem\n",
                          "What is hottest planet?\n"};
  char options[][100] = {"A.Jupiter\nB.Saturn\nC.Earth\nD.Sun\n",
                         "A.Jupiter\nB.Venus\nC.Saturn\nD.Sun\n"};
  char answer_key[] = {'A', 'B'};
  int size = (sizeof question / sizeof question[0]);
  char guess = '\0';
  int score = 0;

  for (int i = 0; i < size; i++) {
    printf("%s\n", question[i]);
    printf("%s\n", options[i]);
    printf("\nEnter your choice: ");
    scanf(" %c", &guess);
    guess = toupper(guess);
    if (guess == answer_key[i]) {
      score++;
      printf("Correct\n");

    } else {
      printf("Wrong\n");
      printf("The correct answer is , %c", answer_key[i]);
    }
  }
  printf("\nYour score is %d out of %d points.\n", score, size);
}
