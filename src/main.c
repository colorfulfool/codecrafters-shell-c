#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* find_executable(char* command) {
  char* start = getenv("PATH");

  while (*start) {
    char* end = strchr(start, ':');

    if (end == NULL) {
      return NULL;
    }

    char* filepath = malloc(256);
    memcpy(filepath, start, (end - start));
    filepath[end - start] = '/';
    memcpy(filepath + (end - start) + 1, command, strlen(command) * sizeof(char));

    if (access(filepath, X_OK) == 0) {
      return filepath;
    }  

    start = end + 1;
  } 
}

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

  if (strncmp(statement, "type ", 5) == 0) {
    char* arg = statement + 5;
    if (strcmp(arg, "exit") == 0 || strcmp(arg, "echo") == 0 || strcmp(arg, "type") == 0) {
      printf("%s is a shell builtin\n", arg);
    } else {
      char* executable = find_executable(arg);
      if (executable) {
        printf("%s is %s\n", arg, executable);
      } else {
        printf("%s: not found\n", arg);
      }
    }
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
