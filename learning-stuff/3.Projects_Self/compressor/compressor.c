#include "raylib.h"

int main(int argc, char *argv[]) {
  int dataSize = 0;
  unsigned char *data = LoadFileData("./m.txt", &dataSize);
  int compressed_size = 0;
  unsigned char *compressed =
      CompressData((const unsigned char *)data, dataSize, &compressed_size);
  SaveFileData("./compressed.txt", compressed, compressed_size);
  UnloadFileData(data);
  MemFree(compressed);
  return 0;
}
