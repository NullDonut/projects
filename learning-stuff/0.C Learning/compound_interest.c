#include <math.h>
#include <stdio.h>

int main() {
  double principal = 0.0;
  double rate = 0.0;
  int t = 0;
  int no = 0;
  double t_amount = 0.0;
  printf("Enter the principal (P): ");
  scanf("%lf", &principal);
  printf("Enter the interest rate (r): ");
  scanf("%lf", &rate);
  rate = rate / 100;
  printf("Enter the # of years (t): ");
  scanf("%d", &t);
  printf("Enter # of times compunded per year (n): ");
  scanf("%d", &no);

  t_amount = principal * pow((1 + (rate / no)), (no * t));
  double volume1 = 0.0;
  printf("\nAfter %d years, the total will be $%.2lf\n", t, t_amount);

  return 0;
}
