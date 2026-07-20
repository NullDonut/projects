#include <stdio.h>
#include <stdlib.h>

#define WIDTH 360
#define HEIGHT 360

int main(void) {
  system("rm -rf output.ppm");
  FILE* fp = fopen("output.ppm", "wb");

  fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

  for (int y = 0; y < HEIGHT; y += 2) {
    for (int x = 0; x < WIDTH; x += 2) {
      unsigned char pixel[3];
      if ((x) % 2 == 0) {
        pixel[0] = 255;
        pixel[1] = 255;
        pixel[2] = 255;
      } else {
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
      }
      fwrite(pixel, sizeof(unsigned char), sizeof(pixel), fp);
    }
  }
  fclose(fp);
  return 0;
}
