#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int send_string(int fd, const char *str) {
  size_t size = strlen(str);
  if(write(fd,&size,sizeof(size_t)) == -1) {
    perror("send_string");
    return -1;
  }
  if(write(fd,str,size)) {
    perror("send_string");
    return -1;
  }

  return EXIT_SUCCESS;
}

char *recv_string(int fd) {
  size_t size = 0;
  read(fd,&size,sizeof(size_t));
  char *buf = calloc(size,sizeof(char));
  read(fd,buf,size);
  return buf;
}

int send_argv(int fd, char *argv[]) {
  size_t size = 0;
  while (argv[size] != NULL) {
    size++;
  }
  write(fd,&size,sizeof(size_t));
  
  for (size_t i = 0; i < size; i++) {
    send_string(fd, argv[i]);
  }
  
  return EXIT_SUCCESS;
}

char **recv_argv(int fd) {
  size_t size = 0;
  read(fd,&size,sizeof(size_t));
  char **buf = calloc(size+1,sizeof(char *));
  for (size_t i = 0; i < size; i++) {
    buf[i] = recv_string(fd);
  }
  buf[size] = NULL;
  return buf;
}
