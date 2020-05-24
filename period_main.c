#define _DEFAULT_SOURCE

#include "period_main.h"
#include "period_ds.h"
#include "period_files.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "message.h"
#include "controlsyscall.h"

/* ---------- Flags ---------- */

volatile sig_atomic_t on_progress = 1;
volatile sig_atomic_t usr1_receive = 0;
volatile sig_atomic_t usr2_receive = 0;
volatile sig_atomic_t alrm_receive = 0;
volatile sig_atomic_t chld_receive = 0;

/* ---------- Handler ---------- */

void handler(int sig) {
  switch (sig) {
    case SIGUSR1:
      usr1_receive = 1;
      break;
    case SIGUSR2:
      usr2_receive = 1;
      break;
    case SIGALRM:
      alrm_receive = 1;
      break;
    case SIGCHLD:
      chld_receive = 1;
      break;
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
      on_progress = 0;
      break;
  }
}

/* ---------- Functions ---------- */

struct command_list *receive_new_command(int fifo_fd, struct command_list *cl) {
  // Set flag to 0
  usr1_receive = 0;

  // Get datas
  char **datas = recv_argv(fifo_fd);
  struct command *cmd = malloc(sizeof(struct command));

  cmd->start = atol(datas[0]);
  cmd->period = atoi(datas[1]);
  cmd->next_exec = cmd->start;
  cmd->cmd_name = datas[2];
  cmd->arg_nb = atoi(datas[3]);
  cmd->cmd_args = calloc(cmd->arg_nb, sizeof(char *));
  for(size_t i = 0; i < cmd->arg_nb; ++i) {
    cmd->cmd_args[i] = atoi(datas[4+i]);
  }
  cmd->pid = 0;

  // Add command to the list
  cl = command_list_add(cl, cmd);
  return cl;
}

void send_all_commands(int fifo_fd, struct command_list *cl) {
  // Send commands
  while(cl != NULL) {
    char start[20];
    sprintf(start, "%ld", cl->data->start);
    char period[11];
    sprintf(period, "%d", cl->data->period);

    char **current_cmd = calloc(4 + cl->data->arg_nb, sizeof(char *));

    current_cmd[0] = start;
    current_cmd[1] = period;
    current_cmd[2] = cl->data->cmd_name;
    current_cmd[3] = cl->data->arg_nb;
    for(size_t i = 0; i < cl->data->arg_nb; ++i) {
      current_cmd[4+i] = cl->data->cmd_args[i];
    }

    send_argv(fifo_fd, current_cmd);

    cl = cl->next;
  }

  // Stop sending
  char *final_sending[] = {NULL};
  send_argv(fifo_fd, final_sending);
}

struct command_list *get_next_command(struct command_list *cl) {
  // Get current time
  time_t now = time(NULL);
  perror_control(now, "Get time (time)");

  // Check data structure
  if(cl == NULL) {
    return NULL;
  }

  // Set alarm
  unsigned int alarm_time = cl->data->next_exec - now;
  alarm(alarm_time);

  return cl;
}

struct command_list *exec_commands(struct command_list *cl, struct command_list *next) {
  // Execute next command
  execute_command(next);

  if(next->data->period == 0) {
    cl = command_list_remove(cl, next->data);
  }

  return cl;
}

void execute_command(struct command_list *cl) {
  // Extract command and arguments
  size_t sz = 0;
  size_t index = 0;
  while(cl->data->cmd[index] != '\0') {
    if(cl->data->cmd[index] == ' ') {
      ++sz;
    }
    ++index;
  }
  sz += 2; // last word and NULL
  char **cmd = calloc(sz+2, sizeof(char *));
  char *token = calloc(255, sizeof(char));
  strcpy(token, cl->data->cmd);
  token = strtok(token, " ");
  index = 0;
  while(token != NULL) {
    cmd[index++] = token;
    token = strtok(NULL, " ");
  }

  // Execute command
  pid_t child_pid = fork_control();
  if(child_pid == 0) {
    execvp(cmd[0], cmd);

    perror("Execute command (execvp)");
    exit(EXIT_SYSCALL);
  }

  // Free datas
  free(cmd);
  free(token);

  // Change data
  cl->data->next_exec = cl->data->next_exec + cl->data->period;
}

void wait_child() {
  // Wait child
  int status;
  pid_t pid = wait_control(&status);

  // Save status
  if(WIFEXITED(status)) {
    fprintf(stderr, "Process %d terminated : exit status %d\n", pid, WEXITSTATUS(status));
  } else if(WIFSIGNALED(status)) {
    fprintf(stderr, "Process %d terminated : signal number %d\n", pid, WTERMSIG(status));
  }
}

/* ---------- Main ---------- */

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
  sigaction(SIGALRM, &action, NULL);
  sigaction(SIGCHLD, &action, NULL);
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGQUIT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);

  // Create list of commands
  struct command_list *all_cmds = NULL;
  struct command_list *next = NULL;

  // Pause until signals
  while(on_progress) {
    pause();

    // SIGUSR1
    if(usr1_receive == 1) {
      // Get command
      all_cmds = receive_new_command(fifo, all_cmds);

      // Set alarm
      next = get_next_command(all_cmds);
    }
    // SIGUSR2
    if(usr2_receive == 1) {
      // Send commands
      send_all_commands(fifo, all_cmds);
    }
    // SIGALRM
    if(alrm_receive == 1) {
      // Set flag to 0
      alrm_receive = 0;

      // Execute command
      exec_commands(all_cmds, next);
    }
    // SIGCHLD
    if(chld_receive == 1) {
      // Set flag to 0
      chld_receive = 0;

      // Wait child
      wait_child();
    }

    // -----> DEBUG
    command_list_dump(all_cmds);
  }

  // Fin du programme
  close_control(fifo);
  int ret = unlink("/tmp/period.pid");
  perror_control(ret, "Suppression d'un fichier (unlink)");
  command_list_destroy(all_cmds);

  return EXIT_SUCCESS;
}