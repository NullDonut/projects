#include <stdio.h>
enum Day { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };
int main(int argc, char *argv[]) {
  enum Day today = SUNDAY;
  printf("%d\n", today);
}
