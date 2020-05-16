#include "period_ds.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "controlsyscall.h"

/* ---------- Command structure ---------- */

void command_dump(struct command *self) {
  printf("'%s' : %ld : %d : %ld\n", self->cmd, self->start, self->period, self->next_exec);
}

void command_destroy(struct command *self) {
  free(self->cmd);
  free(self);
}

/* ---------- Command list structure ---------- */

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
