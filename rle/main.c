#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
void compression(char fileName[]);

int main(int clen, char* cargs[]) {
  if (clen != 3) {
    printf("Usage:\n\trle (compress/decompress) <filename>\n");
    exit(-1);
  }

  if (strcmp(cargs[1], "compress") != 0 &&
      strcmp(cargs[1], "decompress") != 0) {
    printf("Usage:\n\trle (compress/decompress) <filename>\n");
    exit(-1);
  }

  if (cargs[1][0] == 'c') {
    printf("Compression....\n");
    compression(cargs[2]);
  }
  if (cargs[1][0] == 'd') {
    printf("decompression......\n");
  }
  return 0;
}
void a() {}
void compression(char fileName[]) {
  char compressedFileName[256];
  snprintf(compressedFileName, sizeof(compressedFileName), "%s.rle", fileName);
  printf("Compresed File Name: %s\n", compressedFileName);
  FILE* inputfp = fopen(fileName, "r");
  FILE* outputfp = fopen(compressedFileName, "w");
  if (outputfp == NULL || inputfp == NULL) {
    printf("Error in opening file..");
  }
  int c = 0;
  int counter = 0;
  int tmp = fgetc(inputfp);
  while ((c = fgetc(inputfp)) != EOF) {
    if (c != tmp) {
      printf("The character is %c and the count is %d\n", (char)tmp, counter);
      fprintf(outputfp, "%c%d", (char)tmp, counter);
      counter = 0;
    }
    tmp = c;
    ++counter;
  }
}
