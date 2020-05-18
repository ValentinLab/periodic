#include "../../include/controlsyscall.h"
#include "../../include/message.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int send_string(int fd, const char *str) {
  size_t size = strlen(str);
  perror_control(write(fd,&size,sizeof(size_t)), "Fail to write (send_string)");
  perror_control(write(fd,str,size), "Fail to write (send_string)");

  return EXIT_SUCCESS;
}

char *recv_string(int fd) {
  size_t size = 0;
  perror_control(read(fd,&size,sizeof(size_t)), "Fail to read (recv_string)");
  char *buf = calloc(size+1,sizeof(char));
  perror_control(read(fd,buf,size), "Fail to read (recv_string)");
  return buf;
}

int send_argv(int fd, char *argv[]) {
  size_t size = 0;
  while (argv[size] != NULL) {
    size++;
  }
  perror_control(write(fd,&size,sizeof(size_t)), "Fail to write (send_argv)");
  
  for (size_t i = 0; i < size; i++) {
    send_string(fd, argv[i]);
  }
  
  return EXIT_SUCCESS;
}

char **recv_argv(int fd) {
  size_t size = 0;
  perror_control(read(fd,&size,sizeof(size_t)), "Fail to read (recv_argv)");
  char **buf = calloc(size+1,sizeof(char *));
  for (size_t i = 0; i < size; i++) {
    buf[i] = recv_string(fd);
  }
  buf[size] = NULL;
  return buf;
}
