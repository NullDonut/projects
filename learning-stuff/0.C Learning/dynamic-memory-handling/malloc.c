#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct int_array {
  int* ptr;
  int size;
  int used;
};
struct int_array create_array(int base_size);
void resize_array(struct int_array* ar, int new_size);
void print_status(struct int_array i1);
void append(struct int_array* i1, int value);

int main(void) {
  struct int_array i1 = create_array(5);

  print_status(i1);
  append(&i1, 2);
  append(&i1, 2);
  append(&i1, 2);
  append(&i1, 2);
  append(&i1, 2);
  append(&i1, 2);
  print_status(i1);
  free(i1.ptr);
}

void append(struct int_array* i1, int value) {
  if (i1->used == i1->size) {
    resize_array(i1, i1->used * 2);
  }
  i1->ptr[i1->used] = value;
  i1->used += 1;
}
struct int_array create_array(int base_size) {
  struct int_array ar;
  ar.size = base_size;
  ar.ptr = (int*)calloc(ar.size, sizeof(int));
  if (ar.ptr == NULL) {
    printf("Memory Allocation failed");
    exit(-1);
  }
  ar.used = 0;
  return ar;
}
void resize_array(struct int_array* ar, int new_size) {
  ar->ptr = (int*)realloc(ar->ptr, sizeof(int) * new_size);

  if (ar->ptr == NULL) {
    printf("Memory Re-Allocation failed");
    exit(-1);
  }
  for (int i = ar->size; i < new_size; i++) {
    ar->ptr[i] = 0;
  }
  ar->size = new_size;
}
void print_status(struct int_array i1) {
  printf("The current size of array: %d and used is: %d\n", i1.size, i1.used);

  for (int i = 0; i < i1.size; i++) {
    printf("%d. %d\n", i + 1, i1.ptr[i]);
  }
}
