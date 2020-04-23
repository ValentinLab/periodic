#include <stdio.h>
#include <unistd.h>
#include "controlstream.h"

void write_pid() {
  // Check if PID file already exists
  FILE *f = fopen_control_errors("text.txt", "w");
  fprintf(f, "coucou");
  fclose_control_errors(f);
}

int main(int argc, char **argv) {
  // TODO : question 6.2 (pour la version finale)
  write_pid();
  return 0;
}