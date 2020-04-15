#include <stdio.h>
#include <time.h>

#define EXIT_PERROR 1

int main(int argc, char **argv) {
  // Get current time
  time_t now = time(NULL);
  if(now == -1) {
    perror("time");
    return EXIT_PERROR;
  }

  // Print time
  printf("%ld", now);

  return 0;
}