#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCRIPT_USAGE "when N\n"

#define EXIT_FAILURE 1

int main(int argc, char **argv) {
  // Check arguments number
  if(argc != 2) {
    fprintf(stderr, "Error: one argument needed\n%s", SCRIPT_USAGE);
    return EXIT_FAILURE;
  }
  time_t arg_time = atol(argv[1]);
  if(time < 0) {
    fprintf(stderr, "Error: N must be greater or equals than 0\n%s", SCRIPT_USAGE);
    return EXIT_FAILURE;
  }

  // Get date
  char *arg_date = ctime(&arg_time);
  if(arg_date == NULL) {
    fprintf(stderr, "Error: ctime() function\n");
    return EXIT_FAILURE;
  }

  // Print date
  printf("%s", arg_date);

  return 0;
}