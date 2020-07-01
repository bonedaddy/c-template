#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argtable3.h>
#include <string.h>
#include "../include/utils/array_len.h"

#define MAX_ARGS 32

typedef struct command_object {
  char *argv[MAX_ARGS];
  int argc;
} command_object;

bool is_argument(char *arg);
command_object *new_command_object();


bool is_argument(char *arg) {
    if (strlen(arg) >= 2) {
      char *key = arg;
      char c[2] = {key[0], key[1]};
      char *part = c;
      if (strcmp(part, "--") == 0) {
        return true;
      }
    }
  return false;
}
// parses argc and argv to generate the root command object
command_object *new_command_object(int argc, char *argv[]) {
  if (argc > MAX_ARGS) {
    return NULL;
  }
  command_object *pcobj = malloc(sizeof(command_object) + sizeof*(argv));
  for (int i = 0; i < argc; i++) {
    // we only accept whole commands or arguments with `--argName`
    if (strlen(argv[i]) >= 2) {
      // check to see if it is a command name or argument
      if (is_argument(argv[i])) {
        printf("found argument: %s\n", argv[i]);
      }
    }
    pcobj->argv[i] = argv[i];
  }
  return pcobj;
}
int main(int argc, char *argv[]) {
  command_object *pcmd = new_command_object(argc, argv);
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      printf("%s\n", pcmd->argv[i]);
    }
  }
}