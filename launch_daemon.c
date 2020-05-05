#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "controlsyscall.h"

#define USAGE "launch_daemon ABSOLUTE_PATH\n"

int main(int argc, char **argv) {
  // Check arguments number
  if(argc != 2) {
    fprintf(stderr, "Error: Two arguments are needed\n%s", USAGE);
    return EXIT_FAILURE;
  }
  // Check path to daemon
  const char *path_to_daemon = argv[1];
  struct stat st_daemon;
  int ret = stat(path_to_daemon, &st_daemon);
  if(ret == -1) {
    if(errno == ENOENT) {
      fprintf(stderr, "Error: no such file or directory\n%s", USAGE);
      return EXIT_FAILURE;
    }
    perror("Get informations about the daemon (stat)");
    return EXIT_SYSCALL;
  }
  if(!S_ISREG(st_daemon.st_mode)) {
    fprintf(stderr, "Error: the file must be a regular file\n%s", USAGE);
    return EXIT_FAILURE;
  }

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
      execl(path_to_daemon, "period", NULL);
      perror("Launch period (execl)");
      exit(EXIT_SYSCALL);
    }

    exit(EXIT_SUCCESS);
  }

  // Wait child
  pid = wait_control(NULL);

  return EXIT_SUCCESS;
}