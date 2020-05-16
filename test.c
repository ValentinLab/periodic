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
  if(argc < 2 || argc > 3) {
    fprintf(stderr, "Error: argument number\n");
    return EXIT_FAILURE;
  }

  long pid = atol(argv[1]);
  
  int fifo_fd;
  if(argc == 2) {
    fifo_fd = open_control("/tmp/period.fifo", O_WRONLY);

    kill(pid, SIGUSR1);

    char *datas[] = {"ls -l", "10", "5", NULL};
    send_argv(fifo_fd, datas);
  } else {
    fifo_fd = open_control("/tmp/period.fifo", O_RDONLY);

    kill(pid, SIGUSR2);

    char **res = recv_argv(fifo_fd);
    printf("cmd : %s\n", res[0]);
    printf("start : %s\n", res[1]);
    printf("period : %s\n", res[2]);
  }

  close_control(fifo_fd);
  return EXIT_SUCCESS;
}