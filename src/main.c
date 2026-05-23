#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loop() {
  printf("$ ");

  char statement[256];
  fgets(statement, 256, stdin);

  statement[strlen(statement)-1] = 0;

  if (strcmp(statement, "exit") == 0) {
    return;
  }

  if (strncmp(statement, "echo ", 5) == 0) {
    printf("%s\n", statement + 5);
    return loop();
  }

  printf("%s: command not found\n", statement);
  return loop();
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  loop();
  return 0;
}
