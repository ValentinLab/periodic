#ifndef CONTROLGENERAL_H
#define CONTROLGENERAL_H

#include <sys/types.h>

void perror_control(int value, const char *message);

pid_t fork_control();

pid_t wait_control(int *status);

#endif