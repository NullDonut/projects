#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int pass_len = 0;

void input_pass_len();
int num_pass_gen(int max, int min);
int char_pass_gen();
void pass_loop(int pl);
char convert_ascii_to_char_pass(int n[]);

int main() {

  srand(time(NULL));
  input_pass_len();
  pass_loop(pass_len);

  return 0;
}

void input_pass_len() {
  printf("enter the password length: ");
  scanf(" %d", &pass_len);
}
void pass_loop(int pl) {
  char a[pl];
  for (int i = 0; i < pass_len; i++) {
    a[i] = (char)(char_pass_gen());
    printf("%c", a[i]);
  }
}
int char_pass_gen() {
  int n = num_pass_gen(126, 33);
  return n;
}
int num_pass_gen(int max, int min) {
  int rand_num = (rand() % (max + 1 - min)) + min;

  return rand_num;
}
