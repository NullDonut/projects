#include "raylib.h"
int main(int argc, char *argv[]) {
  int dataSize = 0;
  unsigned char *data = LoadFileData("./compressed.txt", &dataSize);

  int compressed_size = 0;
  unsigned char *compressed =
      DecompressData((const unsigned char *)data, dataSize, &compressed_size);
  SaveFileData("./decompressed.txt", compressed, compressed_size);
  UnloadFileData(data);
  MemFree(compressed);
  return 0;
}
