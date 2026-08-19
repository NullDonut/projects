#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef enum {
  NONE,
  WHITESPACE,
  ADDITION,
  SUBSTRACTION,
  PRODUCT,
  DIVISION,
  NUMBER
} available_types;

typedef struct {
  char value[50];
  int value_idx;
  available_types type;
} token;

// Tokenization Functions
void tokenizer(char* s, size_t string_len);
void add_token_to_tk_array(token t1);
available_types check_type(char c);
void clear_string(token* t);
void print_tk_array();

// Error Checking Functions
void tk_error_checker();

token tk_array[256];
int tk_array_idx = 0;

int main(void) {
  char s[] = "123+234+4-5+";
  size_t string_len = strlen(s);

  tokenizer(s, string_len);
  print_tk_array();
  tk_error_checker();
  return 0;
}

available_types check_type(char c) {
  if (c == ' ') {
    return WHITESPACE;
  }
  if (isdigit(c)) {
    return NUMBER;
  }
  if (c == '+') {
    return ADDITION;
  }
  if (c == '-') {
    return SUBSTRACTION;
  }
  if (c == '*') {
    return PRODUCT;
  }
  if (c == '/') {
    return DIVISION;
  }
  return NONE;
};
void add_token_to_tk_array(token t1) { tk_array[tk_array_idx++] = t1; }
void print_tk_array() {
  for (int i = 0; i < tk_array_idx; i++) {
    token t1 = tk_array[i];
    printf("value:%s  type:", t1.value);
    if (t1.type == WHITESPACE) {
      printf("WHITESPACE");
    }
    if (t1.type == NUMBER) {
      printf("DIGIT");
    }

    if (t1.type == ADDITION) {
      printf("ADDITION");
    }
    if (t1.type == SUBSTRACTION) {
      printf("SUBSTRACTION");
    }
    if (t1.type == PRODUCT) {
      printf("PRODUCT");
    }
    printf("\n");
  }
}
void tokenizer(char* s, size_t string_len) {
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
      add_token_to_tk_array(temp_token);
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
  // Add the remaining content to token array after the loop ends abruptly
  add_token_to_tk_array(temp_token);
}
void tk_error_checker() {
  for (int i = 0; i < tk_array_idx; i++) {
    token temp_token = tk_array[i];
    bool isTokenArthematic =
        (temp_token.type == ADDITION || temp_token.type == SUBSTRACTION ||
         temp_token.type == DIVISION || temp_token.type == PRODUCT);
    if (isTokenArthematic) {
      size_t val_len = temp_token.value_idx;
      if (val_len != 1) {
        printf("[Error] Arthimetic ordering error..\n");
        exit(-1);
      }
    }
  }
}
