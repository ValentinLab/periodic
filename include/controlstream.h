#ifndef CONTROLSTREAM_H
#define CONTROLSTREAM_H

#include <stdio.h>

/**
 * Open the file pointed by pathname and associate a stream with it
 */
FILE *fopen_control_errors(const char *pathname, const char *mode);

/**
 * Flush the stream pointed by stream and close the underlying file descriptor
 */
void fclose_control_errors(FILE *stream);

#endif