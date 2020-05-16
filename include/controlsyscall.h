#ifndef CONTROLSYSCALL_H
#define CONTROLSYSCALL_H

#include <stdio.h>
#include <sys/types.h>

#define EXIT_SYSCALL 3

/* ---------- General ---------- */

/**
 * Call perror and display message if value is equals to -1
 */
void perror_control(int value, const char *message);

/* ---------- Process operations ---------- */

/**
 * Fork and control the returned value
 */
pid_t fork_control();

/**
 * Wait and control the returned value
 */
pid_t wait_control(int *status);

/* ---------- File descriptors ---------- */

/*
 * Open a file (with file descriptor) and control the returned value
 */
int open_control(const char *pathname, int flags);

/**
 * Open a file with a specified mode (with file descriptor) and control the returned value
 */
int open_m_control(const char *pathname, int flags, mode_t mode);

/**
 * Duplicate a file descriptor and control the returned value
 */
void dup2_control(int fd1, int fd2);

/**
 * Close a file (with file descriptor) and control the returned value
 */
void close_control(int fd);

/* ---------- File streams ---------- */

/*
 * Open a file (with stream) and control the returned value
 */
FILE *fopen_control(const char *pathname, const char *mode);

/**
 * Close a file (with stream) and control the returned value
 */
void fclose_control(FILE *stream);

#endif