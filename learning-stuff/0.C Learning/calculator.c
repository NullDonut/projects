#include <stdio.h>
#include <stdlib.h>
int main() {
  double n1 = 0.0;
  double n2 = 0.0;
  char choice = '\0';
  double result = 0.0;
  printf("Enter the first number: ");
  scanf(" %lf", &n1);
  getchar();
  printf("Enter the operator(+-/*): ");
  choice = getchar();
  printf("Enter the second number: ");

  scanf(" %lf", &n2);
  switch (choice) {
  case '+':
    result = n1 + n2;
    break;
  case '-':
    result = n1 - n2;
    break;
  case '*':
    result = n1 * n2;
    break;
  case '/':
    result = n1 / n2;
    break;
  default:
    printf("Wrong Choice\n");

    exit(0);
  }
  printf("%.1lf %c %.1lf = %.1lf\n", n1, choice, n2, result);
}
