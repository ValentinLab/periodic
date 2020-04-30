#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "controlfd.h"
#include "controlstream.h"

int file_exists(const char *pathname) {
  int ret = access(pathname, F_OK);
  if(ret == -1 && errno != ENOENT) {
    perror("Access file (access)");
    exit(EXIT_FAILURE);
  }

  return ret;
}

void write_pid() {
  // PID pathname
  const char *pid_pathname = "/tmp/period.pid";

  // Check if PID file already exists
  int exists = file_exists(pid_pathname);
  if(exists == 0) {
    fprintf(stderr, "Error: a period process is already running.\n");
    exit(EXIT_FAILURE);
  }

  // Write pid
  FILE *pid_output = fopen_control_errors(pid_pathname, "w");
  fprintf(pid_output, "%d", getpid());
  fclose_control_errors(pid_output);
}

void output_redirections() {
  // Stdout
  int fd = open_m_control_errors("/tmp/period.out", O_WRONLY | O_CREAT, 0644);
  int ret = dup2(fd, 1);
  if(ret == -1) {
    perror("Ouput redir (dup2)");
    exit(EXIT_FAILURE);
  }
  close_control_errors(fd);

  // Stderr
  fd = open_m_control_errors("/tmp/period.err", O_WRONLY | O_CREAT, 0644);
  ret = dup2(fd, 2);
  if(ret == -1) {
    perror("Ouput redir (dup2)");
    exit(EXIT_FAILURE);
  }
  close_control_errors(fd);
}

void create_fifo() {
  // Fifo pathname
  const char *fifo_pathname = "/tmp/period.fifo";

  // Check if named pipe already exists
  int exists = file_exists(fifo_pathname);
  if(exists == 0) {
    return;
  }

  // Named pipe creation
  int ret = mkfifo(fifo_pathname, 0644);
  if(ret == -1) {
    perror("Create fifo (mkfifo)");
    exit(EXIT_FAILURE);
  }
}

void create_directory() {
  // Folder pathname
  const char *dir_pathname = "/tmp/period";

  // Check if folder already exists
  int exists = file_exists(dir_pathname);
  if(exists == 0) {
    return;

  }

  int ret = mkdir(dir_pathname, 0777);
  if(ret == -1) {
    perror("Create directory (mkdir)");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **argv) {
  // Save PID in a file, create a named pipe and a period directory
  write_pid();
  //output_redirections();
  create_fifo();
  create_directory();

  return EXIT_SUCCESS;
}