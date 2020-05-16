#define _DEFAULT_SOURCE

#include "period_main.h"
#include "period_ds.c"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "message.h"
#include "controlsyscall.h"

/* ---------- Flags ---------- */

volatile sig_atomic_t on_progress = 1;
volatile sig_atomic_t usr1_receive = 0;
volatile sig_atomic_t usr2_receive = 0;

/* ---------- Data structures ---------- */

// ----- Command

void command_dump(struct command *self) {
  printf("'%s' : %ld : %d : %ld\n", self->cmd, self->start, self->period, self->next_exec);
}

void command_destroy(struct command *self) {
  free(self->cmd);
  free(self);
}

// ----- Command_list

struct command_list *command_list_add(struct command_list *self, struct command *data) {
  assert(data != NULL);

  if(self == NULL || data->next_exec <= self->data->next_exec) {
    struct command_list *new_node = malloc(sizeof(struct command_list));
    new_node->data = data;
    new_node->next = self;

    return new_node;
  }

  return command_list_add(self->next, data);
}

void command_list_dump(struct command_list *self) {
  if(self == NULL) {
    return;
  }

  command_dump(self->data);
  command_list_dump(self->next);
}

void command_list_destroy(struct command_list *self) {
  if(self == NULL) {
    return;
  }

  command_list_destroy(self->next);
  command_destroy(self->data);
  free(self);
}


/* ---------- Handler ---------- */

void handler(int sig) {
  switch (sig) {
    case SIGUSR1:
      usr1_receive = 1;
      break;
    case SIGUSR2:
      usr2_receive = 1;
      break;
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
      on_progress = 0;
      break;
  }
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

int create_fifo() {
  // Fifo pathname
  const char *fifo_pathname = "/tmp/period.fifo";

  // Check if named pipe already exists, then create it
  int exists = file_exists(fifo_pathname);
  if(exists != 0) {
    int ret = mkfifo(fifo_pathname, 0644);
    perror_control(ret, "Create fifo (mkfifo)");
  }

  // Open fifo
  return open_control(fifo_pathname, O_RDWR);
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

struct command_list *receive_new_command(int fifo_fd, struct command_list *cl) {
  // Set flag to 0
  usr1_receive = 0;

  // Get datas
  char **datas = recv_argv(fifo_fd);
  struct command *cmd = malloc(sizeof(struct command));
  cmd->cmd = datas[0];
  cmd->start = atol(datas[1]);
  cmd->period = atoi(datas[2]);
  cmd->next_exec = cmd->start + cmd->period;

  // Add command to the list
  cl = command_list_add(cl, cmd);
  return cl;
}

void send_all_commands(int fifo_fd, struct command_list *cl) {
  char *current_cmd[3];
  while(cl != NULL) {
    char start[20];
    sprintf(start, "%ld", cl->data->start);
    char period[11];
    sprintf(period, "%d", cl->data->period);

    current_cmd[0] = cl->data->cmd;
    current_cmd[1] = start;
    current_cmd[2] = period;

    send_argv(fifo_fd, current_cmd);

    cl = cl->next;
  }
}

int main(int argc, char **argv) {
  // Save PID in a file, create a named pipe and a period directory
  write_pid();
  //output_redirections();
  int fifo = create_fifo();
  create_directory();

  // Handler installation
  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_handler = handler;
  action.sa_flags = 0;
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGUSR2, &action, NULL);
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGQUIT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);

  // Create list of commands
  struct command_list *all_cmds = NULL;

  // Pause until signals
  while(on_progress) {
    pause();

    // SIGUSR1
    if(usr1_receive == 1) {
      all_cmds = receive_new_command(fifo, all_cmds);
    }
    // SIGUSR2
    if(usr2_receive == 1) {
      send_all_commands(fifo, all_cmds);
    }
  }

  // Fin du programme
  close_control(fifo);
  int ret = unlink("/tmp/period.pid");
  perror_control(ret, "Suppression d'un fichier (unlink)");
  command_list_destroy(all_cmds);

  return EXIT_SUCCESS;
}