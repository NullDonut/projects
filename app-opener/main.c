#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char forceClose[] =
    "adb shell am force-stop com.termux.x11 && pkill -f termux-x11";
char openX11Activity[] =
    "adb shell am start -n com.termux.x11/com.termux.x11.MainActivity "
    "--windowingMode 5";
char startActivityInTermux[] = "termux-x11 :1 & export DISPLAY=:1";
void runCommand(char command[]) { system(command); }
void manageFlags(char flagCommand[]);
void autoMode(char command[]) {
  printf("Command to be run is: %s\n ", command);
  runCommand(forceClose);
  // runCommand(startActivityInTermux);
  runCommand(openX11Activity);
  char commandi[512];
  snprintf(commandi, sizeof(commandi),
           "termux-x11 :1 -xstartup \"dbus-launch --exit-with-session %s\"",
           command);
  system(commandi);
  exit(0);
}
int main(int clen, char* cargs[]) {
  if (clen == 2) {
    autoMode(cargs[1]);
  }
  char buf[512];
  int isFlag = 0;
  runCommand(forceClose);
  while (1) {
    printf(">>  ");
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = '\0';
    printf("Command Entered: %s\n", buf);
    if (buf[0] == '/') {
      isFlag = 1;
    } else {
      isFlag = 0;
    }
    if (isFlag) {
      manageFlags(buf);
    }
    // if (buf[])
  }
}
void manageFlags(char flagCommand[]) {
  if (strcmp("/help", flagCommand) == 0) {
    printf("Printing Help Function..\n");
  }
  if (strcmp("/open", flagCommand) == 0) {
    printf("Opening App....\n");
    runCommand(openX11Activity);
  }
  if (strcmp("/close", flagCommand) == 0) {
    printf("CLosing Apps..\n");
    runCommand(forceClose);
  }
}
