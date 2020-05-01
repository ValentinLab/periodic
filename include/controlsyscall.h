#ifndef CONTROLSYSCALL_H
#define CONTROLSYSCALL_H

#include <stdio.h>
#include <sys/types.h>

#define EXIT_SYSCALL 3

/* ---------- General ---------- */

void perror_control(int value, const char *message);

/* ---------- Process operations ---------- */

pid_t fork_control();

pid_t wait_control(int *status);

/* ---------- File descriptors ---------- */

int open_control(const char *pathname, int flags);

int open_m_control(const char *pathname, int flags, mode_t mode);

void dup2_control(int fd1, int fd2);

void close_control(int fd);

/* ---------- File stream ---------- */

FILE *fopen_control(const char *pathname, const char *mode);

void fclose_control(FILE *stream);

#endif