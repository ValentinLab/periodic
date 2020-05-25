#ifndef PERIOD_FILES_H
#define PERIOD_FILES_H

/**
 * Check if the file pointed by pathname exists
 * 
 * @param pathname Path to the file
 * @return 0 if the file exists, 0 otherwise
 */
int file_exists(const char *pathname);

/**
 * Write the process pid in a file
 */
void write_pid();

/**
 * Redirect process outputs
 */
void output_redirections();

/**
 * Create a named pipe
 * 
 * @return Pipe file descriptor
 */
int create_fifo();

/**
 * Create a period directory
 */
void create_directory();

#endif