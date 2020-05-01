#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include "controlsyscall.h"

#define USAGE "launch_daemon ABSOLUTE_PATH"

int main(int argc, char **argv) {
  // Check arguments number
  if(argc != 2) {
    fprintf(stderr, "Error: Two arguments are needed\n%s", USAGE);
    return EXIT_FAILURE;
  }
  // Check path to daemon
  

  // Fork
  pid_t pid = fork_control();

  // Child
  if(pid == 0) {
    // Session leader
    pid = setsid();
    perror_control(pid,"Set SID (setsid)");

    // Fork
    pid = fork_control();

    // Child
    if(pid == 0) {
      // Change working directory and umask
      int ret = chdir("/");
      perror_control(ret, "Change working directory (chdir)");
      umask(0);

      // Close standard file descriptors
      close_control(0);
      close_control(1);
      close_control(2);

      // Exec daemon
      execl("DAEMON_PATHNAME", "period", NULL);
      perror("Launch period (execl)");
      exit(EXIT_SYSCALL);
    }

    exit(EXIT_SUCCESS);
  }

  // Wait child
  pid = wait_control(NULL);

  return EXIT_SUCCESS;
}