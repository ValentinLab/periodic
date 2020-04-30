#include "controlgeneral.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void perror_control(int value, const char *message) {
  if(value == -1) {
    perror(message);
    exit(EXIT_FAILURE);
  }
}

pid_t fork_control() {
  pid_t pid = fork();
  perror_control(pid, "Fork (fork)");

  return pid;
}

pid_t wait_control(int *status) {
  pid_t pid = wait(status);
  perror_control(pid, "Wait process (wait)");

  return pid;
}