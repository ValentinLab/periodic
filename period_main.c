#define _DEFAULT_SOURCE

#include "period_main.h"
#include "period_ds.h"
#include "period_files.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "message.h"
#include "controlsyscall.h"

/* ---------- Flags ---------- */

volatile sig_atomic_t on_progress = 1;
volatile sig_atomic_t usr1_receive = 0;
volatile sig_atomic_t usr2_receive = 0;
volatile sig_atomic_t alrm_receive = 0;

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
  cmd->cmd = datas[0];
  cmd->start = atol(datas[1]);
  cmd->period = atoi(datas[2]);
  cmd->next_exec = cmd->start;

  // Add command to the list
  cl = command_list_add(cl, cmd);
  return cl;
}

void send_all_commands(int fifo_fd, struct command_list *cl) {
  char *current_cmd[4];
  while(cl != NULL) {
    char start[20];
    sprintf(start, "%ld", cl->data->start);
    char period[11];
    sprintf(period, "%d", cl->data->period);

    current_cmd[0] = cl->data->cmd;
    current_cmd[1] = start;
    current_cmd[2] = period;
    current_cmd[3] = NULL;

    send_argv(fifo_fd, current_cmd);

    cl = cl->next;
  }
}

void get_next_command(struct command_list *cl) {
  // Get current time
  time_t now = time(NULL);
  perror_control(now, "Get time (time)");

  // Check data structure
  if(cl == NULL) {
    return;
  }

  // Set alarm
  unsigned int alarm_time = cl->data->next_exec - now;
  alarm(alarm_time);
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
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGQUIT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);

  // Create list of commands
  struct command_list *all_cmds = NULL;

  // Pause until signals
  while(on_progress) {
    pause();

    // Set alarm
    get_next_command(all_cmds);

    // SIGUSR1
    if(usr1_receive == 1) {
      all_cmds = receive_new_command(fifo, all_cmds);

      command_list_dump(all_cmds);
    }
    // SIGUSR2
    if(usr2_receive == 1) {
      send_all_commands(fifo, all_cmds);
    }
    // SIGALRM
    if(alrm_receive == 1) {
      fprintf(stderr, "Yes !");
    }
  }

  // Fin du programme
  close_control(fifo);
  int ret = unlink("/tmp/period.pid");
  perror_control(ret, "Suppression d'un fichier (unlink)");
  command_list_destroy(all_cmds);

  return EXIT_SUCCESS;
}