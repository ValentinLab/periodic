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

/* ---------- Functions ---------- */

/**
 * Read period's PID in a file
 * 
 * @param path Path to file
 * @return Period's PID or -1
 */ 
int read_pid(char *path) {
  // Open file
  FILE* file = fopen_control(path, "r");
  char pid[12];

  // Read value
  fgets(pid, 12, file);
  fclose_control(file);
  
  return atoi(pid);
}

/**
 * Send a signal to a process
 * 
 * @param pid Process's PID
 * @param signal Signal to send
 * @return Kill returned value
 */ 
int send_signal (int pid, int signal) {
  int res = kill(pid, signal);
  perror_control(res, "can't kill (send_signal)");
  return res;
}

/**
 * Check if a string contains a long number
 * 
 * @param str String to check
 * @param err Error to display
 * @return Str transformed into a long
 */
long is_long(char *str, char *err) {
  char **endptr = calloc(strlen(str), sizeof(char *));
  long result = strtol(str, endptr, 10);
  if(strcmp(*endptr, "\0") != 0) {
    free(endptr);
    exit(EXIT_FAILURE);
  }
  free(endptr);

  if(result < 0) {
    fprintf(stderr, "%s need to be superior or equal to zero\n%s", err, GENERIC_USAGE);
    exit(EXIT_FAILURE);
  }

  return result;
}

/* ---------- Main ---------- */

int main(int argc, char *argv[]) {
  if (argc <= 0 || argc == 3) {
    fprintf(stderr,"invalid arguments\n%s", GENERIC_USAGE);
    return EXIT_FAILURE;
  }
  
  // Try to read the pid of period
  int pid = read_pid(PID_PATH);
  perror_control(pid, "can't read PID (periodic)");

  if (argc == 1) { // Si il y a 0 argument
    int fd = open(NAMED_PIPE_PATH, O_RDWR);
    perror_control(fd, "open (periodic - 1)");

    perror_control(send_signal(pid, SIGUSR2), "can't send SIGUSR2 (periodic)");
    char **res = recv_argv(fd);
    
    if(strcmp(res[0], "NULL") == 0) {
      printf("List of command is empty.\n");
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

    perror_control(send_signal(pid, SIGUSR1), "can't send SIGUSR1 (periodic - 1)");

    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    perror_control(fd, "open (periodic - 2)");

    // Define the period in delete mod
    send_string(fd, "1");
    sprintf(argv[1],"%ld",delete_num);
    // Send the cmd num that need to be deleted
    send_string(fd, argv[1]);
    close_control(fd);
  } else {  // Si il y a 3 arguments au moins
    // Memory allocation for the new array
    char **to_send = calloc(argc - 2, sizeof(char *));

    // 1. Start
    time_t start_time = time(NULL);
    perror_control(start_time, "Get current time (time)");
    if(strcmp(argv[1], "now") != 0) {
      long start = is_long(argv[1], "start");
      if(argv[1][0] == '+') {
        start_time += start;
      } else {
        if(start < start_time) {
          fprintf(stderr, "Error : you can't use past time\n");
          return EXIT_FAILURE;
        }
        start_time = start;
      }
    }
    to_send[0] = calloc(20, sizeof(char));
    sprintf(to_send[0], "%ld", start_time);

    // 2. Period
    long period = is_long(argv[2], "period");
    to_send[1] = calloc(20, sizeof(char));
    sprintf(to_send[1], "%ld", period);

    // Open pipe
    int fd = open(NAMED_PIPE_PATH, O_WRONLY);
    perror_control(fd, "open (periodic - 3)");

    // Define period to "add mod"
    send_string(fd, "0");

    // Send arguments number
    char numArg[12];
    sprintf(numArg, "%d", argc - 2);
    send_string(fd, numArg);

    // Send command datas
    for(size_t i = 2; i < argc-1; ++i) {
      fprintf(stderr, "%ld : %s\n", i, argv[i+1]);
      to_send[i] = calloc(strlen(argv[i+1]), sizeof(char));
      strcpy(to_send[i], argv[i+1]);
      fprintf(stderr, "--\n");
    }
    send_argv(fd, to_send);

    close_control(fd);

    // Send SIGUSR1 to period
    perror_control(send_signal(pid, SIGUSR1), "can't send SIGUSR1 (periodic - 2)");

    // Free memory
    free(to_send[0]);
    free(to_send[1]);
    free(to_send);
  }

  return EXIT_SUCCESS;
}
