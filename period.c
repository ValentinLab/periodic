#include "period.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "controlsyscall.h"

/* ---------- Data structures ---------- */

bool command_equals(struct command *self, struct command *other) {
  return self->cmd == other->cmd && self->period == other->period && self->start == self->start;
}

struct list_cmd *list_cmd_add(struct list_cmd *self, const struct command *cmd) {
  if(self == NULL || cmd == NULL) {
    return NULL;
  }

  if(self->cmd->period > cmd->period) {
    struct list_cmd *new = malloc(sizeof(struct list_cmd));
    if(new == NULL) {
      fprintf(stderr, "Error: memory allocation");
      exit(EXIT_FAILURE);
    }
    new->cmd = cmd;
    new->next = self;

    return self;
  }

  self->next = list_cmd_add(self->next, cmd);
  return self;
}

struct list_cmd *list_cmd_remove(struct list_cmd *self, const struct command *cmd) {
  if(self == NULL || cmd == NULL) {
    return NULL;
  }

  if(command_equals(self->cmd, cmd)) {
    struct list_cmd *next = self->next;

    free(cmd);
    free(self->cmd);
    free(self);

    return next;
  }

  self->next = list_cmd_remove(self->next, cmd);
  return self;
}

/* ---------- Functions ---------- */

int file_exists(const char *pathname) {
  int ret = access(pathname, F_OK);
  if(ret == -1 && errno != ENOENT) {
    perror("Access file (access)");
    exit(EXIT_SYSCALL);
  }

  return ret;
}

void write_pid() {
  // PID pathname
  const char *pid_pathname = "/tmp/period.pid";

  // Check if PID file already exists
  int exists = file_exists(pid_pathname);
  if(exists == 0) {
    exit(EXIT_FAILURE);
  }

  // Write pid
  FILE *pid_output = fopen_control(pid_pathname, "w");
  fprintf(pid_output, "%d", getpid());
  fclose_control(pid_output);
}

void output_redirections() {
  // Stdout
  int fd = open_m_control("/tmp/period.out", O_WRONLY | O_CREAT, 0644);
  dup2_control(fd, 1);
  close_control(fd);

  // Stderr
  fd = open_m_control("/tmp/period.err", O_WRONLY | O_CREAT, 0644);
  dup2_control(fd, 2);
  close_control(fd);
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
  perror_control(ret, "Create fifo (mkfifo)");
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
  perror_control(ret, "Create directory (mkdir)");
}

int main(int argc, char **argv) {
  // Save PID in a file, create a named pipe and a period directory
  write_pid();
  //output_redirections();
  create_fifo();
  create_directory();

  return EXIT_SUCCESS;
}