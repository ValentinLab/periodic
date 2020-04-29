#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/message.h"

#define FILE_PATH "./tmp/period.pid"
#define MAX_PID_SIZE 10

/**
 * Permet de lire le pid dans un fichier
 * 
 * @param path Le chemin vers le fichier
 * @return Renvoie le pid ou -1 si le fichier n'existe pas
 */ 
int read_pid(char *path) {
  char buf[MAX_PID_SIZE];
  int fd = open(path, O_RDONLY);

  if (fd == -1) {
    perror("open"); 
    return -1;
  }

  read(fd,buf,MAX_PID_SIZE);
  printf("pid : %s\n",buf);
  return atoi(buf);
}

/**
 * Permet d'envoyer un signal au process celon le PID en paramétre
 */ 
int send_signal (int pid, int signal) {
  int res = kill(pid, signal);
  if (res == -1) {
    perror("kill");
    return -1;
  }
  return res;
}

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 3) {
    fprintf(stderr,"invalid start\nusage : ./periodic start period cmd [arg]...\nusage : ./periodic\n");
    return EXIT_FAILURE;
  }

  if (argc == 1) {
    int pid = read_pid(FILE_PATH);
    if(pid == -1) {
      perror("read_pid");
      return EXIT_FAILURE;
    }
    send_signal(pid,SIGUSR2);
    char *res = recv_string(pid);

  } else {  // Si il y a 3 arguments au moins

  }

  return EXIT_SUCCESS;
}