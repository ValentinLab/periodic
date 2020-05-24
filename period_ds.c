#include "period_ds.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controlsyscall.h"

/* ---------- Command structure ---------- */

void command_dump(const struct command *self) {
  if(self == NULL) {
    printf("-NULL-");
    return;
  }

  printf("'%s' : %ld : %d : %ld\n", self->cmd_name, self->start, self->period, self->next_exec);
}

int command_cmp(const struct command *one, const struct command *two) {
  int cmd = strcmp(one->cmd_name, two->cmd_name);
  int start = one->start - two->start;
  int period = one->period - two->period;

  int args = 0;
  if(one->arg_nb == two->arg_nb) {
    for(size_t i = 0; i < one->arg_nb; ++i) {
      args += strcmp(one->cmd_args[i], two->cmd_args[i]);
    }
  } else {
    args = one->arg_nb - two->arg_nb;
  }

  return cmd + start + period + args;
}

void command_destroy(struct command *self) {
  // Free datas
  free(self->cmd_name);
  for(size_t i = 0; i < self->arg_nb; ++i) {
    free(self->cmd_args[i]);
  }
  free(self->cmd_args);

  // Free element
  free(self);
}

/* ---------- Command list structure ---------- */

struct command_list *command_list_add(struct command_list *self, struct command *data) {
  assert(data != NULL);

  if(self == NULL || data->next_exec < self->data->next_exec) {
    struct command_list *new_node = malloc(sizeof(struct command_list));
    new_node->data = data;
    new_node->next = self;

    return new_node;
  }

  self->next = command_list_add(self->next, data);
  return self;
}

struct command_list *command_list_remove(struct command_list *self, struct command *data) {
  if(self == NULL) {
    return NULL;
  }

  if(self->next == NULL) {
    if(command_cmp(self->data, data) == 0) {
      command_destroy(data);
      free(self);

      return NULL;
    }
  }

  if(command_cmp(self->next->data, data) == 0) {
    command_destroy(data);
    struct command_list *tmp = self->next;
    self->next = self->next->next;
    free(tmp);

    return self;
  }

  self->next = command_list_remove(self->next, data);
  return self;
}

void command_list_dump(const struct command_list *self) {
  if(self == NULL) {
    printf("--NULL--");
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
