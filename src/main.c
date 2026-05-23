#include <stdio.h>
#include <stdlib.h>

void loop() {
  printf("$ ");

  char statement[256];
  scanf("%s", &statement);

  printf("%s: command not found\n", statement);
  loop();
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  loop();

  return 0;
}
