#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
  char* ptr;
  int size;
  int used;
} String;

String init_array(int base_size);
void resize_array(String* str, int new_size);
void print_status(String str);
void append(String* str, char value);

String newString(char* string);
void appendString(String* s, char* string);

int main(void) {
  String s1 = newString("My name is Shivansh");
  print_status(s1);
  appendString(&s1, " Bansal");
  print_status(s1);
  free(s1.ptr);
}

String init_array(int base_size) {
  String str;
  str.ptr = (char*)calloc(base_size, sizeof(char));
  str.used = 0;
  str.size = base_size;
  return str;
}
void print_status(String str) {
  printf("String size is:%d and used is:%d\n", str.size, str.used);
  for (int i = 0; i < str.size; i++) {
    if (str.ptr[i] == '\0') {
      printf("%d. \\0\n", i + 1);
      continue;
    }
    printf("%d. %c\n", i + 1, str.ptr[i]);
  }
}
void resize_array(String* str, int new_size) {
  str->ptr = (char*)realloc(str->ptr, sizeof(char) * new_size);
  if (str->ptr == NULL) {
    printf("Error occured in reAlllocation..\n");
    exit(-1);
  }
  for (int i = str->size; i < new_size; i++) {
    str->ptr[i] = 0;
  }
  str->size = new_size;
}
void append(String* str, char value) {
  if (str->size == str->used + 1) {
    resize_array(str, str->size * 2);
  }
  str->ptr[str->used++] = value;
  str->ptr[str->used] = '\0';
}
String newString(char* string) {
  String s1 = init_array(strlen(string) + 1);
  s1.used = strlen(string);

  strcpy(s1.ptr, string);
  return s1;
}
void appendString(String* s, char* string) {
  int len = strlen(string);
  for (int i = 0; i < len; i++) {
    append(s, string[i]);
  }
}
