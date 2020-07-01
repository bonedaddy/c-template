#include <stdbool.h>
#include <stdio.h>
#include "../include/utils/command_line.h"
#include "../include/utils/safe_mem.h"


int main(int argc, char *argv[]) {
  command_object *pcmd = new_command_object(argc, argv);
  if (pcmd == NULL) {
    printf("failed to get command_object");
    return - 1;
  }
  // create a safe memory wrapper around the command object
  memory_object memobj = new_memory_object(pcmd);
  pcmd->command->callback(pcmd->argc, pcmd->argv);
  int response = free_memory_object(&memobj);
  if (response != 0) {
    printf("failed to free memory object\n");
    return response;
  }
  return 0;
}