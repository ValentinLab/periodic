#include "../include/message.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int send_string(int fd, const char *str) {
  
  int size = strlen(str);

  write(fd,&size,size);
  write(fd,str,size);

  return EXIT_SUCCESS;
}

char *recv_string(int fd) {
  return NULL;
}

int send_argv(int fd, char *argv[]) {
  return EXIT_SUCCESS;
}

char **recv_argv(int fd) {
  return NULL;
}

int main() {
  return EXIT_SUCCESS;
}