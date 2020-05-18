#ifndef PERIOD_MAIN_H
#define PERIOD_MAIN_H

/* ---------- Handler ---------- */

void handler(int sig);

/* ---------- Functions ---------- */

struct command_list *receive_new_command(int fifo_fd, struct command_list *cl);

void send_all_commands(int fifo_fd, struct command_list *cl);

void get_next_command(struct command_list *cl);

void execute_command(struct command_list *cl);

void wait_child();

/* ---------- Functions ---------- */

int main(int argc, char **argv);

#endif