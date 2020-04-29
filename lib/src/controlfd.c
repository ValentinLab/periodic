#include "controlfd.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int open_control_errors(const char *pathname, int flags) {
  int fd = open(pathname, flags);
  if(fd == -1) {
    perror("Open file (open)");
    exit(EXIT_FAILURE);
  }

  return fd;
}

int open_m_control_errors(const char *pathname, int flags, mode_t mode) {
  int fd = open(pathname, flags, mode);
  if(fd == -1) {
    perror("Open file (open)");
    exit(EXIT_FAILURE);
  }

  return fd;
}

void close_control_errors(int fd) {
  int ret = close(fd);
  if(ret == -1) {
    perror("Close file (close)");
    exit(EXIT_FAILURE);
  }
}