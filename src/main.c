#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STATEMENT_LEN 256
#define PATH_LEN 256

char* find_executable(char* command) {
  char* start = getenv("PATH");

  while (*start) {
    char* end = strchr(start, ':');

    if (end == NULL) {
      return NULL;
    }

    char* filepath = malloc(PATH_LEN);
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

  char statement[STATEMENT_LEN];
  fgets(statement, STATEMENT_LEN, stdin);

  statement[strlen(statement)-1] = 0;

  if (strcmp(statement, "exit") == 0) {
    return;
  }

  if (strcmp(statement, "pwd") == 0) {
    char* cwd = malloc(PATH_LEN);
    if (getcwd(cwd, PATH_LEN) == NULL) {
      puts("getcwd overflow");
      exit(1);
    }
    puts(cwd);
    free(cwd);
    return loop();
  }

  if (strncmp(statement, "echo ", 5) == 0) {
    puts(statement + 5);
    return loop();
  }

  if (strncmp(statement, "cd ", 3) == 0) {
    char* dir = statement + 3;
    if (access(dir, F_OK) == 0) {
      chdir(dir);
    } else {
      printf("%s: No such file or directory\n", dir);
    }
    return loop();
  }

  if (strncmp(statement, "type ", 5) == 0) {
    char* command = statement + 5;
    if (strcmp(command, "exit") == 0 || strcmp(command, "echo") == 0 || strcmp(command, "type") == 0 || strcmp(command, "pwd") == 0) {
      printf("%s is a shell builtin\n", command);
    } else {
      char* executable = find_executable(command);
      if (executable) {
        printf("%s is %s\n", command, executable);
      } else {
        printf("%s: not found\n", command);
      }
      free(executable);
    }
    return loop();
  }

  char *space_pos = strchr(statement, ' ');

  char* command;
  if (space_pos) {
    int command_len = space_pos - statement;
    command = malloc(command_len);
    memcpy(command, statement, command_len);
  } else {
    command = statement;
  }

  char* executable = find_executable(command);
  if (executable) {
    system(statement);
  } else {
    printf("%s: command not found\n", statement);
  }
  free(executable);

  return loop();
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  loop();
  return 0;
}
