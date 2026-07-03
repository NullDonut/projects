#include <stdio.h>
#include <string.h>
int main() {
  char name[30] = "";
  float price = 0.0f;
  int quantity = 0;
  char currency = '$';
  float total = 0.0f;
  printf("What item would like to buy: ");
  fgets(name, sizeof(name), stdin);
  name[strlen(name) - 1] = '\0';
  printf("quantity: ");
  scanf(" %d", &quantity);
  printf("Price of each item: ");
  scanf(" %f", &price);
  printf("Name of the item is %s", name);
  printf("\nPrice: %f", price);
  printf("\nQuantity: %d", quantity);
  total = price * quantity;
  printf("\nTotal:%c%.2f\n", currency, total);
  return 0;
}
