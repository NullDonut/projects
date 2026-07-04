#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char c;
  int i;
} token;
token TokenList[256];
int TokenListIdx = 0;
void tokenchecker(char s[256]) {
  size_t len = strlen(s);
  char c = s[0];
  int l = 0;
  for (int i = 0; i < len; i++) {
    if (c != s[i]) {
      token tok;
      tok.c = c;
      tok.i = l;

      TokenList[TokenListIdx] = tok;
      TokenListIdx++;

      c = s[i];
      l = 0;
      printf("c: %c , i: %d\n", tok.c, tok.i);
    }
    l++;
  }
  token tok;
  tok.c = c;
  tok.i = l;

  TokenList[TokenListIdx] = tok;
  TokenListIdx++;
  printf("c: %c , i: %d\n", tok.c, tok.i);
}
void tkListtoStr() {
  printf("Length of tokenList is: %d\n", TokenListIdx);
  char s[TokenListIdx * 2 + 1];
  int sIdx = 0;
  for (int i = 0; i < TokenListIdx; i++) {
    token t = TokenList[i];
    s[sIdx] = t.c;
    sIdx++;
    s[sIdx] = t.i + '0';
    sIdx++;
  }
  s[sIdx] = '\0';
  printf("Encrypted form: %s\n", s);
}
int main(void) {
  char a[256] = "aaabbbbbccccccdffffd";
  printf("Current String: %s\n", a);
  tokenchecker(a);
  tkListtoStr();
}
