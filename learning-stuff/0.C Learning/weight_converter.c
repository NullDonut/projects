#include <stdio.h>

int main() {
  int choice = 0;
  double kilo = 0.0;
  double pounds = 0.0;
  printf("Weight Conversion Calculator\n");
  printf("1. Kilogram to Pounds \n");
  printf("2. Pounds to Kilogram \n");
  printf("Enter your choice (1 or 2): ");
  scanf("%d", &choice);
  if (choice == 1) {
    printf("Enter kilograms: ");
    scanf("%lf", &kilo);
    pounds = kilo * 2.20462;
    printf("Pounds: %.3lf\n", pounds);

  } else if (choice == 2) {
    printf("Enter pounds: ");
    scanf("%lf", &pounds);
    kilo = pounds * 0.453592;
    printf("Kilograms: %.3lf\n", kilo);
  } else {
    printf("Wrong Choice!");
  }
  return 0;
}
