#include <raylib.h>
#include <stddef.h>

int main(void) {
  char data[] = "ddddddddnwdnjsfjjfbeshkfchhvdjkb";
  char* compdata;
  int compressedSize;

  CompressData(data, sizeof(data), &compressedSize);
  return 0;
}
