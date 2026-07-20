#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char encoding(char ch, int shiftValue);
char decoding(char enc_ch, int shiftValue);

int main(void) {
  char string[256] = "Mjqqt";
  int strLen = strlen(string);

  printf("String is: %s\nStrlen is:%d\n", string, strLen);

  char finalString[strLen];
  for (int i = 0; i < strLen; i++) {
    char enc_ch = decoding(string[i], 5);
    finalString[i] = enc_ch;
  }

  printf("\n\nEncoded String: %s\n", finalString);
  return 0;
}
char encoding(char ch, int shiftValue) {
  // Initialization
  char encoded_ch = '\0';
  bool is_lower = false;  // Default is Uppercase
  int char_ascii = (int)ch;

  // Uppercase or Lowercase
  if (char_ascii >= 97) {
    is_lower = true;
  }
  // Fix the ascii according to case
  if (is_lower) {
    char_ascii -= 'a';
  } else {
    char_ascii -= 'A';
  }

  // Adding Shift
  int encoded_char_ascii = char_ascii + shiftValue;

  // Mod 26
  encoded_char_ascii =
      (encoded_char_ascii) ? encoded_char_ascii - 26 : encoded_char_ascii;
  encoded_char_ascii =
      (encoded_char_ascii) ? encoded_char_ascii + 26 : encoded_char_ascii;

  // Int to char

  encoded_ch =
      (char)((is_lower) ? encoded_char_ascii + 'a' : encoded_char_ascii + 'A');

  //  Basic info
  printf("Char:%c char_ascii:%d encoded_char_ascii:%d", ch, char_ascii,
         encoded_char_ascii);
  printf(" is_lower:%b", is_lower);
  printf(" Encoded Char: %c\n", encoded_ch);

  return encoded_ch;
}
char decoding(char enc_ch, int shiftValue) {
  char decoded_ch = '\0';
  bool is_lower = false;  // Default is Uppercase
  int enc_char_ascii = (int)enc_ch;

  // Uppercase or Lowercase
  if (enc_char_ascii >= 97) {
    is_lower = true;
  }
  // Fix the ascii according to case
  if (is_lower) {
    enc_char_ascii -= 'a';
  } else {
    enc_char_ascii -= 'A';
  }

  // Adding Shift
  int decoded_char_ascii = enc_char_ascii - shiftValue;
  // Mod 26
  decoded_char_ascii =
      (decoded_char_ascii) ? decoded_char_ascii - 26 : decoded_char_ascii;
  decoded_char_ascii =
      (decoded_char_ascii) ? decoded_char_ascii + 26 : decoded_char_ascii;

  // Int to char

  decoded_ch =
      (char)((is_lower) ? decoded_char_ascii + 'a' : decoded_char_ascii + 'A');

  //  Basic info
  printf("Char:%c char_ascii:%d decoded_char_ascii:%d", enc_ch, enc_char_ascii,
         decoded_char_ascii);
  printf(" is_lower:%b", is_lower);
  printf(" Decoded Char: %c\n", decoded_ch);
  return decoded_ch;
}
