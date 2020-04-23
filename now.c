#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  // Get current time
  time_t now = time(NULL);
  if(now == -1) {
    perror("time");
    return EXIT_FAILURE;
  }

  // Print time
  printf("%ld", now);

  return EXIT_SUCCESS;
}