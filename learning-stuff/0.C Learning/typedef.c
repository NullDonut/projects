#include <stdio.h>

typedef int Number;
typedef char String[50][80];

int main(int argc, char *argv[]) {
  Number x = 10;
  Number y = 20;
  Number z = 30;
  String a = {"Sam", "Harry"};
  printf("%s\n", a[0]);
}
