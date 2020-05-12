#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/message.h"

#define FILE_PATH "/tmp/reader.fifo"

int main(int argc, char *argv[]) {
  int fd = open(FILE_PATH, O_RDONLY);
  printf("%s\n", recv_string(fd));
  
  char **args = recv_argv(fd);
  while (*args != NULL) {
    printf("%s\n", *args);
    args++;
  }
  
  close(fd);

  return EXIT_SUCCESS;
}