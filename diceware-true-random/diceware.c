#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

#define MAX_DICEROLLS 5
#define NO_OF_WORDS 7

int main(void) {
  unsigned char buf[MAX_DICEROLLS];
  int n_array[NO_OF_WORDS];
  char results[NO_OF_WORDS][64];
  int n_array_idx = 0;
  for (int j = 0; j < NO_OF_WORDS; j++) {
    int sum = 0;
    RAND_bytes(buf, MAX_DICEROLLS);
    for (int i = 0; i < MAX_DICEROLLS; i++) {
      int n = buf[i] % 6 + 1;
      sum = sum * 10 + n;
    }
    n_array[n_array_idx++] = sum;
    memset(buf, 0, sizeof(buf));
  }
  FILE* fd = fopen("./wordlist.txt", "r");
  char b[64];
  while (fgets(b, sizeof(b), fd) != NULL) {
    int key;
    char word[64];
    if (sscanf(b, "%d %63s", &key, word) != 2) continue;
    for (int k = 0; k < NO_OF_WORDS; k++) {
      if (n_array[k] == key) {
        strcpy(results[k], word);
      }
    }
  }
  for (int i = 0; i < NO_OF_WORDS; i++) {
    printf("%d %s\n", n_array[i], results[i]);
  }
  return 0;
}
