#ifndef FUNC_H
#define FUNC_H
# define MAXLENGTH 25
int parseline(const char *cmdline, char **argv) ;
int builtin(char ** argv);
void sigint_handler(int sig);
void sigchld_handler(int sig);
#endif