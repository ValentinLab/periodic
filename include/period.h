#ifndef PERIOD_H
#define PERIOD_H

#include <stdbool.h>

/* ---------- Data structures ---------- */

struct command {
  int start;
  int period;
  char *cmd;
};

bool command_equals(struct command *self, struct command *other);

struct list_cmd {
  struct list_cmd *next;
  struct command *cmd;
};

struct list_cmd *list_cmd_add(struct list_cmd *self, const struct command *cmd);

struct list_cmd *list_cmd_remove(struct list_cmd *self, const struct command *cmd);

/* ---------- Functions ---------- */

int file_exists(const char *pathname);

void write_pid();

void output_redirections();

void create_fifo();

void create_directory();

int main(int argc, char **argv);

#endif