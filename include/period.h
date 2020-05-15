#ifndef PERIOD_H
#define PERIOD_H

#include <stdbool.h>

/* ---------- Data structures ---------- */

struct command {
  char *cmd;
  int period;
  long start;
  long next_exec;
};

struct command_list {
  struct command *data;
  struct command_list *next;
};

/* ---------- Data structures functions ---------- */

struct command_list *command_list_add(struct command_list *self, struct command *data);

/* ---------- Functions ---------- */

int file_exists(const char *pathname);

void write_pid();

void output_redirections();

int create_fifo();

void create_directory();

int main(int argc, char **argv);

#endif