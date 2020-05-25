#ifndef PERIOD_DS_H
#define PERIOD_DS_H

#include <sys/types.h>

/* ---------- Command structure ---------- */

struct command {
  char *cmd_name;
  char **cmd_args;
  int arg_nb;
  int period;
  long start;
  long next_exec;
  pid_t pid;
  int no;
};

// DEBUG
void command_dump(const struct command *self);

/**
 * Compare two command structures
 * (compares the name, args, start, period)
 * 
 * @param one Pointer to the first structure
 * @param two Pointer to the second structure
 * @return 0 if one and two are equals, -1 if one is before two and 2, 1 otherwise
 */
int command_cmp(const struct command *one, const struct command *two);

/**
 * Destroy a command
 * 
 * @param self Pointer to a command structure
 */
void command_destroy(struct command *self);

/* ---------- Command list structure ---------- */

struct command_list {
  struct command *data;
  struct command_list *next;
};

/**
 * Add a command structure the list of commands
 * The command position in the list depends on it's next execution time
 * 
 * @param self Pointer to the list of commands
 * @param data Pointer to the command structure
 * @return Pointer to the list with the new element
 */
struct command_list *command_list_add(struct command_list *self, struct command *data);

/**
 * Remove data from the list of commands
 * 
 * @param self Pointer to the list of commands
 * @param data Pointer to the command structure
 * @return Pointer to the list without data
 */
struct command_list *command_list_remove(struct command_list *self, struct command *data);

/**
 * Replace the first element
 * If the first element isn't at the good position, it must be moved
 * 
 * @param self Pointer to the list of commands
 * @return Pointer to the list with the first element at the good position
 */
struct command_list *command_list_replace(struct command_list *self);

// DEBUG
void command_list_dump(const struct command_list *self);

/**
 * Destroy a list of commands and the commands
 * 
 * @param self Pointer to the list of commands
 */
void command_list_destroy(struct command_list *self);

#endif