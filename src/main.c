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
    memcpy(filepath + (end - start) + 1, command, strlen(command) * sizeof(char) + 1);

    if (access(filepath, X_OK) == 0) {
      return filepath;
    }  

    start = end + 1;
  } 
}

char* unquote(char* input) {
  char *output_start = malloc(strlen(input) + 1);
  char *output = output_start;

  bool inside_single_quotes = false;
  bool inside_double_quotes = false;
  bool inside_space = false;
  bool is_escaped = false;

  while (*input) {
    if (*input == '\'') {
      if (inside_double_quotes || is_escaped) {
        *output = *input; 
        output += 1;

        is_escaped = false;
      } else {
        inside_single_quotes = !inside_single_quotes;
      }
      inside_space = false;
    } else if (*input == '"') {
      if (is_escaped || inside_single_quotes) {
        *output = *input; 
        output += 1;

        is_escaped = false;
      } else {
        inside_double_quotes = !inside_double_quotes;
        inside_space = false;
      }
    } else if (*input == ' ') {
      if (!inside_space || inside_single_quotes || inside_double_quotes || is_escaped) {
        *output = *input; 
        output += 1;

        inside_space = true;
        is_escaped = false;
      }
    } else if (*input == '\\') {
      if (is_escaped || inside_single_quotes) {
        *output = *input; 
        output += 1;

        is_escaped = false;
      } else {
        is_escaped = true;
      }
    } else {
      *output = *input; 
      output += 1;

      inside_space = false;
    }

    input += 1;
  }

  *output = '\0';
  return output_start;
}

char* extract_command(char* statement) {
  char* command = malloc(STATEMENT_LEN);
  strcpy(command, statement);

  if (*command == '\'') {
    char *closing = strchr(command + 1, '\'');
    *(closing + 1) = '\0';
  } else if (*command == '"') {
    char *closing = strchr(command + 1, '"');
    *(closing + 1) = '\0';
  } else {
    strtok(command, " ");
  }

  return command;
}

void loop() {
  printf("$ ");

  char statement[STATEMENT_LEN];
  fgets(statement, STATEMENT_LEN, stdin);

  statement[strlen(statement)-1] = 0; // remove newline

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
    char* args = unquote(statement + 5);
    puts(args);
    free(args);
    return loop();
  }

  if (strncmp(statement, "cd ", 3) == 0) {
    char* dir = statement + 3;
    if (strcmp(dir, "~") == 0) {
      dir = getenv("HOME");
    }
    if (chdir(dir) != 0) {
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

  char* quoted_command = extract_command(statement);
  char* command = unquote(quoted_command);
  char* executable = find_executable(command);
  if (executable) {
    system(statement);
  } else {
    printf("%s: command not found\n", command);
  }
  free(executable);
  free(command);
  free(quoted_command);

  return loop();
}

#ifndef TESTING
int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  loop();
  return 0;
}
#endif
