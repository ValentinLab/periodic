#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include "controlgeneral.h"
#include "controlfd.h"

#define DAEMON_PATHNAME "/home/valentin/documents/travail/l2/s4/systeme/projets/periodic/bin/period"

int main(int argc, char **argv) {
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
      close_control_errors(0);
      close_control_errors(1);
      close_control_errors(2);

      // Exec daemon
      execl(DAEMON_PATHNAME, "period", NULL);
      perror("Launch period (execl)");
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
  }

  // Wait child
  pid = wait_control(NULL);

  return EXIT_SUCCESS;
}