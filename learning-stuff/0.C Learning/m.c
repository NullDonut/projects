#include <stdio.h>
// Palindrome Program
int main() {
  int i = 291;
  int temp = i;
  int sum = 0;
  while (temp > 0) {
    int digit = temp % 10;
    sum = sum * 10 + digit;
    temp = temp / 10;
  }
  if (sum == i) {
    printf("The number %d is Palindrome\n", i);

  } else {
    printf("The numer %d is not Palindrome\n", i);
  }
}
