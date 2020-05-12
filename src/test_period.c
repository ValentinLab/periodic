#define _POSIX_SOURCE
#define PID_PATH "./tmp/period.pid"
#define NAMED_PIPE_PATH "./tmp/period.fifo"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/message.h"

volatile sig_atomic_t usr1_receive = 0;

void handSIGUSR1(int sig) {
  usr1_receive = 1;
}

int main (int argc, char *argv[]){

  struct sigaction s;
  s.sa_handler = handSIGUSR1;

  FILE* file = fopen(PID_PATH, "w");
  if(file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  pid_t pid = getpid();
  fprintf(file, "%d", pid);
  fclose(file);

 /* Install the signal handler */ 
  printf("Installing signal handler for signal %d\n", SIGUSR1); 
  int retval = sigaction(SIGUSR1, &s, NULL); 
  if(retval == -1) { 
    perror("sigaction failed"); 
    exit(EXIT_FAILURE); 
  }

  int fd = open(NAMED_PIPE_PATH, O_RDONLY);
  while(1) {
    if (usr1_receive){
      //printf("%s\n", recv_string(fd));
      usr1_receive = 0;
    }
    sleep(1);
  }
  close(fd);
  return EXIT_SUCCESS;
}