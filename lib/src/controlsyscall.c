#include "controlsyscall.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/* ---------- General ---------- */

void perror_control(int value, const char *message) {
  if(value == -1) {
    perror(message);
    exit(EXIT_SYSCALL);
  }
}

/* ---------- Process operations ---------- */

pid_t fork_control() {
  pid_t pid = fork();
  perror_control(pid, "New process (fork)");

  return pid;
}

pid_t wait_control(int *status) {
  pid_t pid = wait(status);
  perror_control(pid, "Wait for child process (wait)");

  return pid;
}

/* ---------- File descriptors ---------- */

int open_control(const char *pathname, int flags) {
  int fd = open(pathname, flags);
  perror_control(fd, "Open file (open)");

  return fd;
}

int open_m_control(const char *pathname, int flags, mode_t mode) {
  int fd = open(pathname, flags, mode);
  perror_control(fd, "Open file (open)");

  return fd;
}

void dup2_control(int fd1, int fd2) {
  int ret = dup2(fd1, fd2);
  perror_control(ret, "Duplicate file descriptor (dup2)");
}

void close_control(int fd) {
  int ret = close(fd);
  perror_control(ret, "Close file (close)");
}

/* ---------- File stream ---------- */

FILE *fopen_control(const char *pathname, const char *mode) {
  FILE *current_file = fopen(pathname, mode);
  if(current_file == NULL) {
    perror("Open file (fopen)");
    exit(EXIT_SYSCALL);
  }

  return current_file;
}

void fclose_control(FILE *stream) {
  int ret = fclose(stream);
  if(ret == EOF) {
    perror("Close file (fclose)");
    exit(EXIT_SYSCALL);
  }
}