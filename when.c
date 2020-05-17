#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCRIPT_USAGE "Usage: when N\n"

int main(int argc, char **argv) {
  // Check arguments number
  if(argc != 2) {
    fprintf(stderr, "Error: one argument needed\n%s", SCRIPT_USAGE);
    return EXIT_FAILURE;
  }

  // Get time argument
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

  return EXIT_SUCCESS;
}