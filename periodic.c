#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "message.h"

#define PID_PATH "./tmp/period.pid"
#define NAMED_PIPE_PATH "./tmp/period.fifo"
#define MAX_PID_SIZE 4

/**
 * Permet de lire le pid dans un fichier
 * 
 * @param path Le chemin vers le fichier
 * @return Renvoie le pid ou -1 si le fichier n'existe pas
 */ 
int read_pid(char *path) {
  char buf[MAX_PID_SIZE];
  FILE* file = fopen(path, "r");

  if (file == NULL) {
    return -1;
  }

  while (!feof(file)) {
    fread(buf,sizeof(char),MAX_PID_SIZE,file);
  }
  
  printf("pid of period : %s\n",buf);
  return atoi(buf);
}

/**
 * Permet d'envoyer un signal au process celon le PID en paramétre
 */ 
int send_signal (int pid, int signal) {
  int res = kill(pid, signal);
  if (res == -1) {
    perror("send_signal");
    return -1;
  }
  return res;
}

int main(int argc, char *argv[]) {
  if (argc != 1 && argc <= 3) {
    fprintf(stderr,"invalid start\nusage : ./periodic start period cmd [args]...\nusage : ./periodic\n");
    return EXIT_FAILURE;
  }
  
  // Try to read the pid of period
  int pid = read_pid(PID_PATH);
  if(pid == -1) {
    perror("read_pid");
    return EXIT_FAILURE;
  }

  if (argc == 1) { // Si il y a 0 argument
    int signal = send_signal(pid,SIGUSR2);
    if (signal == -1) {
      exit(EXIT_FAILURE);
    }
    printf("Registred command : \n");
    //char *res = recv_string(pid);
    //size_t size = strlen(res);
  } else {  // Si il y a 3 arguments au moins
    if(mkfifo(NAMED_PIPE_PATH, 0666) == -1) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
    if (send_signal(pid,SIGUSR1) == -1) {
      exit(EXIT_FAILURE);
    }
    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    if (fd == -1) {
      perror("open named pipe");
      exit(EXIT_FAILURE);
    }
    send_string(fd, argv[1]);
    send_string(fd, argv[2]);
    send_string(fd, argv[3]);
    /*
    if (argc > 4) {
      size_t sizeArgs = argc-4;
      char** args = calloc(sizeArgs, sizeof(char *));
      for (size_t i = sizeArgs; i < argc; i++) {
        args[i - sizeArgs] = argv[i];
      }
      for (size_t i = 0; i < sizeArgs; i++) {
        printf("%s\n", args[i]);
      }
    }
    */
    close(fd);
    unlink(NAMED_PIPE_PATH);
  }

  return EXIT_SUCCESS;
}
