#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  printf("$ ");

  char statement[256];
  scanf("%s", &statement);

  printf("%s: command not found\n", statement);

  return 0;
}
