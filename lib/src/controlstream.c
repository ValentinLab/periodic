#include "controlstream.h"
#include <stdio.h>
#include <stdlib.h>

FILE *fopen_control_errors(const char *pathname, const char *mode) {
  FILE *current_file = fopen(pathname, mode);
  if(current_file == NULL) {
    perror("Open file (fopen)");
    exit(EXIT_FAILURE);
  }

  return current_file;
}

void fclose_control_errors(FILE *stream) {
  int ret = fclose(stream);
  if(ret == EOF) {
    perror("Close file (fclose)");
    exit(EXIT_FAILURE);
  }
}