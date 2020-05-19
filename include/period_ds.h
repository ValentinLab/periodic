#ifndef PERIOD_DS_H
#define PERIOD_DS_H

/* ---------- Command structure ---------- */

struct command {
  char *cmd;
  int period;
  long start;
  long next_exec;
};

void command_dump(const struct command *self);

int command_cmp(const struct command *one, const struct command *two);

void command_destroy(struct command *self);

/* ---------- Command list structure ---------- */

struct command_list {
  struct command *data;
  struct command_list *next;
};

struct command_list *command_list_add(struct command_list *self, struct command *data);

struct command_list *command_list_remove(struct command_list *self, struct command *data);

void command_list_dump(const struct command_list *self);

void command_list_destroy(struct command_list *self);

#endif