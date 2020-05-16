#ifndef MESSAGE_H
#define MESSAGE_H

/**
 * Permet d'envoyer une chaine de caractére via un descripteur de fichier
 * 
 * @param fd Le descripteur de fichier
 * @param str La chaine que l'on souhaite envoyer
 */
int send_string(int fd, const char *str);

/**
 * Permet de recevoir une chaine de caractére via un descripteur de fichier
 * 
 * @param fd Le descripteur de fichier
 */
char *recv_string(int fd);

/**
 * Permet d'envoyer un tableau de chaine de caractére via un descripteur de fichier
 * 
 * @param fd Le descripteur de fichier
 * @param argv Le tableau de chaine de caractére
 */
int send_argv(int fd, char *argv[]);

/**
 * Permet de recevoir un tableau de chaine de caractére via un descripteur de fichier
 * 
 * @param fd Le descripteur de fichier
 */ 
char **recv_argv(int fd);

#endif