#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "include/lexer.h"

typedef enum {
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,
  TOK_LETTER,
  TOK_NUMBER,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_ERROR,
  TOK_UNDEFINED
} token_t;

typedef struct {
  char value[50];
  token_t type;
  int filled_space;
} token;

token token_a[256];
int token_a_idx = 0;

void lexer(char* sampleData, int sampleDataLen);
token_t token_type_finder(char ch);
void flush_token(token temp_token);
void print_token_a();

int main(void) {
  char sampleData[] = "{24+2} + 123abc + 5 + abc123 + 5";
  int len = strlen(sampleData);
  lexer(sampleData, len);
  print_token_a();
}

token_t token_type_finder(char ch) {
  if (isalpha(ch)) return TOK_LETTER;
  if (isdigit(ch)) return TOK_NUMBER;
  if (ch == '+') return TOK_ADD;
  if (ch == '-') return TOK_SUB;
  if (ch == '*') return TOK_MUL;
  if (ch == '/') return TOK_DIV;
  if (ch == '{') return TOK_LPAREN;
  if (ch == '}') return TOK_RPAREN;
  return TOK_UNDEFINED;
}
bool check_is_seprator(char sampleData) {
  token_t t = token_type_finder(sampleData);
  if (t == TOK_UNDEFINED) return true;
  if (t == TOK_ADD) return true;
  if (t == TOK_DIV) return true;
  if (t == TOK_MUL) return true;
  if (t == TOK_LPAREN) return true;
  if (t == TOK_RPAREN) return true;
  if (t == TOK_NUMBER) return true;
  if (t == TOK_LETTER) return true;
  return false;
}
void lexer(char* sampleData, int sampleDataLen) {
  token temp_token;
  temp_token.type = token_type_finder(sampleData[0]);
  temp_token.filled_space = 0;
  bool skip_flush;
  for (int i = 0; i < sampleDataLen; i++) {
    skip_flush = false;
    if (token_type_finder(sampleData[i]) != temp_token.type) {  // TOken change

      // Transiton check;

      // 123abc
      if ((token_type_finder(sampleData[i]) == TOK_LETTER &&
           temp_token.type == TOK_NUMBER)) {
        temp_token.type = TOK_ERROR;
        skip_flush = true;
      }
      if (token_type_finder(sampleData[i]) == TOK_NUMBER &&
          temp_token.type == TOK_LETTER) {
        skip_flush = true;
      }
      if (temp_token.type == TOK_ERROR &&
          (token_type_finder(sampleData[i]) == TOK_LETTER ||
           token_type_finder(sampleData[i]) == TOK_NUMBER))
        skip_flush = true;
      // FLush token in token array

      if (skip_flush == false && check_is_seprator(sampleData[i])) {
        flush_token(temp_token);
        temp_token.type = token_type_finder(sampleData[i]);
        temp_token.filled_space = 0;
      }
    }
    temp_token.value[temp_token.filled_space++] = sampleData[i];
  }
  // FLush last token
  flush_token(temp_token);
}
void flush_token(token temp_token) {
  temp_token.value[temp_token.filled_space] = '\0';

  token_a[token_a_idx++] = temp_token;
}
void print_token_a() {
  for (int i = 0; i < token_a_idx; i++) {
    token t1 = token_a[i];
    printf("Value:%s ", t1.value);
    if (t1.type == TOK_DIV) printf("TOK_DIV");
    if (t1.type == TOK_UNDEFINED) printf("TOK_UNDEFINED");
    if (t1.type == TOK_ADD) printf("TOK_ADD");
    if (t1.type == TOK_LETTER) printf("TOK_LETTER");
    if (t1.type == TOK_NUMBER) printf("TOK_NUMBER");
    if (t1.type == TOK_MUL) printf("TOK_MUL");
    if (t1.type == TOK_SUB) printf("TOK_SUB");
    if (t1.type == TOK_LPAREN) printf("TOK_LPAREN");
    if (t1.type == TOK_RPAREN) printf("TOK_RPAREN");
    if (t1.type == TOK_ERROR) printf("TOK_ERROR");
    printf("\n");
  }
  printf("\n");
}
