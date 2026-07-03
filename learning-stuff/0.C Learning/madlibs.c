#include <stdio.h>
#include <string.h>

int main() {
  char ad[30] = "";
  char noun[30] = "";
  char verb[30] = "";
  char ad2[30] = "";
  char ad3[30] = "";
  printf("Enter a adjective (description): ");
  fgets(ad, sizeof(ad), stdin);
  printf("Enter a noun (animal or person): ");
  fgets(noun, sizeof(noun), stdin);
  printf("Enter a adjective (description): ");
  fgets(ad2, sizeof(ad2), stdin);
  printf("Enter a verb (end w/ -ing): ");
  fgets(verb, sizeof(verb), stdin);
  printf("Enter a adjective (description): ");
  fgets(ad3, sizeof(ad3), stdin);

  ad[strlen(ad) - 1] = '\0';
  noun[strlen(noun) - 1] = '\0';
  verb[strlen(verb) - 1] = '\0';
  ad2[strlen(ad2) - 1] = '\0';
  ad3[strlen(ad3) - 1] = '\0';

  printf("\nToday I went to a %s zoo.\n", ad);
  printf("In an exibhit, I saw a %s.\n", noun);
  printf("%s was %s and %s!\n", noun, ad2, verb);
  printf("I was %s!\n", ad3);

  return 0;
}
