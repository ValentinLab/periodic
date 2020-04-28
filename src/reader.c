#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/message.h"

int main(int argc, char *argv[]) {
  char *path = "/tmp/reader";
  int fd = open(path, O_RDONLY);
  printf("%s\n", recv_string(fd));
  
  char **args = recv_argv(fd);
  while (*args != NULL) {
    printf("%s\n", *args);
    args++;
  }
  
  close(fd);

  return EXIT_SUCCESS;
}