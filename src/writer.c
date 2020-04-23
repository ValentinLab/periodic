#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/message.h"

int main(int argc, char *argv[]) {
  char *path = "/tmp/reader";
  char *args[3] = {"arg1", "arg2", NULL};
  mkfifo(path, 0666);

  int fd = open(path, O_WRONLY);
  send_string(fd,"send string");
  send_argv(fd, args);
  close(fd);

  unlink(path);

  return EXIT_SUCCESS;
}