#include "blackboard_handler.h"




// Error handling and process spawning functions
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int spawn(const char * program, char ** arg_list) {
  pid_t child_pid = fork();
  if (child_pid != 0)
    return child_pid;
  else {
    execvp(program, arg_list);
    error_exit("Error: exec failed");
    return 1;
 }
}
