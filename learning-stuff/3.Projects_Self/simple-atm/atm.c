#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Simple ATM
//
//
int acc_balance = 0;
void menu();
void line();
char temp = '\0';
int main() {
  menu();

  return 0;
}

void deposit(int *acc_b) {
  printf("Amount to be deposited.");
  printf(">> ");
  int bal_add = 0;
  scanf(" %d", &bal_add);
  *acc_b = bal_add + *acc_b;
  printf("\n%d added to your account.\n", bal_add);
  printf("\nPress any key to continue.\n");
  temp = getchar();
  temp = getchar();
  menu();
}
void withdraw(int *acc_b) {
  printf("Amount to be withdraw.");
  printf(">> ");
  int bal_add = 0;
  scanf(" %d", &bal_add);
  *acc_b = *acc_b - bal_add;
  printf("\n%d withdraw from your account.\n", bal_add);
  printf("\nPress any key to continue.\n");
  temp = getchar();
  temp = getchar();
  menu();
}
void check_balance(int *acc_b) {
  printf("Account balance: %d\n", *acc_b);
  printf("\nPress any key to continue.\n");
  temp = getchar();
  temp = getchar();
  menu();
}
void menu() {
  while (true) {
    system("clear");
    line();
    int choice = 0;
    printf("1.Deposit\n");
    printf("2.Withdraw\n");
    printf("3.Balance check\n");
    printf("4.Exit\n");
    printf(">> ");
    scanf(" %d", &choice);
    line();
    switch (choice) {
    case 1:
      deposit(&acc_balance);
      break;
    case 2:
      withdraw(&acc_balance);
      break;

    case 3:
      check_balance(&acc_balance);
      break;

    case 4:
      printf("\nExiting...\n");
      exit(0);
    default:
      printf("\nContinuing...\n");
      continue;
    }
  }
}
void line() {
  initscr();
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  endwin();
  for (int i = 0; i < cols; i++) {
    printf("-");
  }
  printf("\n");
}
