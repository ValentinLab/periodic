#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/message.h"

#define FILE_PATH "/tmp/reader"

int main(int argc, char *argv[]) {
  char *args[3] = {"arg1", "arg2", NULL};
  mkfifo(FILE_PATH, 0666);

  int fd = open(FILE_PATH, O_WRONLY);
  send_string(fd,"send string");
  send_argv(fd, args);
  close(fd);

  unlink(FILE_PATH);

  return EXIT_SUCCESS;
}