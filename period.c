#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "controlstream.h"

void write_pid() {
  // PID filename
  char *pid_filename = "/tmp/period.pid";

  // Check if PID file already exists
  int acc = access(pid_filename, F_OK);
  if(acc == -1 && errno != ENOENT) {
    perror("Access file (access)");
    exit(EXIT_FAILURE);
  }
  if(acc == 0) {
    fprintf(stderr, "Error: a period process is already running.\n");
    exit(EXIT_FAILURE);
  }

  // Write pid
  FILE *pid_output = fopen_control_errors(pid_filename, "w");
  fprintf(pid_output, "%d", getpid());
  fclose_control_errors(pid_output);
}

int main(int argc, char **argv) {
  // Save PID in a file
  write_pid();

  // TODO : question 6.2 (pour la version finale)

  return EXIT_SUCCESS;
}