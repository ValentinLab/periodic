#ifndef CONTROLFD_H
#define CONTROLFD_H

#include <sys/stat.h>

int open_control_errors(const char *pathname, int flags);

int open_m_control_errors(const char *pathname, int flags, mode_t mode);

void close_control_errors(int fd);

#endif