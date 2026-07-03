#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void read_file(int fd);
int main() {
  int fd = open("./a.txt", O_RDWR, 0);
  if (fd < 0) {
    printf("File not found.");
    exit(-1);
  }
  printf("File descriptor: %d\n", fd);
}
void read_file(int fd) {
  char a[512];
  int size = sizeof(a) - 1;
  read(fd, a, size);
  printf("%s\n", a);
}
void open_file(int fd) { open(const char *_Nonnuld); }
