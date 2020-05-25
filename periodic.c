#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "controlsyscall.h"
#include "message.h"

#define PID_PATH "/tmp/period.pid"
#define NAMED_PIPE_PATH "/tmp/period.fifo"
#define MAX_PID_SIZE 4

/**
 * Permet de lire le pid dans un fichier
 * 
 * @param path Le chemin vers le fichier
 * @return Renvoie le pid ou -1 si le fichier n'existe pas
 */ 
int read_pid(char *path) {
  char buf[MAX_PID_SIZE];
  FILE* file = fopen_control(path, "r");

  while (!feof(file)) {
    fread(buf,sizeof(char),MAX_PID_SIZE,file);
  }
  
  printf("pid of period : %s\n",buf);
  fclose_control(file);

  return atoi(buf);
}

/**
 * Permet d'envoyer un signal au process celon le PID en paramétre
 */ 
int send_signal (int pid, int signal) {
  int res = kill(pid, signal);
  perror_control(res, "Can't kill (send_signal)");
  return res;
}

int main(int argc, char *argv[]) {
  if (argc != 1 && argc <= 3) {
    fprintf(stderr,"invalid arguments\nusage : ./periodic start period cmd [args]...\nusage : ./periodic\n");
    return EXIT_FAILURE;
  }
  
  // Try to read the pid of period
  int pid = read_pid(PID_PATH);
  perror_control(pid, "Can't read PID (periodic)");

  if (argc == 1) { // Si il y a 0 argument
    int fd = open(NAMED_PIPE_PATH, O_RDWR);
    perror_control(fd, "open named pipe (periodic)");

    perror_control(send_signal(pid, SIGUSR2), "Can't send SIGUSR2 (periodic)");
    char **res = recv_argv(fd);
    
    if(strcmp(res[0], "NULL") == 0) {
      printf("Il n'y a pas de commande.\n");
      free(res[0]);
      free(res);
    } else {
      printf("Registred command :\n");
      printf("N°|start|period|cmd|args\n");
      while (strcmp(res[0], "NULL") != 0) {
        int i = 0;
        while (res[i] != NULL) {
          if(i == 0) {
            printf("%s - ", res[i]);
          } else {
            printf("%s ", res[i]);
          }
          free(res[i]);
          i++;
        }
        printf("\n");
        free(res[i]);
        free(res);
        res = recv_argv(fd);
      }
      free(res[0]);
      free(res);
    }
    close_control(fd);
  } else {  // Si il y a 3 arguments au moins
    char **test_start = calloc(strlen(argv[1]), sizeof(char*));
    char **test_period = calloc(strlen(argv[2]), sizeof(char *));
    
    if(strcmp(argv[1], "now") == 0) {
      time_t now = time(NULL);
      perror_control(now, "Get time (time)");
      char *str;
      sprintf(str, "%ld", now);
      argv[1] = str;
    } else{
      strtol(argv[1], test_start, 10);
      if(strcmp(*test_start, "\0") != 0) {
        fprintf(stderr,"invalid start\nusage : ./periodic start period cmd [args]...\nusage : ./periodic\n");
        free(test_start);
        free(test_period);
        exit(EXIT_FAILURE);
      }
    }
    strtol(argv[2], test_period, 10);
    if(strcmp(*test_period, "\0") != 0) {
      fprintf(stderr,"invalid period\nusage : ./periodic start period cmd [args]...\nusage : ./periodic\n");
      free(test_start);
      free(test_period);
      exit(EXIT_FAILURE);
    }

    perror_control(send_signal(pid, SIGUSR1), "Can't send SIGUSR1 (periodic)");

    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    perror_control(fd, "open named pipe (periodic)");

    send_string(fd, "0");

    // Send argument size
    char numArg[12];
    sprintf(numArg, "%d", argc - 2);

    send_string(fd, numArg);
    send_argv(fd, argv+1);

    free(test_start);
    free(test_period);
    close_control(fd);
  }

  return EXIT_SUCCESS;
}
