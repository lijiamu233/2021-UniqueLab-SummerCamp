#ifndef FUNC_H
#define FUNC_H
# define MAXLENGTH 25

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void init_mysh();
int parseline(const char *cmdline, char **argv) ;
int builtin(char ** argv);
void sigint_handler(int sig);
void sigchld_handler(int sig);
int io_redirec(char **argv,int mode);
void execute(char **argv);
int pipe_execute(char **argv);
#endif