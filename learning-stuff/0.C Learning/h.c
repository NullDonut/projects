#include <stdio.h>
#include <string.h>
int main(void) {
  int age = 0;
  float gpa = 0.0;
  char grade = '\0'; // A,B,C,D,E
  char name[30] = "";
  printf("Enter your age: ");

  scanf(" %d", &age);
  printf("Enter your gpa: ");
  scanf(" %f", &gpa);
  printf("Enter your grade: ");
  scanf(" %c", &grade);
  getchar();
  printf("Enter your name: ");
  fgets(name, sizeof(name), stdin);
  name[strlen(name - 1)] = '\0';

  printf("%d\n", age);
  printf("%f\n", gpa);
  printf("%s\n", name);

  printf("Hello World!\n");
  return 0;
}
