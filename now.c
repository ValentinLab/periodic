#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "controlsyscall.h"

int main(int argc, char **argv) {
  // Get current time
  time_t now = time(NULL);
  perror_control(now, "Get time (time)");

  // Print time
  printf("%ld", now);

  return EXIT_SUCCESS;
}