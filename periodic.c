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
#define GENERIC_USAGE "usage : ./periodic start period cmd [args]...\nusage : ./periodic cmd_number\nusage : ./periodic\n"

/**
 * Permet de lire le pid dans un fichier
 * 
 * @param path Le chemin vers le fichier
 * @return Renvoie le pid ou -1 si le fichier n'existe pas
 */ 
int read_pid(char *path) {
  FILE* file = fopen_control(path, "r");
  char pid[12];

  fgets(pid, 12, file);
  fclose_control(file);

  printf("pid of period : %s\n", pid);
  
  return atoi(pid);
}

/**
 * Permet d'envoyer un signal au process celon le PID en paramétre
 */ 
int send_signal (int pid, int signal) {
  int res = kill(pid, signal);
  perror_control(res, "Can't kill (send_signal)");
  return res;
}

long is_long(char *str, char *err) {
  char **endptr = calloc(strlen(str), sizeof(char *));
  long result = strtol(str, endptr, 10);
  if(strcmp(*endptr, "\0") != 0) {
    fprintf(stderr,"invalid %s\n%s", err, GENERIC_USAGE);
    free(endptr);
    exit(EXIT_FAILURE);
  }
  free(endptr);
  return result;
}

int main(int argc, char *argv[]) {
  if (argc <= 0 || argc == 3) {
    fprintf(stderr,"invalid arguments\n%s", GENERIC_USAGE);
    return EXIT_FAILURE;
  }
  
  // Try to read the pid of period
  int pid = read_pid(PID_PATH);
  perror_control(pid, "Can't read PID (periodic)");

  if (argc == 1) { // Si il y a 0 argument
    int fd = open(NAMED_PIPE_PATH, O_RDWR);
    perror_control(fd, "open (periodic - 1)");

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
        free(res[i]);
        free(res);
        printf("\n");
        res = recv_argv(fd);
      }
      free(res[0]);
      free(res);
    }
    close_control(fd);
  } else if(argc == 2) { // Si il y a 1 argument
    long delete_num = is_long(argv[1], "cmd_number");

    if(delete_num < 0) {
      fprintf(stderr,"cmd_number need to be superior or equal to zero\n%s", GENERIC_USAGE);
      exit(EXIT_FAILURE);
    }

    perror_control(send_signal(pid, SIGUSR1), "Can't send SIGUSR1 (periodic - 1)");

    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    perror_control(fd, "open (periodic - 2)");

    // Define the period in delete mod
    send_string(fd, "1");
    sprintf(argv[1],"%ld",delete_num);
    // Send the cmd num that need to be deleted
    send_string(fd, argv[1]);
    close_control(fd);
  } else {  // Si il y a 3 arguments au moins
    if(strcmp(argv[1], "now") == 0) {
      time_t now = time(NULL);
      sprintf(argv[1], "%ld", now);
    } else{
      long start = is_long(argv[1], "start");
      time_t next_time = time(NULL) + start;
      printf("%ld", next_time);
      sprintf(argv[1], "%ld", next_time);
    }

    is_long(argv[2], "period");

    perror_control(send_signal(pid, SIGUSR1), "Can't send SIGUSR1 (periodic - 2)");

    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    perror_control(fd, "open (periodic - 3)");

    // Define the period in add mod
    send_string(fd, "0");

    // Send argument size
    char numArg[12];
    sprintf(numArg, "%d", argc - 2);

    send_string(fd, numArg);
    send_argv(fd, argv+1);

    close_control(fd);
  }

  return EXIT_SUCCESS;
}
