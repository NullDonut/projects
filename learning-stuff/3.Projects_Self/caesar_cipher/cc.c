#include <complex.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
int change_factor = 7;
void caesar_cipher(char a[256]);
int main(int argc, char *argv[]) {
  char c[256] = "";
  printf(">> ");
  fgets(c, sizeof(c), stdin);
  c[strlen(c) - 1] = '\0';
  printf("\nLine:%s\n", c);
  caesar_cipher(c);
}
void caesar_cipher(char a[256]) {
  char temp[256] = "";
  for (int i = 0; i < strlen(a); i++) {
    temp[i] = a[i];
    if (temp[i] == ' ') {
      printf(" ");
      continue;
    }
    temp[i] = tolower(temp[i]);
    int n = (int)temp[i];
    int new_n = n + change_factor;
    if (new_n > 122) {
      new_n = new_n - 26;
    }

    printf("%c", (char)new_n);
  }
}
