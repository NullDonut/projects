#include <math.h>
#include <stdio.h>

int main() {
  double radius = 0.0;
  double area = 0.0;
  double surfaceArea = 0.0;
  double volume = 0.0;
  const double PI = 3.14159;

  printf("Enter the radius: ");
  scanf("%lf", &radius);
  surfaceArea = (4) * (PI * pow(radius, 3));
  volume = surfaceArea / 3.0;
  area = PI * pow(radius, 2);

  printf("Area: %.2lf \n", area);
  printf("Surface Area: %.2lf\n", surfaceArea);
  printf("Volume: %.2lf \n", volume);
}
