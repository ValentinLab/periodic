#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "message.h"
#include "controlsyscall.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "Error: argument number\n");
    return EXIT_FAILURE;
  }

  long pid = atol(argv[1]);
  
  int fifo_fd = open_control("/tmp/period.fifo", O_WRONLY);

  char *datas[] = {"ls -l", "10", "0", NULL};
  send_argv(fifo_fd, datas);

  kill(pid, SIGUSR1);

  close_control(fifo_fd);
  return EXIT_SUCCESS;
}