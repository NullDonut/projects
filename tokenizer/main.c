#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef enum { NONE, WHITESPACE, WORD, NUMBER } available_types;

typedef struct {
  char value[50];
  int value_idx;
  available_types type;
} token;

void create_token(token t1);
available_types check_type(char c);
void clear_string(token* t);

int main(void) {
  char s[] = "My name is 123";
  size_t string_len = strlen(s);

  token temp_token;
  temp_token.type = check_type(s[0]);
  temp_token.value_idx = 0;
  bool type_changed = false;
  for (int i = 0; i < string_len; i++) {
    if (temp_token.type != check_type(s[i])) {
      type_changed = true;
    }
    if (type_changed) {
      type_changed = false;
      create_token(temp_token);
      temp_token.type = check_type(s[i]);
      temp_token.value_idx = 0;
      memset(temp_token.value, 0, sizeof(temp_token.value));
    }
    if (temp_token.value_idx >= 50) {
      printf("Buffer Overflow Error");
      // Will Handle it later with dynamic memory allocation
    }
    temp_token.value[temp_token.value_idx] = s[i];
    temp_token.value_idx++;
  }
  create_token(temp_token);
  return 0;
}

available_types check_type(char c) {
  if (c == ' ') {
    return WHITESPACE;
  }
  if (isdigit(c)) {
    return NUMBER;
  }
  if (isalpha(c)) {
    return WORD;
  }
  return NONE;
};
void create_token(token t1) {
  printf("value:%s  type:", t1.value);
  if (t1.type == WHITESPACE) {
    printf("WHITESPACE");
  }
  if (t1.type == NUMBER) {
    printf("DIGIT");
  }
  if (t1.type == WORD) {
    printf("WORD");
  }
  if (t1.type == NONE) {
    printf("NONE");
  }
  printf("\n");
}
