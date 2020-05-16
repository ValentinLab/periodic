#ifndef PERIOD_MAIN_H
#define PERIOD_MAIN_H

/* ---------- Handler ---------- */

void handler(int sig);

/* ---------- Functions ---------- */

struct command_list *receive_new_command(int fifo_fd, struct command_list *cl);

void send_all_commands(int fifo_fd, struct command_list *cl);

/* ---------- Functions ---------- */

int main(int argc, char **argv);

#endif