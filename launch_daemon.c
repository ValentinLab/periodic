#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid = fork();
  if(pid == -1) {
    perror("New child (fork)");
    return EXIT_FAILURE;
  }

  if(pid == 0) {
    pid = setsid();
    if(pid == -1) {
      perror("Set SID (setsid)");
      exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1) {
      perror("New child (fork)");
      exit(EXIT_FAILURE);
    }

    if(pid == 0) {
      execl("bin/period", "period", NULL);

      perror("Launch period (execl)");
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
  }

  pid = wait(NULL);
  if(pid == -1) {
    perror("Wait child (wait)");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}