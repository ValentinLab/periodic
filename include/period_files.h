#ifndef PERIOD_FILES_H
#define PERIOD_FILES_H

int file_exists(const char *pathname);

void write_pid();

void output_redirections();

int create_fifo();

void create_directory();

#endif