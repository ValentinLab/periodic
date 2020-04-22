#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 3) {
    fprintf(stderr,"invalid start\nusage : ./periodic start period cmd [arg]...\nusage : ./periodic\n");
    return EXIT_FAILURE;
  }

  FILE *fd = fopen("./tmp/period.pid",'r');

  if (fd == NULL) {
    perror("fopen"); 
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}