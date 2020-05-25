#ifndef PERIOD_MAIN_H
#define PERIOD_MAIN_H

/* ---------- Handler ---------- */

/**
 * Handler for:
 *    SIGUSR1, SIGURS2, SIGALRM, SIGCHLD, SIGQUIT, SIGTERM
 * 
 * @param sig Signal number
 */
void handler(int sig);

/* ---------- Functions ---------- */

/**
 * Receive a new command from periodic
 * 
 * @param fifo_fd Pipe's file descriptor
 * @param cl Pointer to the list of commands
 * @param no Command number
 * @return Pointer to the list of commands changed (add, delete command ...)
 */
struct command_list *receive_new_command(int fifo_fd, struct command_list *cl, int no);

/**
 * Send all currently saved commands to periodic
 * 
 * @param fifo_fd Pipe's file descriptor
 * @param cl Pointer to the list of commands
 */
void send_all_commands(int fifo_fd, struct command_list *cl);

/**
 * Get the next command to execute and set an alarm
 * 
 * @param cl Pointer to the list of commands
 */
void get_next_command(struct command_list *cl);

/**
 * Execute every commands with a 'next_exec' equals to Epoch
 * 
 * @param cl Pointer to the list of commands
 * @return Pointer to the list of commands changed (add, delete command ...)
 */
struct command_list *exec_commands(struct command_list *cl);

/**
 * Exexute a command (i.e. fork)
 * 
 * @param cl Pointer to the list of commands
 */
void execute_one_command(struct command_list *cl);

/**
 * Wait for period's children
 * 
 * @return Pointer to the list of commands changed (add, delete command ...)
 */
struct command_list *wait_child();

/**
 * Kill all period's children
 * 
 * @param cl Pointer to the list of commands
 */
void kill_children(struct command_list *cl);

/**
 * Function called at normal process termination
 * Removes /tmp/period.pid"
 */
void exit_period();

/* ---------- Main ---------- */

int main(int argc, char **argv);

#endif