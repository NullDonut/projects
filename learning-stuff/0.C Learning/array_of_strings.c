#include <stdio.h>
#include <string.h>

int main() {
  char name[3][25] = {0};
  printf("Enter a name:");
  fgets(name[0], sizeof(name[0]), stdin);
  name[0][strlen(name[0]) - 1] = '\0';
  printf("%s", name[0]);
}
